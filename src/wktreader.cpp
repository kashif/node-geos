#include "wktreader.hpp"

WKTReader::WKTReader() {
    _reader = new geos::io::WKTReader();
}

WKTReader::~WKTReader() {}

Persistent<FunctionTemplate> WKTReader::constructor;

void WKTReader::Initialize(Handle<Object> target)
{
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(WKTReader::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("WKTReader"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "read", WKTReader::Read);

    target->Set(String::NewSymbol("WKTReader"), constructor->GetFunction());
}

Handle<Value> WKTReader::New(const Arguments& args)
{
    HandleScope scope;
    WKTReader* wktReader = new WKTReader();
    wktReader->Wrap(args.This());
    return args.This();
}

Handle<Value> WKTReader::Read(const Arguments& args)
{
    HandleScope scope;
    WKTReader* reader = ObjectWrap::Unwrap<WKTReader>(args.This());
    try {
        geos::geom::Geometry* geom = reader->_reader->read(*String::Utf8Value(args[0]->ToString()));
        return scope.Close(Geometry::New(geom));
    } catch (geos::io::ParseException e) {
        return ThrowException(Exception::Error(String::New(e.what())));
    } catch (...) {
        return ThrowException(Exception::Error(String::New("Exception while reading WKT.")));
    }
}
