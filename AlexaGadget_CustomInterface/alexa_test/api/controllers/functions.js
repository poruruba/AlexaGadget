'use strict';

/* 関数を以下に追加する */
const func_table = {
//  "test-func" : require('./test_func').handler,
//  "test-dialogflow" : require('./test_dialogflow').fulfillment,
};
const alexa_table = {
//  "test-alexa" : require('./lambda/test_alexa').handler,
//  "test-clova": require('./test-clova').handler,
"test-alexa": require('./test-alexa/lambda').handler,
};
const lambda_table = {
//  "test-lambda" : require('./test-lambda').handler,
//  "forward_lambda" : require('./forward_lambda').handler,
};
const express_table = {
//  "test-express": require('./test-express').handler,
};

module.exports = { func_table, alexa_table, lambda_table, express_table };
