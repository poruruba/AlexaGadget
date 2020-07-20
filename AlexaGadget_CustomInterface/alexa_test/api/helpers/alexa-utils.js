'use strict';

var AWS = require('aws-sdk');
AWS.config.update({region: 'ap-northeast-1'});

//const Adapter = require('ask-sdk-dynamodb-persistence-adapter');
//const config = {tableName: 'AskPersistentAttributes', createTable: true};
//var adapter = new Adapter.DynamoDbPersistenceAdapter(config);   

class AlexaUtils{
    constructor(alexa, adapter){
        this.alexa = alexa;
        this.skillBuilder = alexa.SkillBuilders.custom();
        this.DynamoDBAdapter = adapter;        
    }

    intent( matcher, handle ){
        this.skillBuilder.addRequestHandlers(new BaseIntentHandler(matcher, handle));
    }

    customReceived( handle ){
        this.skillBuilder.addRequestHandlers(new BaseIntentHandler("CustomInterfaceController.EventsReceived", handle));
    }
    customExpired( handle ){
        this.skillBuilder.addRequestHandlers(new BaseIntentHandler("CustomInterfaceController.Expired", handle));
    }

    errorIntent( handle ){
        ErrorHandler.handle = handle;
    }

    getAttributes( handlerInput ){
        return handlerInput.attributesManager.getSessionAttributes();
    }

    setAttributes( handlerInput, attributes){
        handlerInput.attributesManager.setSessionAttributes(attributes);
    }

    async getPersistentAttributes( handlerInput ){
        return handlerInput.attributesManager.getPersistentAttributes();
    }

    setPersistentAttributes( handlerInput, attributes){
        handlerInput.attributesManager.setPersistentAttributes(attributes);
    }

    async savePersistentAttributes( handlerInput ){
        handlerInput.attributesManager.savePersistentAttributes();
    }

    getSlotId(slot){
        if( slot.resolutions.resolutionsPerAuthority[0].status.code != "ER_SUCCESS_MATCH" )
            return null;
        return slot.resolutions.resolutionsPerAuthority[0].values[0].value.id;
    }

    getSlots( handlerInput ){
        return handlerInput.requestEnvelope.request.intent.slots;
    }

    getAccessToken(handlerInput){
        return handlerInput.requestEnvelope.context.System.user.accessToken;
    }

    getConnectedEndpoints(handlerInput){
        return handlerInput.serviceClientFactory.getEndpointEnumerationServiceClient().getEndpoints()
        .then(response =>{
            return response.endpoints;
        });
    }
    
    buildCustomDirective(endpointId, namespace, name, payload) {
        return {
            type: 'CustomInterfaceController.SendDirective',
            header: {
                name: name,
                namespace: namespace
            },
            endpoint: {
                endpointId: endpointId
            },
            payload: payload
        };
    }
    
    buildStartEventHandlerDirective(token, namespace, name, expirationPayload, durationMs) {
        return {
            type: "CustomInterfaceController.StartEventHandler",
            token: token,
            eventFilter: {
                filterExpression: {
                    'and': [
                        { '==': [{ 'var': 'header.namespace' }, namespace] },
                        { '==': [{ 'var': 'header.name' }, name] }
                    ]
                },
                filterMatchAction: 'SEND_AND_TERMINATE'
            },
            expiration: {
                durationInMilliseconds: durationMs,
                expirationPayload: expirationPayload
            }
        };
    }
    
    buildStopEventHandlerDirective(token) {
        return {
            type: "CustomInterfaceController.StopEventHandler",
            token: token
        };
    }

    lambda(){
        if( this.DynamoDBAdapter ){
            return this.skillBuilder
            .addErrorHandlers(ErrorHandler)
            .withPersistenceAdapter(this.DynamoDBAdapter)
            .withApiClient(new this.alexa.DefaultApiClient())
            .lambda();
        }else{
            return this.skillBuilder
            .addErrorHandlers(ErrorHandler)
            .withApiClient(new this.alexa.DefaultApiClient())
            .lambda();
        }
    }
};

class BaseIntentHandler{
    constructor(matcher, handle){
        this.matcher = matcher;
        this.myhandle = handle;
    }

    canHandle(handlerInput) {
        if( this.matcher == 'LaunchRequest'){
            return handlerInput.requestEnvelope.request.type === 'LaunchRequest';
        }else if( this.matcher == 'HelpIntent' ){
            return handlerInput.requestEnvelope.request.type === 'IntentRequest'
                && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.HelpIntent';
        }else if( this.matcher == 'CancelIntent' ){
            return handlerInput.requestEnvelope.request.type === 'IntentRequest'
                && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.CancelIntent';            
        }else if( this.matcher == 'StopIntent'){
            return handlerInput.requestEnvelope.request.type === 'IntentRequest'
                && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.StopIntent';
        }else if( this.matcher == 'SessionEndedRequest'){
            return handlerInput.requestEnvelope.request.type === 'SessionEndedRequest';
        }else if( this.matcher == 'NavigateHomeIntent'){
            return handlerInput.requestEnvelope.request.type === 'IntentRequest'
                && handlerInput.requestEnvelope.request.intent.name === 'AMAZON.NavigateHomeIntent';
        }else if( this.matcher == 'CustomInterfaceController.EventsReceived' ){
            return handlerInput.requestEnvelope.request.type === 'CustomInterfaceController.EventsReceived';
        }else if( this.matcher == 'CustomInterfaceController.Expired' ){
            return handlerInput.requestEnvelope.request.type === 'CustomInterfaceController.Expired';
        }else{
            return handlerInput.requestEnvelope.request.type === 'IntentRequest'
                && handlerInput.requestEnvelope.request.intent.name === this.matcher;
        }
    }

    async handle(handlerInput) {
        console.log('handle: ' + this.matcher + ' called');
        console.log(handlerInput);
        try{
            return await this.myhandle(handlerInput);
        }catch(error){
            console.error(error);
            throw error;
        }
    }
}
  
const ErrorHandler = {
    canHandle() {
        return true;
    },

    handle(handlerInput, error) {
        console.log(`Error handled: ${error.message}`);
        console.log(handlerInput);
        console.log(`type: ${handlerInput.requestEnvelope.request.type}`);
        return handlerInput.responseBuilder
            .speak('よく聞き取れませんでした。')
            .reprompt('もう一度お願いします。')
            .getResponse();
    },
};

module.exports = AlexaUtils;
