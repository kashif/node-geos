#include "geometryfactory.hpp"

GeometryFactory::GeometryFactory() {
    _factory = new geos::geom::GeometryFactory();
}

GeometryFactory::~GeometryFactory() {}

Persistent<FunctionTemplate> GeometryFactory::constructor;

void GeometryFactory::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(GeometryFactory::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("GeometryFactory"));


    target->Set(String::NewSymbol("GeometryFactory"), constructor->GetFunction());
}

Handle<Value> GeometryFactory::New(const Arguments& args) {
    HandleScope scope;
    GeometryFactory* factory = new GeometryFactory();
    factory->Wrap(args.This());
    return args.This();
}
