#include "./xml_errors.h"

#include <node.h>
#include <nan.h>

using namespace v8;

void set_string_field(Local<Object> obj, const char *name, const char *value) {
  Nan::HandleScope scope;
  if (!value) {
    return;
  }
  Nan::Set(obj, Nan::New<String>(name).ToLocalChecked(),
           Nan::New<String>(value, strlen(value)).ToLocalChecked());
}

void set_numeric_field(Local<Object> obj, const char *name, const int value) {
  Nan::HandleScope scope;
  Nan::Set(obj, Nan::New<String>(name).ToLocalChecked(),
           Nan::New<Int32>(value));
}

// Code copied from libxmljs. Should be kept in sync. Or dynamically linked to libxml node extension.
Local<Value> BuildSyntaxError(xmlError *error) {
  Nan::EscapableHandleScope scope;

  Local<Value> err =
      Exception::Error(Nan::New<String>(error->message).ToLocalChecked());
  Local<Object> out = Local<Object>::Cast(err);

  set_numeric_field(out, "domain", error->domain);
  set_numeric_field(out, "code", error->code);
  set_string_field(out, "message", error->message);
  set_numeric_field(out, "level", error->level);
  set_numeric_field(out, "column", error->int2);
  set_string_field(out, "file", error->file);
  set_numeric_field(out, "line", error->line);
  set_string_field(out, "str1", error->str1);
  set_string_field(out, "str2", error->str2);
  set_string_field(out, "str3", error->str3);

  // only add if we have something interesting
  if (error->int1) {
    set_numeric_field(out, "int1", error->int1);
  }

  return scope.Escape(err);
}
