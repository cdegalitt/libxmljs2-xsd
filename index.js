/**
 * Node.js bindings for XSD validation from libxml
 * @module libxml-xsd
 */

var fs = require('fs');
var libxmljs = require('libxmljs2');
var binding = require('bindings')('node-libxml-xsd');

/**
 * The libxmljs module. Prevents the need for a user's code to require it a second time. Also prevent weird bugs.
 */
exports.libxmljs = libxmljs;

/**
 * A compiled schema. Do not call this constructor, instead use parse or parseFile.
 *
 * store both the source document and the parsed schema
 * if we don't store the schema doc it will be deleted by garbage collector and it will result in segfaults.
 *
 * @constructor
 * @param {Document} schemaDoc - XML document source of the schema
 * @param {Document} schemaObj - Simple wrapper of a XSD schema
 */
var Schema = function(schemaDoc, schemaObj){
	this.schemaDoc = schemaDoc;
	this.schemaObj = schemaObj;
};

/**
 * Parse a XSD schema
 *
 * @param {string|Document} source - The content of the schema as a string or a [libxmljs document]{@link https://github.com/polotek/libxmljs/wiki/Document}
 * @returns The parsed Schema
 */
exports.parse = function(source) {
	// schema can be given as a string or a pre-parsed xml document
	if (typeof source === 'string') {
		source = libxmljs.parseXml(source);
	}
	return new Schema(source, binding.schemaSync(source));
};

/**
 * Parse a XSD schema
 *
 * @param {stringPath} sourcePath - The path of the file
 * @returns The parsed Schema
 */
exports.parseFile = function(sourcePath) {
	var data = fs.readFileSync(sourcePath, 'utf8');
	return exports.parse(data.toString());
};

/**
 * Validate a XML document over a schema
 *
 * @param {string|Document} source - The XML content to validate with the schema, to be given as a string or a [libxmljs document]{@link https://github.com/polotek/libxmljs/wiki/Document}
 * @return An array of validation errors, or null if document is valid
 */
Schema.prototype.validate = function(source) {

	// xml can be given as a string or a pre-parsed xml document
	if (typeof source === 'string') {
		source = libxmljs.parseXml(source);
	}

	var validationErrors = binding.validateSync(this.schemaObj, source);
	return validationErrors.length > 0 ? validationErrors : null;
};

/**
 * Apply a schema to a XML file
 *
 * @param {string} sourcePath - The path of the file to read
 * @return An array of validation errors, or null if document is valid
 */
Schema.prototype.validateFile = function(sourcePath) {
	var data = fs.readFileSync(sourcePath, 'utf8');
	return this.validate(data.toString());
};
