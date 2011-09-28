#include "geometry.hpp"

Geometry::Geometry() {
}

Geometry::Geometry(geos::geom::Geometry* geom) : ObjectWrap() {
    _geom = geom;
}
Geometry::~Geometry()
{
}

Persistent<FunctionTemplate> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target)
{
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Geometry::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Geometry"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "toString", Geometry::ToString);

    target->Set(String::NewSymbol("Geometry"), constructor->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry* geometry) {
    HandleScope scope;
    Geometry *geom = new Geometry(geometry);
    Handle<Value> ext = External::New(geom);
    Handle<Object> obj = constructor->GetFunction()->NewInstance(1, &ext);
    geom->Wrap(obj);
    return scope.Close(obj);
}

Handle<Value> Geometry::New(const Arguments& args) {
    Geometry *geom;
    HandleScope scope;
    geom = new Geometry();
    geom->Wrap(args.This());
    return args.This();
}

Handle<Value> Geometry::ToString(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    return scope.Close(String::New(geom->_geom->toString().data()));
}
