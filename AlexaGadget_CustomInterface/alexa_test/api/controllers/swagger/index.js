'use strict';

const yaml = require('js-yaml');
const fs   = require('fs');

const HELPER_BASE = process.env.HELPER_BASE || '../../helpers/';
const Response = require(HELPER_BASE + 'response');
const Redirect = require(HELPER_BASE + 'redirect');

const SWAGGERFILE_BASE = process.env.SWAGGERFILE_BASE || './api/';

exports.handler = async (event, context, callback) => {
  var doc = yaml.safeLoad(fs.readFileSync(SWAGGERFILE_BASE + 'swagger/swagger.yaml', 'utf8'));

  return new Response(doc);
};
