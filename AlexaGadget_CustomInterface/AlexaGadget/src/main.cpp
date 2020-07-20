//#include <Arduino.h>
#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <ArduinoJson.h>

#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "common.h"
#include "accessories.pb.h"
#include "directiveParser.pb.h"
#include "eventParser.pb.h"
#include "alexaDiscoveryDiscoverResponseEvent.pb.h"
#include "alexaDiscoveryDiscoverDirective.pb.h"
#include "notificationsSetIndicatorDirective.pb.h"
#include "alexaGadgetStateListenerStateUpdateDirective.pb.h"
#include "alexaDiscoveryDiscoverResponseEventPayload.pb.h"

#include "customEvent.pb.h"

const char *AmazonId = "ÅyAmazonIDÅz";
const char *AlexaGadgetSecret = "ÅyAlexaÉKÉWÉFÉbÉgÇÃÉVÅ[ÉNÉåÉbÉgÅz";
const char *EndpointId = "ÅyÉKÉWÉFÉbÉgÇÃéØï éqÅz";
const char *FriendlyName ="ÅyêªïiIDÅz";
const char *DeviceToken = "ÅyÉfÉoÉCÉXÉgÅ[ÉNÉìÅz";
const char *FirmwareVersion = "ÅyÉtÉ@Å[ÉÄÉEÉFÉAÉoÅ[ÉWÉáÉìÅz";
const char *ModelName = "ÅyÉÇÉfÉãñºÅz";
const char *RadioAddress = "ÅyESP32ÇÃBLEÇÃMacAddressÅz";

#define SAMPLE_MAX_TRANSACTION_SIZE (512U)
#define SAMPLE_NEGOTIATED_MTU       (23U)

#define UUID_SERVICE_SHORT  0xFE03
#define BT_VENDOR_ID        0x0171

#define UUID_SERVICE BLEUUID((uint16_t)UUID_SERVICE_SHORT)
#define UUID_WRITE "F04EB177-3005-43A7-AC61-A390DDF83076"
#define UUID_NOTIFY "2BEEA05B-1879-4BB4-8A2F-72641F82420B"

long sendPacket(stream_id_t stream_id, const uint8_t *p_buffer, uint16_t buffer_len);
void resetPacket(void);
long appendPacket(const uint8_t *p_buffer, uint16_t buffer_len);
long createPacket(stream_id_t stream_id, uint8_t trxn_id, uint8_t seq_no, const uint8_t *p_payload, uint16_t payload_len, uint8_t *p_buffer, uint16_t *p_buffer_len);
ControlEnvelope makeDeviceInformationResponse(const char *p_serial_number, const char *p_name, const char *p_device_type);
ControlEnvelope makeDeviceFeatureResponse(uint64_t features);
alexaDiscovery_DiscoverResponseEventProto makeDiscoveryDiscoverEvent(const char *p_endpointId, const char *p_friendlyName,
                                                 const alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification *p_identification);

BLECharacteristic *pCharacteristic_write;
BLECharacteristic *pCharacteristic_notify;

bool connected = false;
bool isBonded = false;
uint8_t value_write[SAMPLE_NEGOTIATED_MTU] = {0x00};
uint8_t value_read[SAMPLE_NEGOTIATED_MTU] = {0x00};

uint8_t gp_receive_buffer[SAMPLE_MAX_TRANSACTION_SIZE];
uint16_t g_receive_total_len;
uint8_t g_trxn_id;
bool g_ack_bit;
uint8_t gp_send_buffer[SAMPLE_MAX_TRANSACTION_SIZE];

ControlEnvelope controlEnvelope;
static const ControlEnvelope controlEnvelope_zero = ControlEnvelope_init_default;
alexaDiscovery_DiscoverResponseEventProto discover_response_envelope;
ControlEnvelope get_device_info_controlEnvelope;
ControlEnvelope get_device_features_controlEnvelope;

const int message_capacity = JSON_OBJECT_SIZE(2);
StaticJsonDocument<message_capacity> json_message;
char message_buffer[255];

void debug_dump(const uint8_t *p_bin, uint16_t len){
  for( uint16_t i = 0 ; i < len ; i++ ){
    Serial.print((p_bin[i] >> 4) & 0x0f, HEX);
    Serial.print(p_bin[i] & 0x0f, HEX);
  }
  Serial.println("");
}

class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    connected = true;

    Serial.println("Connected\n");
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
    BLE2902* desc = (BLE2902*)pCharacteristic_notify->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);

    Serial.println("Disconnected\n");
  }
};

class MyDescriptorCallback : public BLEDescriptorCallbacks{
  void onWrite(BLEDescriptor* pDescriptor){
    Serial.println("onWrite(Descriptor)");

    BLE2902* desc = (BLE2902*)pCharacteristic_notify->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    if( !desc->getNotifications() )
      return;

    uint8_t protocol_version_packet[PROTOCOL_VERSION_PACKET_SIZE] = {
        (PROTOCOL_IDENTIFIER >> 8) & 0xff,
        (PROTOCOL_IDENTIFIER >> 0U) & 0xff,
        PROTOCOL_VERSION_MAJOR,
        PROTOCOL_VERSION_MINOR,
        (SAMPLE_NEGOTIATED_MTU >> 8) & 0xff,
        (SAMPLE_NEGOTIATED_MTU >> 0) & 0xff,
        (SAMPLE_MAX_TRANSACTION_SIZE >> 8) & 0xff,
        (SAMPLE_MAX_TRANSACTION_SIZE >> 0) & 0xff,
    };
    pCharacteristic_notify->setValue(protocol_version_packet, PROTOCOL_VERSION_PACKET_SIZE);
    pCharacteristic_notify->notify();
    Serial.println("Notified");
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic* pCharacteristic){
    Serial.println("onWrite");
    uint8_t* value = pCharacteristic->getData();
    std::string str = pCharacteristic->getValue();    
    uint16_t len = str.length();
    bool status;
    long result;

    result = appendPacket(value, len);
    if( result < 0 ){
      resetPacket();
      return;
    }else if( result > 0 ){
      return;
    }
    resetPacket();

    debug_dump(gp_receive_buffer, g_receive_total_len);
    Serial.printf("ack=%d\n", g_ack_bit);

    if( ((value[0] >> 4) & 0x0f) == CONTROL_STREAM ){
      memmove(&controlEnvelope, &controlEnvelope_zero, sizeof(ControlEnvelope));
      pb_istream_t istream = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
      status = pb_decode(&istream, ControlEnvelope_fields, &controlEnvelope);
      if( !status ) {
        Serial.println("pb_decode Error");
        return;
      }
      Serial.printf("Command=%d\n", controlEnvelope.command);

      if( controlEnvelope.command == Command_GET_DEVICE_INFORMATION ){
        Serial.println("Command_GET_DEVICE_INFORMATION");

        pb_ostream_t ostream = pb_ostream_from_buffer(gp_send_buffer, sizeof(gp_send_buffer));
        status = pb_encode(&ostream, ControlEnvelope_fields, &get_device_info_controlEnvelope);
        if (!status) {
          Serial.println("pb_encode Error");
          return;
        }
        Serial.printf("bytes_written=%d\n", ostream.bytes_written);

        sendPacket(CONTROL_STREAM, gp_send_buffer, ostream.bytes_written);
      }else if( controlEnvelope.command == Command_GET_DEVICE_FEATURES ){
        Serial.println("Command_GET_DEVICE_FEATURES");

        pb_ostream_t ostream = pb_ostream_from_buffer(gp_send_buffer, sizeof(gp_send_buffer));
        status = pb_encode(&ostream, ControlEnvelope_fields, &get_device_features_controlEnvelope);
        if( !status ){
          Serial.println("pb_encode Error");
          return;
        }
        Serial.printf("bytes_written=%d\n", ostream.bytes_written);

        sendPacket(CONTROL_STREAM, gp_send_buffer, ostream.bytes_written);
      }else{
        Serial.println("Not Supported");
      }
    }else if( ((value[0] >> 4) & 0x0f) == ALEXA_STREAM){
      Serial.println("Alexa stream");

      directive_DirectiveParserProto directive_envelope = directive_DirectiveParserProto_init_default;
      pb_istream_t istream = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
      status = pb_decode(&istream, directive_DirectiveParserProto_fields, &directive_envelope);
      if( !status ){
        Serial.println("pb_decode Error");
        return;
      }
      Serial.printf("name = %s, namespace=%s\n", directive_envelope.directive.header.name, directive_envelope.directive.header.namespacc);

      if (0 == strcmp(directive_envelope.directive.header.name, "SetIndicator") && (0 == strcmp(directive_envelope.directive.header.namespacc, "Notifications"))) {
        pb_istream_t istream_notification = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
        notifications_SetIndicatorDirectiveProto notifications_envelope = notifications_SetIndicatorDirectiveProto_init_default;
        status = pb_decode(&istream_notification, notifications_SetIndicatorDirectiveProto_fields, &notifications_envelope);
        if( !status ){
          Serial.println("pb_decode Error");
          return;
        }

        Serial.printf("visualIndicator:%d, audioIndicator=%d, assetId=%s, url=%s\n",
          notifications_envelope.directive.payload.persistVisualIndicator,
          notifications_envelope.directive.payload.playAudioIndicator,
          notifications_envelope.directive.payload.asset.assetId,
          notifications_envelope.directive.payload.asset.url);

      } else if (0 == strcmp(directive_envelope.directive.header.name, "Discover") && (0 == strcmp(directive_envelope.directive.header.namespacc, "Alexa.Discovery"))) {
        pb_istream_t istream_discovery = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
        alexaDiscovery_DiscoverDirectiveProto discovery_envelope = alexaDiscovery_DiscoverDirectiveProto_init_default;
        status = pb_decode(&istream_discovery, alexaDiscovery_DiscoverDirectiveProto_fields, &discovery_envelope);
        if( !status ){
          Serial.println("pb_decode Error");
          return;
        }
        Serial.printf("scope type: %s\n", discovery_envelope.directive.payload.scope.type);
        Serial.printf("scope token: %s\n", discovery_envelope.directive.payload.scope.token);

        pb_ostream_t ostream = pb_ostream_from_buffer(gp_send_buffer, sizeof(gp_send_buffer));
        status = pb_encode(&ostream, alexaDiscovery_DiscoverResponseEventProto_fields, &discover_response_envelope);
        if (!status){
          Serial.println("pb_encode Error");
          return;
        }
        Serial.printf("bytes_written=%d\n", ostream.bytes_written);

        sendPacket(ALEXA_STREAM, gp_send_buffer, ostream.bytes_written);
      }else
      if (0 == strcmp(directive_envelope.directive.header.name, "StateUpdate") && (0 == strcmp(directive_envelope.directive.header.namespacc, "Alexa.Gadget.StateListener"))){
        pb_istream_t istream_statusupdate = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
        alexaGadgetStateListener_StateUpdateDirectiveProto statusupdate_envelope = alexaGadgetStateListener_StateUpdateDirectiveProto_init_default;
        status = pb_decode(&istream_statusupdate, alexaGadgetStateListener_StateUpdateDirectiveProto_fields, &statusupdate_envelope);
        if( !status ){
          Serial.println("pb_decode Error");
          return;
        }
        int states_count = statusupdate_envelope.directive.payload.states_count;
        for (int i = 0; i < states_count; ++i) {
          Serial.printf("state name: %s\n", statusupdate_envelope.directive.payload.states[i].name);
          Serial.printf("state value: %s\n", statusupdate_envelope.directive.payload.states[i].value);

          if( strcmp(statusupdate_envelope.directive.payload.states[i].name, "wakeword") == 0 ){
            if( strcmp(statusupdate_envelope.directive.payload.states[i].value, "active") == 0 ){
              digitalWrite(GPIO_NUM_10, LOW);
            }else if(strcmp(statusupdate_envelope.directive.payload.states[i].value, "cleared") == 0){
              digitalWrite(GPIO_NUM_10, HIGH);
            }
          }
        }
      }else
      if (0 == strcmp(directive_envelope.directive.header.name, "ClearIndicator") && (0 == strcmp(directive_envelope.directive.header.namespacc, "Notifications"))){
        // Do nothing
      }else
      if (0 == strcmp(directive_envelope.directive.header.name, "Button") && (0 == strcmp(directive_envelope.directive.header.namespacc, "Custom.Sample"))){
        pb_istream_t istream_directive = pb_istream_from_buffer(gp_receive_buffer, g_receive_total_len);
        directive_DirectiveParserProto envelope = directive_DirectiveParserProto_init_default;         
        status = pb_decode(&istream_directive, directive_DirectiveParserProto_fields, &envelope);
        if( !status ){
          Serial.println("pb_decode Error");
          return;
        }
        Serial.printf("payload=%s\n", envelope.directive.payload.bytes); 
        DeserializationError err = deserializeJson(json_message, envelope.directive.payload.bytes);
        if( err ){
          Serial.println("Deserialize error");
          Serial.println(err.c_str());
          return;
        }
        const char* message = json_message["message"];
        Serial.println(message);
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextSize(4);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print(message);
      }else{
        Serial.println("Not Supported");
      }
    }
  }

  void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code){
    Serial.println("onStatus");
  }
/*
  void onRead(BLECharacteristic *pCharacteristic){
    Serial.println("onRead");
  }
*/
};

void taskServer(void*) {
  get_device_info_controlEnvelope = makeDeviceInformationResponse(EndpointId, FriendlyName, AmazonId);
  get_device_features_controlEnvelope = makeDeviceFeatureResponse(0x0011);

  alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification additionalIdentification = alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_init_default;
  strcpy(additionalIdentification.firmwareVersion, FirmwareVersion);
  strcpy(additionalIdentification.deviceToken, DeviceToken);
  strcpy(additionalIdentification.deviceTokenEncryptionType, "1");
  strcpy(additionalIdentification.amazonDeviceType, AmazonId);
  strcpy(additionalIdentification.modelName, ModelName);
  strcpy(additionalIdentification.radioAddress, RadioAddress);
  discover_response_envelope = makeDiscoveryDiscoverEvent(AmazonId, FriendlyName, &additionalIdentification);

  BLEDevice::init("M5StickC");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  BLEService *pService = pServer->createService(UUID_SERVICE);

  pCharacteristic_write = pService->createCharacteristic( UUID_WRITE, BLECharacteristic::PROPERTY_WRITE );
  pCharacteristic_write->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);
  pCharacteristic_write->setValue(value_write, sizeof(value_write));
  pCharacteristic_write->setCallbacks(new MyCharacteristicCallbacks());

  pCharacteristic_notify = pService->createCharacteristic( UUID_NOTIFY, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_notify->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED);
  pCharacteristic_notify->setValue(value_read, sizeof(value_read));
  BLEDescriptor *pDescriptor = new BLE2902();
  pDescriptor->setCallbacks(new MyDescriptorCallback());
  pCharacteristic_notify->addDescriptor(pDescriptor);

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(UUID_SERVICE);
  BLEAdvertisementData advertisementData = BLEAdvertisementData();
  advertisementData.setFlags(0x06);
  std::string strServiceData = "";
  strServiceData += (char)23;
  strServiceData += (char)0x16;
  strServiceData += (char)(UUID_SERVICE_SHORT & 0xff);
  strServiceData += (char)((UUID_SERVICE_SHORT >> 8) & 0xff);
  strServiceData += (char)(BT_VENDOR_ID & 0xff);
  strServiceData += (char)((BT_VENDOR_ID >> 8) & 0xff);
  strServiceData += (char)0x00;
  strServiceData += (char)0xff;
  strServiceData += (char)0x00;
  strServiceData += (!isBonded) ? (char)0x00 : (char)0x01;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  strServiceData += (char)0x00;
  advertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->start();

  vTaskDelay(portMAX_DELAY); //delay(portMAX_DELAY);
}

void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Axp.ScreenBreath(9);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Alexa Gadget");
  delay(500);

  Serial.begin(9600);
  Serial.println("setup");

  pinMode(GPIO_NUM_10, OUTPUT);
  digitalWrite(GPIO_NUM_10, HIGH);

  xTaskCreate(taskServer, "server", 40000, NULL, 5, NULL);
}

int count = 0;

void loop() {
  M5.update();

  if ( M5.BtnA.wasPressed() ){
    Serial.println("M5.BtnA.wasPressed");
    if( connected ){
      BLE2902* desc = (BLE2902*)pCharacteristic_notify->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
      if( desc->getNotifications() ){
        CustomEventProto custom_event_envelope = CustomEventProto_init_default;
        strcpy(custom_event_envelope.event.header.namespacc, "Custom.Sample");
        strcpy(custom_event_envelope.event.header.name, "ButtonStatus");

        json_message.clear();
        json_message["button"] = "M5.BtnA";
        json_message["count"] = ++count;
        serializeJson(json_message, message_buffer, sizeof(message_buffer));
        strcpy(custom_event_envelope.event.payload, message_buffer);

        pb_ostream_t ostream = pb_ostream_from_buffer(gp_send_buffer, sizeof(gp_send_buffer));
        bool status = pb_encode(&ostream, CustomEventProto_fields, &custom_event_envelope);
        if (status) {
          Serial.printf("bytes_written=%d\n", ostream.bytes_written);
          sendPacket(ALEXA_STREAM, gp_send_buffer, ostream.bytes_written);
        }else{
          Serial.println("pb_encode Error");
        }
      }
    }
    delay(100);
  }
}


// extra function
static stream_id_t g_stream_id;
static uint16_t g_buffer_offset = 0;
static uint8_t g_seq_no;

void resetPacket(void){
    g_buffer_offset = 0;
}

long appendPacket(const uint8_t *p_buffer, uint16_t buffer_len){
    if( g_buffer_offset == 0){
        if( buffer_len < 5)
            return -1;
        g_stream_id = (stream_id_t)((p_buffer[0] >> 4) & 0x0f);
        g_trxn_id = p_buffer[0] & 0x0f;
        g_seq_no = (p_buffer[1] >> 4) & 0x0f;
        g_ack_bit = (p_buffer[1] & 0x02) != 0x00;
        if( p_buffer[1] & 0x01 ){
            Serial.println("EXT Not supported");
            return -2;
        }
        g_receive_total_len = (p_buffer[3] << 8) | p_buffer[4];
        if( g_receive_total_len > SAMPLE_MAX_TRANSACTION_SIZE)
            return -3;
        uint16_t unit = p_buffer[5];
        if( (6 + unit) < buffer_len )
            return -4;
        memmove(&gp_receive_buffer[g_buffer_offset], &p_buffer[6], unit);
        g_buffer_offset += unit;
    }else{
        if( buffer_len < 3)
            return -5;
        if( p_buffer[0] != ((g_stream_id << 4) | g_trxn_id) )
            return -6;
        g_seq_no = (p_buffer[1] >> 4) & 0x0f;
        g_ack_bit = (p_buffer[1] & 0x02) != 0x00;
        if( p_buffer[1] & 0x01 ){
            Serial.println("EXT Not supported");
            return -7;
        }
        uint16_t unit = p_buffer[2];
        if( unit == 0 )
          return -8;
        if( g_buffer_offset + unit > g_receive_total_len)
            return -9;
        if( (4 + unit) < buffer_len )
            return -10;
        memmove(&gp_receive_buffer[g_buffer_offset], &p_buffer[3], unit);
        g_buffer_offset += unit;
    }

    return g_receive_total_len - g_buffer_offset;
}

long sendPacket(stream_id_t stream_id, const uint8_t *p_buffer, uint16_t buffer_len){
  uint8_t seq = 0;
  long result_len;

  Serial.println("sendPacket");
  debug_dump(p_buffer, buffer_len);
  do{
      uint16_t packet_len = sizeof(value_read);
      result_len = createPacket(stream_id, g_trxn_id, seq, p_buffer, buffer_len, value_read, &packet_len);
      if( result_len < 0 ){
          Serial.printf("Error result_len=%ld\n", result_len);
          return result_len;
      }
      pCharacteristic_notify->setValue(value_read, packet_len);
      pCharacteristic_notify->notify();

      seq++;
  }while(result_len > 0);

  Serial.println("Notify End");
  return 0;
}

long createPacket(stream_id_t stream_id, uint8_t trxn_id, uint8_t seq_no, const uint8_t *p_payload, uint16_t payload_len, uint8_t *p_buffer, uint16_t *p_buffer_len){
    uint16_t index = 0;
    uint16_t rest;
    uint16_t unit;

    if( *p_buffer_len < 6)
        return -1;

    p_buffer[index++] = ((stream_id & 0x0f) << 4) | (trxn_id & 0x0f);
    p_buffer[index] = ((seq_no & 0x0f) << 4);
    if( seq_no == 0 ){
        p_buffer[index++] |= TRANSACTION_TYPE_INITIAL << 2;
        p_buffer[index++] = 0x00;
        p_buffer[index++] = (payload_len >> 8) & 0xff;
        p_buffer[index++] = payload_len & 0xff;
        rest = payload_len;
        unit = (rest <= (SAMPLE_NEGOTIATED_MTU - 6)) ? rest : (SAMPLE_NEGOTIATED_MTU - 6);
        p_buffer[index++] = unit;
        if( *p_buffer_len < (6 + unit) )
            return -1;
        memmove(&p_buffer[index], &p_payload[0], unit);
        index += unit;
    }else{
        if( (payload_len - (SAMPLE_NEGOTIATED_MTU - 6) - (SAMPLE_NEGOTIATED_MTU - 3) * (seq_no - 1) ) <= 0 )
            return -1;
        
        rest = payload_len - (SAMPLE_NEGOTIATED_MTU - 6) - (SAMPLE_NEGOTIATED_MTU - 3) * (seq_no - 1);
        unit = (rest <= (SAMPLE_NEGOTIATED_MTU - 3)) ? rest : (SAMPLE_NEGOTIATED_MTU - 3);
        if( (rest - unit) > 0 )
            p_buffer[index++] |= TRANSACTION_TYPE_CONTINUE << 2;
        else
            p_buffer[index++] |= TRANSACTION_TYPE_FINAL << 2;
        p_buffer[index++] = unit;
        if( *p_buffer_len < (3 + unit) )
            return -1;
        memmove(&p_buffer[index], &p_payload[payload_len - rest], unit);
        index += unit;
    }
    *p_buffer_len = index;

    return rest - unit;
}

ControlEnvelope makeDeviceInformationResponse(const char *p_serial_number, const char *p_name, const char *p_device_type){
  ControlEnvelope get_device_info_controlEnvelope = ControlEnvelope_init_default;
  get_device_info_controlEnvelope.command = Command_GET_DEVICE_INFORMATION;
  get_device_info_controlEnvelope.which_payload = ControlEnvelope_response_tag;
  get_device_info_controlEnvelope.payload.response.error_code = ErrorCode_SUCCESS;
  get_device_info_controlEnvelope.payload.response.which_payload = Response_device_information_tag;
  DeviceInformation *deviceInformation = &get_device_info_controlEnvelope.payload.response.payload.device_information;

  strcpy(deviceInformation->serial_number, p_serial_number);
  strcpy(deviceInformation->name, p_name);
  deviceInformation->supported_transports_count = 1;
  deviceInformation->supported_transports[0] = Transport_BLUETOOTH_LOW_ENERGY;
  strcpy(deviceInformation->device_type, p_device_type);

  return get_device_info_controlEnvelope;
}

ControlEnvelope makeDeviceFeatureResponse(uint64_t features){
  ControlEnvelope get_device_features_controlEnvelope = ControlEnvelope_init_default;
  get_device_features_controlEnvelope.command = Command_GET_DEVICE_FEATURES;
  get_device_features_controlEnvelope.which_payload = ControlEnvelope_response_tag;
  get_device_features_controlEnvelope.payload.response.error_code = ErrorCode_SUCCESS;
  get_device_features_controlEnvelope.payload.response.which_payload = Response_device_features_tag;
  DeviceFeatures *deviceFeatures = &get_device_features_controlEnvelope.payload.response.payload.device_features;
  deviceFeatures->features = features;
  deviceFeatures->device_attributes = 0x00;

  return get_device_features_controlEnvelope;
}

alexaDiscovery_DiscoverResponseEventProto makeDiscoveryDiscoverEvent(const char *p_endpointId, const char *p_friendlyName,
                                                 const alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification *p_identification){
  alexaDiscovery_DiscoverResponseEventProto discover_response_envelope = alexaDiscovery_DiscoverResponseEventProto_init_default;

  strcpy(discover_response_envelope.event.header.namespacc, "Alexa.Discovery");
  strcpy(discover_response_envelope.event.header.name, "Discover.Response");

  discover_response_envelope.event.payload.endpoints_count = 1;
  strcpy(discover_response_envelope.event.payload.endpoints[0].endpointId, p_endpointId);
  strcpy(discover_response_envelope.event.payload.endpoints[0].friendlyName, p_friendlyName);
  discover_response_envelope.event.payload.endpoints[0].capabilities_count = 3;

  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].type, "AlexaInterface");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].interface, "Notifications");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[0].version, "1.0");

  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].type, "AlexaInterface");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].interface, "Alexa.Gadget.StateListener");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].version, "1.0");
  discover_response_envelope.event.payload.endpoints[0].capabilities[1].configuration.supportedTypes_count = 1;
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[1].configuration.supportedTypes[0].name, "wakeword");

  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].type, "AlexaInterface");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].interface, "Custom.Sample");
  strcpy(discover_response_envelope.event.payload.endpoints[0].capabilities[2].version, "1.0");

  discover_response_envelope.event.payload.endpoints[0].additionalIdentification = *p_identification;

  return discover_response_envelope; 
}
