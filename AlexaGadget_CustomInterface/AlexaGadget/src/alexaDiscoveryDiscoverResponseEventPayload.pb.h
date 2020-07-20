/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.6 at Wed Jul 15 00:57:07 2020. */

#ifndef PB_ALEXADISCOVERY_ALEXADISCOVERYDISCOVERRESPONSEEVENTPAYLOAD_PB_H_INCLUDED
#define PB_ALEXADISCOVERY_ALEXADISCOVERYDISCOVERRESPONSEEVENTPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification {
    char firmwareVersion[32];
    char deviceToken[65];
    char deviceTokenEncryptionType[32];
    char amazonDeviceType[32];
    char modelName[32];
    char radioAddress[32];
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification) */
} alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification;

typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes {
    char name[32];
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes) */
} alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes;

typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration {
    pb_size_t supportedTypes_count;
    alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes supportedTypes[10];
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration) */
} alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration;

typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities {
    char type[32];
    char interface[32];
    char version[32];
    alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration configuration;
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities) */
} alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities;

typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints {
    char endpointId[32];
    char friendlyName[32];
    char description[32];
    char manufacturerName[32];
    pb_size_t capabilities_count;
    alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities capabilities[32];
    alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification additionalIdentification;
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints) */
} alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints;

typedef struct _alexaDiscovery_DiscoverResponseEventPayloadProto {
    pb_size_t endpoints_count;
    alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints endpoints[1];
/* @@protoc_insertion_point(struct:alexaDiscovery_DiscoverResponseEventPayloadProto) */
} alexaDiscovery_DiscoverResponseEventPayloadProto;

/* Default values for struct fields */

/* Initializer values for message structs */
#define alexaDiscovery_DiscoverResponseEventPayloadProto_init_default {0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_init_default}}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_init_default {"", "", "", "", 0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default}, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_init_default}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_default {"", "", "", alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_init_default}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_init_default {0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default}}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_default {""}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_init_default {"", "", "", "", "", ""}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_init_zero {0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_init_zero}}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_init_zero {"", "", "", "", 0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero}, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_init_zero}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_init_zero {"", "", "", alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_init_zero}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_init_zero {0, {alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero, alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero}}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_init_zero {""}
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_init_zero {"", "", "", "", "", ""}

/* Field tags (for use in manual encoding/decoding) */
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_modelName_tag 5
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_deviceTokenEncryptionType_tag 3
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_firmwareVersion_tag 1
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_amazonDeviceType_tag 4
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_radioAddress_tag 6
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_deviceToken_tag 2
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_name_tag 1
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_supportedTypes_tag 1
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_configuration_tag 4
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_type_tag 1
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_interface_tag 2
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_version_tag 3
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_capabilities_tag 11
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_additionalIdentification_tag 12
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_endpointId_tag 1
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_manufacturerName_tag 4
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_description_tag 3
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_friendlyName_tag 2
#define alexaDiscovery_DiscoverResponseEventPayloadProto_endpoints_tag 1

/* Struct field encoding specification for nanopb */
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_fields[2];
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_fields[7];
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_fields[5];
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_fields[2];
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_fields[2];
extern const pb_field_t alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_fields[7];

/* Maximum encoded size of messages (where known) */
#define alexaDiscovery_DiscoverResponseEventPayloadProto_size 15356
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_size 15352
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_size 465
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_size 360
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_Capabilities_Configuration_SupportedTypes_size 34
#define alexaDiscovery_DiscoverResponseEventPayloadProto_Endpoints_AdditionalIdentification_size 237

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define ALEXADISCOVERYDISCOVERRESPONSEEVENTPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
