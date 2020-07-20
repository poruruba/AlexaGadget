'use strict';

const Alexa = require('ask-sdk-core');

const HELPER_BASE = process.env.HELPER_BASE || '../../../helpers/';
const AskUtils = require(HELPER_BASE + 'alexa-utils');
const app = new AskUtils(Alexa);

const custom_event_token = '1234abcd-40bb-11e9-9527-6b98b093d166';

app.intent('LaunchRequest', async (handlerInput) =>{
    console.log(handlerInput);

    var endpoints = await app.getConnectedEndpoints(handlerInput);
    if (endpoints.length == 0) {
        console.error("No endpoints");
        var builder = handlerInput.responseBuilder;
        builder.speak('エンドポイントを取得できませんでした。');
        return builder.getResponse();
    }

    var sessionAttributes = app.getAttributes(handlerInput);
    sessionAttributes.endpointId = endpoints[0].endpointId;
    app.setAttributes(handlerInput, sessionAttributes)

    var endpointId = app.getAttributes(handlerInput).endpointId;

    var payload = {
        message: "Hello"
    };

    var builder = handlerInput.responseBuilder;
    builder.speak('こんにちは');
    builder.reprompt('どんな御用ですか');
    builder.addDirective(app.buildCustomDirective(endpointId, 'Custom.Sample', 'Button', payload));
    return builder.getResponse();
});

app.intent('Thanks', async (handlerInput) =>{
    var payload = {
        message: "Push Button",
        number: 1234
    };

    var endpointId = app.getAttributes(handlerInput).endpointId;

    var builder = handlerInput.responseBuilder;
    builder.speak('どういたしまして。ボタンを押してください');
    builder.addDirective(app.buildStartEventHandlerDirective(custom_event_token, "Custom.Sample", "ButtonStatus", { message: "ボタンが押されませんでした" }, 10000));
    builder.addDirective(app.buildCustomDirective(endpointId, 'Custom.Sample', 'Button', payload));
    return builder.getResponse();
});

app.customReceived(async (handlerInput) =>{
    console.log(handlerInput.requestEnvelope.request.events[0].payload);

    var builder = handlerInput.responseBuilder;
    builder.speak('ボタンが押されました');
    builder.withShouldEndSession(true);
    return builder.getResponse();
});

app.customExpired(async (handlerInput) =>{
    var builder = handlerInput.responseBuilder;
    builder.speak(handlerInput.requestEnvelope.request.expirationPayload.message);
    builder.withShouldEndSession(true);
    return builder.getResponse();
});

exports.handler = app.lambda();