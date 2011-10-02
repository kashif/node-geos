#include "quadtree.hpp"

Quadtree::Quadtree() {
    _quadtree = new geos::index::quadtree::Quadtree();
}
Quadtree::~Quadtree() {}

Persistent<FunctionTemplate> Quadtree::constructor;

void Quadtree::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Quadtree::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Quadtree"));

    target->Set(String::NewSymbol("Quadtree"), constructor->GetFunction());
}

Handle<Value> Quadtree::New(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = new Quadtree();
    quadtree->Wrap(args.This());
    return args.This();
}
