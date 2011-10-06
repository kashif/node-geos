#include "quadtree.hpp"

Quadtree::Quadtree() {
    _quadtree = new geos::index::quadtree::Quadtree();
}

Quadtree::~Quadtree() {}

Persistent<FunctionTemplate> Quadtree::constructor;

void Quadtree::Initialize(Handle<Object> target) {
    HandleScope scope;

    Quadtree::constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Quadtree::New));
    Quadtree::constructor->InstanceTemplate()->SetInternalFieldCount(1);
    Quadtree::constructor->SetClassName(String::NewSymbol("Quadtree"));

    NODE_SET_PROTOTYPE_METHOD(Quadtree::constructor, "insert", Insert);
    NODE_SET_PROTOTYPE_METHOD(Quadtree::constructor, "queryAll", QueryAll);
    NODE_SET_PROTOTYPE_METHOD(Quadtree::constructor, "remove", Remove);
    NODE_SET_PROTOTYPE_METHOD(Quadtree::constructor, "query", Query);

    target->Set(String::NewSymbol("Quadtree"), Quadtree::constructor->GetFunction());

}

Handle<Value> Quadtree::New(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = new Quadtree();
    quadtree->Wrap(args.This());
    return args.This();
}



Handle<Value> Quadtree::Insert(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    quadtree->_quadtree->insert(geom->_geom->getEnvelopeInternal(), (int32_t *) args[1]->Int32Value());
    return Undefined();
}

Handle<Value> Quadtree::Remove(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    bool r = quadtree->_quadtree->remove(geom->_geom->getEnvelopeInternal(), (int32_t *) args[1]->Int32Value());
    return r ? True() : False();
}

Handle<Value> Quadtree::Query(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    std::vector<int32_t> v;
    quadtree->_quadtree->query(geom->_geom->getEnvelopeInternal(),(std::vector<void*>&) v);
    Local<Array> result = Array::New(v.size());
    int i = 0;
    for (std::vector<int32_t>::iterator it = v.begin(); it != v.end(); ++it) {
        result->Set(i,Integer::New(*it));
        i++;
    }
    return scope.Close(result);
}

Handle<Value> Quadtree::QueryAll(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    std::vector<int32_t> *v = (std::vector<int32_t> *) quadtree->_quadtree->queryAll();
    Local<Array> result = Array::New(v->size());
    int i = 0;
    for (std::vector<int32_t>::iterator it = v->begin(); it != v->end(); ++it) {
        result->Set(i,Integer::New(*it));
        i++;
    }
    return scope.Close(result);
}
