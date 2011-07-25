#include "strTree.hpp"

StrTree::StrTree(int size) {
    geos_strTree_ = GEOSSTRtree_create(size);
}

StrTree::~StrTree() {
    GEOSSTRtree_destroy(geos_strTree_);
}

Handle<Value> StrTree::New(const Arguments& args) {
    HandleScope scope;

    StrTree* strTree;
    if(args.Length() == 0) {
        strTree = new StrTree(10);
    } else if (args[0]->Int32Value() < 2){
        return ThrowException(String::New("Tree size must be greate 1"));
    } else {
        strTree = new StrTree(args[0]->Int32Value());
    }

    strTree->Wrap(args.This());
    return args.This();
}

struct payload {
    Persistent<Function> cb;
    StrTree *strTree;
};

struct bucket {
    Local<Object> o;
};

void StrTree::QueryCallback(void *item, void *userdata) {
    HandleScope scope;
    Local<Value> argv[1];
    payload *p = (payload *) userdata;
    bucket *b = (bucket *) item;
    TryCatch try_catch;
    argv[0] = b->o;
    p->cb->Call(Context::GetCurrent()->Global(),1,argv);

    if(try_catch.HasCaught()) {
        FatalException(try_catch);
    }

    p->cb.Dispose();
    delete p;
    //delete b;
}

Handle<Value> StrTree::Query(const Arguments& args) {
    HandleScope scope;

    if(args.Length() != 2)
        return ThrowException(String::New("no geometry to query."));

    StrTree *strTree = ObjectWrap::Unwrap<StrTree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    GEOSGeometry *g = geom->GetGEOSGeometry();

    Local<Function> callback = Local<Function>::Cast(args[1]);
    Persistent<Function> cb = Persistent<Function>::New(callback);

    //this seems to be async
    //strTree->Ref();
    payload *p = new payload();
    p->cb = cb;
    p->strTree = strTree;
    GEOSSTRtree_query(strTree->geos_strTree_,g, QueryCallback, p);

    return Undefined();
}

Handle<Value> StrTree::Remove(const Arguments& args) {
    HandleScope scope;

    if(args.Length() == 0) {
        return ThrowException(String::New("no geometry to remove."));
    }

    StrTree *strTree = ObjectWrap::Unwrap<StrTree>(args.This());
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    GEOSGeometry *g = geom->GetGEOSGeometry();
    char r = GEOSSTRtree_remove(strTree->geos_strTree_,g, NULL);
    if(r) {
        return True();
    } else {
        return False();
    }
}


Handle<Value> StrTree::Insert(const Arguments& args) {
    HandleScope scope;
    if(args.Length() == 0) {
        return ThrowException(String::New("nothing to insert."));
    }

    Local<Object> o = args[1]->ToObject();
    StrTree* strTree = ObjectWrap::Unwrap<StrTree>(args.This());
    //TODO check if input is geometry object
    Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    GEOSGeometry *g = geom->GetGEOSGeometry();
    //TODO handle userdata
    bucket *b = new bucket();
    b->o = o;
    GEOSSTRtree_insert(strTree->geos_strTree_, g, b);

    return Undefined(); //return nothing and close scope?

}

static Persistent<FunctionTemplate> s_ct;

void StrTree::Initialize(Handle<Object> target) {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("StrTree"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "insert", Insert);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "remove", Remove);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "query", Query);

    target->Set(String::NewSymbol("StrTree"), s_ct->GetFunction());

}

