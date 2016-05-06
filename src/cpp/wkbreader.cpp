#include "wkbreader.hpp"
#include <sstream>

WKBReader::WKBReader() {
    _reader = new geos::io::WKBReader();
}

WKBReader::WKBReader(const geos::geom::GeometryFactory *gf) {
    _reader = new geos::io::WKBReader(*gf);
}

WKBReader::~WKBReader() {}

Persistent<Function> WKBReader::constructor;

void WKBReader::Initialize(Handle<Object> target)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, WKBReader::New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "WKBReader"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "readHEX", WKBReader::ReadHEX);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "WKBReader"), tpl->GetFunction());
}

void WKBReader::New(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKBReader *wkbReader;

    if(args.Length() == 1) {
        GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args[0]->ToObject());
        wkbReader = new WKBReader(factory->_factory);
    } else {
        wkbReader = new WKBReader();
    }

    wkbReader->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void WKBReader::ReadHEX(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    WKBReader* reader = ObjectWrap::Unwrap<WKBReader>(args.This());
    String::Utf8Value hex(args[0]->ToString());
    std::string str = std::string(*hex);
    std::istringstream is( str );
    try {
        geos::geom::Geometry* geom = reader->_reader->readHEX(is);
        args.GetReturnValue().Set(Geometry::New(geom));
    } catch (geos::io::ParseException e) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, e.what())));
    } catch (geos::util::GEOSException e) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, e.what())));
    } catch (...) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Exception while reading WKB.")));
    }
}
