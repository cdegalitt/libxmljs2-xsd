// Very simple v8 wrapper for xslt stylesheet, see "Wrapping C++ objects" section here http://nodejs.org/api/addons.html

#ifndef SRC_XML_ERRORS_H_
#define SRC_XML_ERRORS_H_

#include <node.h>
#include <xml_syntax_error.h> // include from libxmljs

v8::Local<v8::Value> BuildSyntaxError(xmlError *error);

#endif // SRC_XML_ERRORS_H_
