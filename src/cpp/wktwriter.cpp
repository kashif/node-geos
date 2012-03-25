#include "wktwriter.hpp"

WKTWriter::WKTWriter() {
    _writer = new geos::io::WKTWriter();
}

WKTWriter::~WKTWriter() {}

Persistent<FunctionTemplate> WKTWriter::constructor;

void WKTWriter::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(WKTWriter::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("WKTWriter"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "write", WKTWriter::Write);
    NODE_SET_PROTOTYPE_METHOD(constructor, "setRoundingPrecision", WKTWriter::SetRoundingPrecision);
    NODE_SET_PROTOTYPE_METHOD(constructor, "setTrim", WKTWriter::SetTrim);

    target->Set(String::NewSymbol("WKTWriter"), constructor->GetFunction());
}

Handle<Value> WKTWriter::New(const Arguments& args) {
    HandleScope scope;
    WKTWriter* writer = new WKTWriter();
    writer->Wrap(args.This());
    return args.This();
}

Handle<Value> WKTWriter::Write(const Arguments& args) {
    HandleScope scope;
    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    //TODO catch exception?
    std::string str = writer->_writer->write(geom->_geom);
    return scope.Close(String::New(str.data()));
}

Handle<Value> WKTWriter::SetRoundingPrecision(const Arguments& args) {
    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    writer->_writer->setRoundingPrecision(args[0]->Int32Value());
    return Undefined();
}

Handle<Value> WKTWriter::SetTrim(const Arguments& args) {
    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    writer->_writer->setTrim(args[0]->BooleanValue());
    return Undefined();
}
