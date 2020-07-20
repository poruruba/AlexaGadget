'use strict';

const yaml = require('yaml');
const yaml_types = require('yaml/types');
const fs = require('fs');

const SWAGGER_FILE = "api/swagger/swagger.yaml";
const TARGET_FNAME = "swagger.yaml";
const ROUTING_NAME = "routing";

const root_file = fs.readFileSync(SWAGGER_FILE, 'utf-8');
const root = yaml.parseDocument(root_file);

if( process.argv.length != 3){
  console.error('Invalid Params');
  return;
}

if( process.argv[2] == 'clean'){
  var num = 0;
  
  num += delete_paths(root);
  num += delete_definitions(root);

  if( num == 0 ){
    console.log(SWAGGER_FILE + ' no changed');
    return;
  }

  var swagger = String(root);
  fs.writeFileSync(SWAGGER_FILE, swagger, 'utf-8');
  console.log(SWAGGER_FILE + ' cleaned');
}else if( process.argv[2] == 'mount'){
  var num = 0;

  num += delete_paths(root);
  num += delete_definitions(root);

  const files = fs.readdirSync("api/controllers");
  for( var i = 0 ; i < files.length ; i++ ){
    var stats_dir = fs.statSync("api/controllers/" + files[i]);
    if( !stats_dir.isDirectory() )
      continue;
    try{
      fs.statSync("api/controllers/" + files[i] + '/' + TARGET_FNAME );
    }catch(error){
      continue;
    }

    const file = fs.readFileSync("api/controllers/" + files[i] + '/' + TARGET_FNAME, 'utf-8');
    const doc = yaml.parseDocument(file);
    num += append_paths(root, doc, files[i]);
    num += append_definitions(root, doc, files[i]);
  };

  if( num == 0 ){
    console.log(SWAGGER_FILE + ' no changed');
    return;
  }

  var swagger = String(root);
  fs.writeFileSync(SWAGGER_FILE, swagger, 'utf-8');
  console.log(SWAGGER_FILE + ' mounted');
}else{
  console.error('Invalid Params');
  return;
}

return;


function append_paths(root, target, name){
  var map = target.get('paths');
  if( !map )
    return 0;

  var num = 0;
  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.items[0].value instanceof yaml_types.Scalar )
      continue;

    var children = map.items[i].value.items;
    for( var j = 0 ; j < children.length ; j++ ){
      children[j].value.set("x-swagger-router-controller", ROUTING_NAME);
      children[j].value.set("operationId", name);
      children[j].value.set("x-automount", name);
    }

    map.items[i].comment = " automounted";
    root.addIn(['paths'], map.items[i]);
    console.log('mounted(paths): ' + map.items[i].key.value);
    num++;
  }

  return num;
}

function append_definitions(root, target, name){
  var map = target.get('definitions');
  if( !map )
    return 0;

  for( var i = 0 ; i < map.items.length ; i++ ){
    map.items[i].value.set("x-automount", name);

    map.items[i].comment = " automounted";
    root.addIn(['definitions'], map.items[i]);
    console.log('mounted(definition): ' + map.items[i].key.value);
  }

  return map.items.length;
}

function delete_paths(root){
  var map = root.get('paths');
  if( !map )
    return 0;

  var delete_target = [];

  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.items[0].value instanceof yaml_types.Scalar )
      continue;

    var children = map.items[i].value.items;
    for( var j = 0 ; j < children.length ; j++ ){
      if( children[j].value.get("x-automount") ){
        delete_target.push(map.items[i].key.value);
        break;
      }
    }
  }

  for( var i = 0 ; i < delete_target.length ; i++ )
    map.delete(delete_target[i]);
  
  return delete_target.length;
}

function delete_definitions(root){
  var map = root.get('definitions');
  if( !map )
    return 0;

  var delete_target = [];
  for( var i = 0 ; i < map.items.length ; i++ ){
    if( map.items[i].value.get("x-automount") )
      delete_target.push(map.items[i].key.value);
  }

  for( var i = 0 ; i < delete_target.length ; i++ )
    map.delete(delete_target[i]);

  return delete_target.length;
}
