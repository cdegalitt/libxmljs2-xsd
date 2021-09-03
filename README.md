libxmljs2-xsd
===============

[![NPM version](https://badge.fury.io/js/libxmljs2-xsd.svg)](http://badge.fury.io/js/libxmljs2-xsd)

*XSD 1.0 validation for node.js 14.x and 16.x using [libxml](http://xmlsoft.org/) via [libxmljs2](https://github.com/marudor/libxmljs2)*  
As of now, XSD 1.1 is not supported, and the author does not actively work on it. Feel free to submit a PR if you want to.

Installation
------------

	npm install libxmljs2-xsd

On windows there are several prerequisites that need to be met before an install will work.
[Windows-Build-Tools](https://github.com/felixrieseberg/windows-build-tools) is a great helper module for this task.

    npm install --global --production windows-build-tools
    npm install libxmljs2-xsd

Basic usage
-----------

```js
var xsd = require('libxmljs2-xsd');
  // throws in case of error
  var schema = xsd.parseFile(schemaPath);
  // throws in case of technical error, returns a list of validation errors, 
  //   or null if the document is valid
  var validationErrors = schema.validate(documentString);
});
```

libxmljs2 integration
--------------------

libxmljs2-xsd depends on [libxmljs2](https://github.com/marudor/libxmljs2) that bundles libxml.

The libxmljs2 module required by node-libxml-xsd is exposed as ```require('libxmljs2-xsd').libxmljs```. This prevents depending on libxmljs2 twice which is not optimal and source of weird bugs.

It is possible to work with libxmljs2 documents instead of strings as inputs to the *parse()* and *validate()* functions.

Imports and includes
--------------------

XSD includes are supported but relative paths must be given from the execution directory, usually the root of the project.

Includes are resolved when parsing the schema. Therefore the parsing task becomes IO bound, which is why you should not use synchronous parsing when you expect some includes.


Environment compatibility
-------------------------

For now 64bits linux and 32bits windows are confirmed. Other environments are probably ok, but not checked. Please report an issue if you encounter some difficulties.

node-libxml-xsd depends on [node-gyp](https://github.com/TooTallNate/node-gyp), you will need to meet its requirements. This can be a bit painful mostly for windows users. The node-gyp version bundled in your npm will have to be greater than 0.13.0, so you might have to follow [these instructions to upgrade](https://github.com/TooTallNate/node-gyp/wiki/Updating-npm's-bundled-node-gyp). There is no system dependancy otherwise as libxml is bundled by libxmljs2.

API Reference
=============
<a name="module_libxml-xsd"></a>

## libxml-xsd
Node.js bindings for XSD validation from libxml


* [libxml-xsd](#module_libxml-xsd)
    * _static_
        * [.libxmljs](#module_libxml-xsd.libxmljs)
        * [.parse(source, options)](#module_libxml-xsd.parse) ⇒
        * [.parseFile(sourcePath, options)](#module_libxml-xsd.parseFile) ⇒
    * _inner_
        * [~Schema](#module_libxml-xsd..Schema)
            * [new Schema(schemaDoc, schemaObj)](#new_module_libxml-xsd..Schema_new)
            * [.validate(source)](#module_libxml-xsd..Schema+validate) ⇒
            * [.validateFile(sourcePath)](#module_libxml-xsd..Schema+validateFile) ⇒

<a name="module_libxml-xsd.libxmljs"></a>

### libxml-xsd.libxmljs
The libxmljs module. Prevents the need for a user's code to require it a second time. Also prevent weird bugs.

**Kind**: static property of [<code>libxml-xsd</code>](#module_libxml-xsd)  
<a name="module_libxml-xsd.parse"></a>

### libxml-xsd.parse(source, options) ⇒
Parse a XSD schema

**Kind**: static method of [<code>libxml-xsd</code>](#module_libxml-xsd)  
**Returns**: The parsed Schema  

| Param | Type | Description |
| --- | --- | --- |
| source | <code>string</code> \| <code>Document</code> | The content of the schema as a string or a [libxmljs document](https://github.com/polotek/libxmljs/wiki/Document) |
| options | <code>object</code> | [The parsing options](https://github.com/marudor/libxmljs2/blob/a61fdccf87a2928c084a3ec3e2a44f163998d81b/index.d.ts#L11) |

<a name="module_libxml-xsd.parseFile"></a>

### libxml-xsd.parseFile(sourcePath, options) ⇒
Parse a XSD schema

**Kind**: static method of [<code>libxml-xsd</code>](#module_libxml-xsd)  
**Returns**: The parsed Schema  

| Param | Type | Description |
| --- | --- | --- |
| sourcePath | <code>stringPath</code> | The path of the file |
| options | <code>object</code> | [The parsing options](https://github.com/marudor/libxmljs2/blob/a61fdccf87a2928c084a3ec3e2a44f163998d81b/index.d.ts#L11) |

<a name="module_libxml-xsd..Schema"></a>

### libxml-xsd~Schema
**Kind**: inner class of [<code>libxml-xsd</code>](#module_libxml-xsd)  

* [~Schema](#module_libxml-xsd..Schema)
    * [new Schema(schemaDoc, schemaObj)](#new_module_libxml-xsd..Schema_new)
    * [.validate(source)](#module_libxml-xsd..Schema+validate) ⇒
    * [.validateFile(sourcePath)](#module_libxml-xsd..Schema+validateFile) ⇒

<a name="new_module_libxml-xsd..Schema_new"></a>

#### new Schema(schemaDoc, schemaObj)
A compiled schema. Do not call this constructor, instead use parse or parseFile.

store both the source document and the parsed schema
if we don't store the schema doc it will be deleted by garbage collector and it will result in segfaults.


| Param | Type | Description |
| --- | --- | --- |
| schemaDoc | <code>Document</code> | XML document source of the schema |
| schemaObj | <code>Document</code> | Simple wrapper of a XSD schema |

<a name="module_libxml-xsd..Schema+validate"></a>

#### schema.validate(source) ⇒
Validate a XML document over a schema

**Kind**: instance method of [<code>Schema</code>](#module_libxml-xsd..Schema)  
**Returns**: An array of validation errors, or null if document is valid  

| Param | Type | Description |
| --- | --- | --- |
| source | <code>string</code> \| <code>Document</code> | The XML content to validate with the schema, to be given as a string or a [libxmljs document](https://github.com/polotek/libxmljs/wiki/Document) |

<a name="module_libxml-xsd..Schema+validateFile"></a>

#### schema.validateFile(sourcePath) ⇒
Apply a schema to a XML file

**Kind**: instance method of [<code>Schema</code>](#module_libxml-xsd..Schema)  
**Returns**: An array of validation errors, or null if document is valid  

| Param | Type | Description |
| --- | --- | --- |
| sourcePath | <code>string</code> | The path of the file to read |


*documented by [jsdoc-to-markdown](https://github.com/75lb/jsdoc-to-markdown)*.
