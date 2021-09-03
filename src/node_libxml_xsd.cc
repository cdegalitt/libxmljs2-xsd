#ifndef BUILDING_NODE_EXTENSION
  #define BUILDING_NODE_EXTENSION
#endif

#include <iostream>
#include <list>
#include <node.h>
#include <nan.h>

// includes from libxmljs
#include <xml_syntax_error.h>
#include <xml_document.h>

#include "./node_libxml_xsd.h"
#include "./schema.h"
#include "./xml_errors.h"

using namespace v8;

void none(void *ctx, const char *msg, ...) {
  // do nothing
  return;
}

// Used to store a list or validation errors
void errorFunc(void* errs, xmlError* error) {
  std::vector<xmlError*>* errors = reinterpret_cast<std::vector<xmlError*>*>(errs);
  errors->push_back(error);
}

NAN_METHOD(SchemaSync) {
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  	Nan::HandleScope scope;

    libxmljs::XmlDocument* doc = Nan::ObjectWrap::Unwrap<libxmljs::XmlDocument>(info[0]->ToObject(context).ToLocalChecked());

    xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewDocParserCtxt(doc->xml_obj);
    if (parser_ctxt == NULL) {
        return Nan::ThrowError("Could not create context for schema parser");
    }
    xmlSchemaValidityErrorFunc err;
    xmlSchemaValidityWarningFunc warn;
    void* ctx;
    xmlSchemaGetParserErrors(parser_ctxt, &err, &warn, &ctx);
    xmlSchemaSetParserErrors(parser_ctxt, err, (xmlSchemaValidityWarningFunc) none, ctx);
    xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
    if (schema == NULL) {
        return Nan::ThrowError("Invalid XSD schema");
    }

    Local<Object> schemaWrapper = Schema::New(context, schema);
  	info.GetReturnValue().Set(schemaWrapper);
}

NAN_METHOD(ValidateSync) {
    Nan::HandleScope scope;
    Local<Context> context = info.GetIsolate()->GetCurrentContext();
    auto isolate = info.GetIsolate();

    // Prepare the array of errors to be filled by validation
    // Local<Array> errors = Nan::New<Array>();
    std::vector<xmlError*> errorsList;
    xmlResetLastError();
    xmlSetStructuredErrorFunc(reinterpret_cast<void *>(&errorsList), errorFunc);

    // Extract schema an document to validate from their wrappers
    Schema* schema = Nan::ObjectWrap::Unwrap<Schema>(info[0]->ToObject(context).ToLocalChecked());
    libxmljs::XmlDocument* doc = Nan::ObjectWrap::Unwrap<libxmljs::XmlDocument>(info[1]->ToObject(context).ToLocalChecked());

    // Actual validation
    xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema->schema_obj);
    if (valid_ctxt == NULL) {
        return Nan::ThrowError("Unable to create a validation context for the schema");
    }
    xmlSchemaValidateDoc(valid_ctxt, doc->xml_obj);

	  xmlSetStructuredErrorFunc(NULL, NULL);

    // Don't return the boolean result, instead return array of validation errors
    // will be empty if validation is ok
    Local<Array> errors = Array::New(isolate);
    for (unsigned int i = 0; i < errorsList.size(); i++ ) {
      errors->Set(context, i, BuildSyntaxError(errorsList.at(i)));
    }
    info.GetReturnValue().Set(errors);
    xmlSchemaFreeValidCtxt(valid_ctxt);
}

// Compose the module by assigning the methods previously prepared
void InitAll(Local<Object> exports, Local<Object> module, Local<Context> context) {
  	Schema::Init(exports, module, context);
  	exports->Set(context, Nan::New<String>("schemaSync").ToLocalChecked(), Nan::New<FunctionTemplate>(SchemaSync)->GetFunction(context).ToLocalChecked());
  	exports->Set(context, Nan::New<String>("validateSync").ToLocalChecked(), Nan::New<FunctionTemplate>(ValidateSync)->GetFunction(context).ToLocalChecked());
}

NODE_MODULE_CONTEXT_AWARE(node_libxml_xsd, InitAll);
