// Very simple v8 wrapper for xslt stylesheet, see "Wrapping C++ objects" section here http://nodejs.org/api/addons.html

#ifndef SRC_SCHEMA_H_
#define SRC_SCHEMA_H_

#include <libxml/xmlschemas.h>

class Schema : public Nan::ObjectWrap {
	public:
	    static void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module, v8::Local<v8::Context> context);
	    static v8::Local<v8::Object> New(v8::Local<v8::Context> context, xmlSchemaPtr schema);
	    xmlSchemaPtr schema_obj;

	private:
	    explicit Schema(v8::Local<v8::Context> context, xmlSchemaPtr schema);
	    ~Schema();
	    static Nan::Persistent<v8::Function> constructor;
};

#endif  // SRC_SCHEMA_H_