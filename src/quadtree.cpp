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

    NODE_SET_PROTOTYPE_METHOD(Quadtree::constructor, "size", Size);

    target->Set(String::NewSymbol("Quadtree"), Quadtree::constructor->GetFunction());

}

Handle<Value> Quadtree::New(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = new Quadtree();
    quadtree->Wrap(args.This());
    return args.This();
}

Handle<Value> Quadtree::Size(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    return scope.Close(Integer::New(quadtree->_quadtree->size()));
}

//TODO add async
Handle<Value> Quadtree::Insert(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    quadtree->_quadtree->insert(geom->_geom->getEnvelopeInternal(), (int32_t *) args[1]->Int32Value());
    return Undefined();
}

//TODO add async
Handle<Value> Quadtree::Remove(const Arguments& args) {
    HandleScope scope; //do we need the scope? don't think so
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    bool r = quadtree->_quadtree->remove(geom->_geom->getEnvelopeInternal(), (int32_t *) args[1]->Int32Value());
    return r ? True() : False();
}

typedef struct {
    Geometry *geom;
    Quadtree *quadtree;
    Persistent<Function> cb;
    std::vector<int32_t> result;
} query_baton_t;

int Quadtree::EIO_Query(eio_req *req) {
    query_baton_t *closure = static_cast<query_baton_t *>(req->data);
    closure->quadtree->_quadtree->query(closure->geom->_geom->getEnvelopeInternal(), (std::vector<void*>&) closure->result);
    return 0;
}

int Quadtree::EIO_AfterQuery(eio_req *req) {
    query_baton_t *closure = static_cast<query_baton_t *>(req->data);

    HandleScope scope;
    Local<Array> result = Array::New(closure->result.size());
    int i = 0;
    for (std::vector<int32_t>::iterator it = closure->result.begin(); it != closure->result.end(); ++it) {
        result->Set(i,Integer::New(*it));
        i++;
    }

    Local<Value> argv[2] = { Local<Value>::New(Null()), Local<Value>::New(result) };

    TryCatch tryCatch;

    closure->cb->Call(Context::GetCurrent()->Global(),2, argv);
    if(tryCatch.HasCaught()) {
        FatalException(tryCatch);
    }

    closure->cb.Dispose();
    //closure->geom->Unref();
    ev_unref(EV_DEFAULT_UC);
    closure->quadtree->Unref();

    delete closure;

    return 0;
}

Handle<Value> Quadtree::Query(const Arguments& args) {
    HandleScope scope;
    Quadtree *quadtree = ObjectWrap::Unwrap<Quadtree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    if (args.Length() == 2) {
        //async call
        query_baton_t *closure = new query_baton_t();
        closure->quadtree = quadtree;
        closure->geom = geom;
        closure->cb = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
        eio_custom(EIO_Query, EIO_PRI_DEFAULT, EIO_AfterQuery, closure);

        ev_ref(EV_DEFAULT_UC);

        //geom->Ref(); //?
        quadtree->Ref();

        return Undefined();

    } else {
        //sync call
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
}
//TODO add async
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
