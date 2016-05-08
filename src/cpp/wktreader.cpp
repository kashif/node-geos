#include "wktreader.hpp"

WKTReader::WKTReader() {
    _reader = new geos::io::WKTReader();
}

WKTReader::WKTReader(const geos::geom::GeometryFactory *gf) {
    _reader = new geos::io::WKTReader(gf);
}

WKTReader::~WKTReader() {}

Persistent<Function> WKTReader::constructor;

void WKTReader::Initialize(Handle<Object> target)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, WKTReader::New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "WKTReader"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "read", WKTReader::Read);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "WKTReader"), tpl->GetFunction());
}

void WKTReader::New(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);;

    WKTReader *wktReader;

    if(args.Length() == 1) {
        GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args[0]->ToObject());
        wktReader = new WKTReader(factory->_factory);
    } else {
        wktReader = new WKTReader();
    }

    wktReader->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void WKTReader::Read(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);;

    WKTReader* reader = ObjectWrap::Unwrap<WKTReader>(args.This());
    try {
        geos::geom::Geometry* geom = reader->_reader->read(*String::Utf8Value(args[0]->ToString()));
        args.GetReturnValue().Set(Geometry::New(geom));
    } catch (geos::io::ParseException e) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, e.what())));
    } catch (geos::util::GEOSException e) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, e.what())));
    } catch (...) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Exception while reading WKT.")));
    }
}
