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

    NODE_SET_PROTOTYPE_METHOD(constructor, "insert", Insert);
    NODE_SET_PROTOTYPE_METHOD(constructor, "queryAll", QueryAll);
    NODE_SET_PROTOTYPE_METHOD(constructor, "remove", Remove);
    NODE_SET_PROTOTYPE_METHOD(constructor, "query", Query);

    target->Set(String::NewSymbol("Quadtree"), constructor->GetFunction());
}

Handle<Value> Quadtree::New(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = new Quadtree();
    quadtree->Wrap(args.This());
    return args.This();
}


struct Item {
    std::string key;
    geos::geom::Geometry *geom;
};

Handle<Value> Quadtree::Insert(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    Item *item = new Item();
    item->key = *String::Utf8Value(args[1]->ToString());
    item->geom = geom->_geom;
    quadtree->_quadtree->insert(geom->_geom->getEnvelopeInternal(), item);
    return Undefined();
}

Handle<Value> Quadtree::Remove(const Arguments& args) {
    //broken...
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    Item *item = new Item();
    item->key = *String::Utf8Value(args[1]->ToString());
    item->geom = geom->_geom;
    bool r = quadtree->_quadtree->remove(geom->_geom->getEnvelopeInternal(), item);
    delete item;
    return r ? True() : False();
}

Handle<Value> Quadtree::Query(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    std::vector<Item*> v;
    quadtree->_quadtree->query(geom->_geom->getEnvelopeInternal(), (std::vector<void*>&) v);
    Local<Array> result = Array::New(v.size());
    int i = 0;
    Local<Object> obj;
    for (std::vector<Item*>::iterator it = v.begin(); it != v.end(); ++it) {
        obj = Object::New();
        obj->Set(String::NewSymbol("key"), String::New((*it)->key.data()));
        obj->Set(String::NewSymbol("geom"), Geometry::New((*it)->geom));
        result->Set(i,obj);
        i++;
    }
    return scope.Close(result);
}

Handle<Value> Quadtree::QueryAll(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    std::vector<Item*> *v = (std::vector<Item*> *) quadtree->_quadtree->queryAll();
    Local<Array> result = Array::New(v->size());
    int i = 0;
    Local<Object> obj;
    for (std::vector<Item*>::iterator it = v->begin(); it != v->end(); ++it) {
        obj = Object::New();
        obj->Set(String::NewSymbol("key"), String::New((*it)->key.data()));
        obj->Set(String::NewSymbol("geom"), Geometry::New((*it)->geom));
        result->Set(i,obj);
        i++;
    }
    return scope.Close(result);
}
