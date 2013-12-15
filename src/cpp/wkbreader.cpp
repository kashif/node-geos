#include "wkbreader.hpp"
#include <sstream>

WKBReader::WKBReader() {
    _reader = new geos::io::WKBReader();
}

WKBReader::WKBReader(const geos::geom::GeometryFactory *gf) {
    _reader = new geos::io::WKBReader(*gf);
}

WKBReader::~WKBReader() {}

Persistent<FunctionTemplate> WKBReader::constructor;

void WKBReader::Initialize(Handle<Object> target)
{
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(WKBReader::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("WKBReader"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "read", WKBReader::Read);

    target->Set(String::NewSymbol("WKBReader"), constructor->GetFunction());
}

Handle<Value> WKBReader::New(const Arguments& args)
{
    HandleScope scope;
    WKBReader *wkbReader;

    if(args.Length() == 1) {
        GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args[0]->ToObject());
        wkbReader = new WKBReader(factory->_factory);
    } else {
        wkbReader = new WKBReader();
    }

    wkbReader->Wrap(args.This());
    return args.This();
}

Handle<Value> WKBReader::Read(const Arguments& args)
{
    HandleScope scope;
    WKBReader* reader = ObjectWrap::Unwrap<WKBReader>(args.This());
    v8::String::AsciiValue hex(args[0]->ToString());
    std::string  str = std::string(*hex);
    std::istringstream is( str );
    try {
        geos::geom::Geometry* geom = reader->_reader->readHEX(is);
        return scope.Close(Geometry::New(geom));
    } catch (geos::io::ParseException e) {
        return ThrowException(Exception::Error(String::New(e.what())));
    } catch (geos::util::GEOSException e) {
        return ThrowException(Exception::Error(String::New(e.what())));
    } catch (...) {
        return ThrowException(Exception::Error(String::New("Exception while reading WKB.")));
    }
}
