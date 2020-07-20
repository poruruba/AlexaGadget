'use strict';

const fs = require('fs');

const { func_table, alexa_table, lambda_table, express_table } = require('./functions.js');

var func_auto_table = [];

const files = fs.readdirSync("api/controllers");
for( var i = 0 ; i < files.length ; i++ ){
    var stats_dir = fs.statSync("api/controllers/" + files[i]);
    if( !stats_dir.isDirectory() )
        continue;
    try{
        fs.statSync("api/controllers/" + files[i] + "/swagger.yaml" );
    }catch(error){
        continue;
    }
    func_auto_table[files[i]] = require('./' + files[i]).handler;
    console.log('auto mouted: ' + files[i]);
};

var exports_list = {};
for( var operationId in func_table ){
    exports_list[operationId] = routing;
}
for( var operationId in func_auto_table ){
    exports_list[operationId] = routing;
}
for( var operationId in alexa_table ){
    exports_list[operationId] = routing;
}
for( var operationId in lambda_table ){
    exports_list[operationId] = routing;
}
for( var operationId in express_table ){
    exports_list[operationId] = express_table[operationId];
}

module.exports = exports_list;

function routing(req, res) {
//    console.log(req);

    var operationId = req.swagger.operation.operationId;
    console.log('[' + req.path + ' calling]');

    try{
        var event;
        var func;
        if( func_table.hasOwnProperty(operationId) || func_auto_table.hasOwnProperty(operationId) ){
            event = {
                headers: req.headers,
                body: JSON.stringify(req.body),
                path: req.path,
                httpMethod: req.method,
                queryStringParameters: req.query,
                stage: req.swagger.swaggerObject.basePath.replace( /^\/|\/$/g, ""),
                Host: req.hostname,
                requestContext: ( req.requestContext ) ? req.requestContext : {},
                files: req.files,
            };

            if( req.swagger.operation["x-automount"] )
                func = func_auto_table[operationId];
            else
            func = func_table[operationId];
            res.func_type = "normal";
        }else if( alexa_table.hasOwnProperty(operationId) ){
            event = req.body;

            func = alexa_table[operationId];
            res.func_type = "alexa";
        }else if( lambda_table.hasOwnProperty(operationId) ){
            event = req.body.event;

            func = lambda_table[operationId];
            res.func_type = "lambda";
        }else{
            console.log('can not found operationId: ' + operationId);
            return_error(res, new Error('can not found operationId'));
            return;
        }
        res.returned = false;

//        console.log(event);

        var context = {
            succeed: (msg) => {
                console.log('succeed called');
                return_response(res, msg);
            },
            fail: (error) => {
                console.log('failed called');
                return_error(res, error);
            },
            req: req,
            swagger: req.swagger
        };

        var task = func(event, context, (error, response) =>{
            console.log('callback called');
            if( error )
                return_error(res, error);
            else
                return_response(res, response);
        });
        if( task instanceof Promise || (task && typeof task.then === 'function') ){
            task.then(ret =>{
                if( ret ){
                    console.log('promise is called');
                    return_response(res, ret);
                }else{
                    console.log('promise return undefined');
                    return_none(res);
                }
            })
            .catch(err =>{
                console.log('error throwed: ' + err);
                return_error(res, err);
            });
        }else{
            console.log('return called');
//            return_none(res);
        }
    }catch(err){
        console.log('error throwed: ' + err);
        return_error(res, err);
    }
}

function return_none(res){
    if( res.returned )
        return;
    else
        res.returned = true;

    res.statusCode = 200;
    res.type('application/json');

    if( res.func_type == 'alexa' ){
        res.json({});
    }else if(res.func_type == 'lambda'){
        res.json({ body: null });
    }else{
        res.json({});
    }
}

function return_error(res, err){
    if( res.returned )
        return;
    else
        res.returned = true;

    res.status(500);
    res.json({ errorMessage: err.toString() });
}

function return_response(res, ret){
    if( res.returned )
        return;
    else
        res.returned = true;

    if( ret.statusCode )
        res.status(ret.statusCode);
    for( var key in ret.headers )
        res.set(key, ret.headers[key]);

//    console.log(ret.body);

    if (!res.get('Content-Type'))
        res.type('application/json');

    if( ret.isBase64Encoded ){
        var bin = new Buffer(ret.body, 'base64')
        res.send(bin);
    }else{
        if( res.func_type == 'alexa'){
            res.json(ret);
        }else if( res.func_type == 'lambda'){
            res.json({ body: ret });
        }else{
            if( ret.body || ret.body == '' )
                res.send(ret.body);
            else
                res.json({});
        }
    }
}
