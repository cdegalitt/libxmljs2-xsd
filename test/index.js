var fs = require('fs');

var should = require('should');

var xsd = require('../index');
var libxmljs = xsd.libxmljs;

describe('node-libxml-xsd', function() {
	var schemaSource;
	before(function(callback) {
		fs.readFile('./test/resources/chapter04ord1.xsd', 'utf8', function(err, data) {
			schemaSource = data;
			callback(err);
		});
	});
	var docSource;
	before(function(callback) {
		fs.readFile('./test/resources/chapter04.xml', 'utf8', function(err, data) {
			docSource = data;
			callback(err);
		});
	});

	var schema;
	describe('synchronous parse function', function() {
		it('should parse a schema from a libxmljs xml document', function() {
			var schemaDoc = libxmljs.parseXml(schemaSource);
			schema = xsd.parse(schemaDoc);
			schema.should.be.type('object');
		});
		it('should parse a schema from a xml string', function() {
			schema = xsd.parse(schemaSource);
			schema.should.be.type('object');
		});
		it('should throw an error when parsing invalid schema', function() {
			(function() {
				xsd.parse('this is not a schema!');
			}).should.throw();
		});
	});

	describe('parseFile function', function() {
		it('should parse a schema from a file', function() {
			schema = xsd.parseFile('./test/resources/chapter04ord1.xsd');
			(schema).should.be.type('object');
		});
	});

	describe('synchronous validate function', function() {
		it('should validate a schema to a libxmljs xml document', function() {
			var doc = libxmljs.parseXml(docSource);
			var validationErrors = schema.validate(doc);
			should.not.exist(validationErrors);
		});
		it('should validate a schema to a xml string', function() {
			var validationErrors = schema.validate(docSource);
			should.not.exist(validationErrors);
		});
		it('should throw an error if given a bad xml', function() {
			(function() {
				schema.validate('this is not valid!');
			}).should.throw();
		});
		it('should return validation errors if content is good xml but not valid', function() {
			var validationErrors = schema.validate('<test>this is not valid!</test>');
			validationErrors.should.be.type('object');
			validationErrors.should.have.lengthOf(1);
		});
	});

	describe('validateFile function', function() {
		it('should validate a schema to a xml file', function() {
			var validationErrors = schema.validateFile('./test/resources/chapter04.xml');
			should.not.exist(validationErrors);
		});
	});

});
