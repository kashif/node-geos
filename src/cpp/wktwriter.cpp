#include "wktwriter.hpp"

WKTWriter::WKTWriter() {
    _writer = new geos::io::WKTWriter();
}

WKTWriter::~WKTWriter() {}

Persistent<Function> WKTWriter::constructor;

void WKTWriter::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, WKTWriter::New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "WKTWriter"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "write", WKTWriter::Write);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setRoundingPrecision", WKTWriter::SetRoundingPrecision);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTrim", WKTWriter::SetTrim);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "WKTWriter"), tpl->GetFunction());
}

void WKTWriter::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKTWriter* writer = new WKTWriter();
    writer->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void WKTWriter::Write(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    //TODO catch exception?
    std::string str = writer->_writer->write(geom->_geom);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.data()));
}

void WKTWriter::SetRoundingPrecision(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    writer->_writer->setRoundingPrecision(args[0]->Int32Value());
    args.GetReturnValue().Set(Undefined(isolate));
}

void WKTWriter::SetTrim(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKTWriter *writer = ObjectWrap::Unwrap<WKTWriter>(args.This());
    writer->_writer->setTrim(args[0]->BooleanValue());
    args.GetReturnValue().Set(Undefined(isolate));
}
