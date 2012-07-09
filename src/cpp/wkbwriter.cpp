#include "wkbwriter.hpp"

WKBWriter::WKBWriter() {
    _writer = new geos::io::WKBWriter();
}

WKBWriter::~WKBWriter() {}

Persistent<FunctionTemplate> WKBWriter::constructor;

void WKBWriter::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(WKBWriter::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("WKBWriter"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "writeHEX", WKBWriter::WriteHEX);

    target->Set(String::NewSymbol("WKBWriter"), constructor->GetFunction());
}

Handle<Value> WKBWriter::New(const Arguments& args) {
    HandleScope scope;
    WKBWriter* writer = new WKBWriter();
    writer->Wrap(args.This());
    return args.This();
}

Handle<Value> WKBWriter::WriteHEX(const Arguments& args) {
    HandleScope scope;
    WKBWriter *writer = ObjectWrap::Unwrap<WKBWriter>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    //TODO catch exception?
    std::stringstream ss;
    writer->_writer->writeHEX(*geom->_geom, ss);
    return scope.Close(String::New(ss.str().c_str()));
}
