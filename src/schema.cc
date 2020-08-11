#include <node.h>
#include <nan.h>

#include "./schema.h"

using namespace v8;

Nan::Persistent<Function> Schema::constructor;

Schema::Schema(v8::Local<v8::Context> context, xmlSchemaPtr schemaPtr) : schema_obj(schemaPtr) {}

Schema::~Schema()
{
    // TODO, potential memory leak here ?
    // We can't free the schema as the xml doc inside was probably
    // already deleted by garbage collector and this results in segfaults
}

void Schema::Init(Local<Object> exports, Local<Object> module, Local<Context> context) {
	 // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>();
    tpl->SetClassName(Nan::New<String>("Schema").ToLocalChecked());
  	tpl->InstanceTemplate()->SetInternalFieldCount(1);
  	
    constructor.Reset(tpl->GetFunction(context).ToLocalChecked());
}

// not called from node, private api
Local<Object> Schema::New(Local<Context> context, xmlSchemaPtr schemaPtr) {
    Nan::EscapableHandleScope scope;

    Local<Object> wrapper = Nan::New(constructor)->NewInstance(context).ToLocalChecked();
    Schema* schema = new Schema(context, schemaPtr);
    schema->Wrap(wrapper);
    return scope.Escape(wrapper);
}
