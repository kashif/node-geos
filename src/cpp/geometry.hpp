#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <uv.h>
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#include "binding.hpp"
#include "geojsonwriter.hpp"

#define NODE_GEOS_UNARY_PREDICATE(cppmethod, geosmethod)                                \
    typedef struct {                                                                    \
        Geometry *geom;                                                                 \
        Persistent<Function> cb;                                                        \
        bool result;                                                                    \
    } geosmethod##_baton_t;                                                             \
                                                                                        \
    void Geometry::EIO_##cppmethod(uv_work_t *req) {                                    \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        closure->result = closure->geom->_geom->geosmethod();                           \
    }                                                                                   \
                                                                                        \
    void Geometry::EIO_After##cppmethod(uv_work_t *req, int status) {                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        assert(status == 0);                                                            \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        Local<Value> argv[2] = { Null(isolate),                                         \
            closure->result ? True(isolate) : False(isolate)                            \
        };                                                                              \
        TryCatch tryCatch;                                                              \
        Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);    \
        local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);          \
                                                                                        \
        if(tryCatch.HasCaught()) {                                                      \
            FatalException(isolate, tryCatch);                                          \
        }                                                                               \
                                                                                        \
        closure->cb.Reset();                                                            \
        closure->geom->Unref();                                                         \
                                                                                        \
        delete closure;                                                                 \
        delete req;                                                                     \
    }                                                                                   \
                                                                                        \
    void Geometry::cppmethod(const FunctionCallbackInfo<Value>& args)                   \
    {                                                                                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());                     \
        if (args.Length() == 1) {                                                       \
            geosmethod##_baton_t *closure = new geosmethod##_baton_t();                 \
            closure->geom = geom;                                                       \
            closure->cb.Reset(isolate, Local<Function>::Cast(args[0]));                 \
            uv_work_t *req = new uv_work_t;                                             \
            req->data = closure;                                                        \
            uv_queue_work(uv_default_loop(), req, EIO_##cppmethod, EIO_After##cppmethod);\
            geom->Ref();                                                                \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        } else {                                                                        \
            try {                                                                       \
                args.GetReturnValue().Set(                                              \
                  geom->_geom->geosmethod() ? True(isolate) : False(isolate)            \
                );                                                                      \
                return;                                                                 \
            } catch(geos::util::GEOSException exception) {                              \
                isolate->ThrowException(                                                \
                  Exception::Error(String::NewFromUtf8(isolate, exception.what()))      \
                );                                                                      \
            }                                                                           \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        }                                                                               \
    }


#define NODE_GEOS_BINARY_PREDICATE(cppmethod, geosmethod)                               \
    typedef struct {                                                                    \
        Geometry *geom;                                                                 \
        Geometry *geom2;                                                                \
        Persistent<Function> cb;                                                        \
        bool result;                                                                    \
    } geosmethod##_baton_t;                                                             \
                                                                                        \
    void Geometry::EIO_##cppmethod(uv_work_t *req) {                                    \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        closure->result = closure->geom->_geom->geosmethod(closure->geom2->_geom);      \
    }                                                                                   \
                                                                                        \
    void Geometry::EIO_After##cppmethod(uv_work_t *req, int status) {                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
                                                                                        \
        assert(status == 0);                                                            \
        geosmethod##_baton_t *closure = static_cast<geosmethod##_baton_t *>(req->data); \
        Local<Value> argv[2] = {                                                        \
            Null(isolate), closure->result ? True(isolate) : False(isolate)             \
        };                                                                              \
        TryCatch tryCatch;                                                              \
        Local<Function> local_callback = Local<Function>::New(isolate, closure->cb);    \
        local_callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);          \
                                                                                        \
        if(tryCatch.HasCaught()) {                                                      \
            FatalException(isolate, tryCatch);                                          \
        }                                                                               \
                                                                                        \
        closure->cb.Reset();                                                            \
        closure->geom->Unref();                                                         \
        closure->geom2->_unref();                                                       \
                                                                                        \
        delete closure;                                                                 \
        delete req;                                                                     \
    }                                                                                   \
                                                                                        \
    void Geometry::cppmethod(const FunctionCallbackInfo<Value>& args)                   \
    {                                                                                   \
        Isolate* isolate = Isolate::GetCurrent();                                       \
        HandleScope scope(isolate);                                                     \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());                     \
        Geometry *geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());            \
        Local<Function> f = Local<Function>::Cast(args[1]);                             \
        if (args.Length() == 2) {                                                       \
            geosmethod##_baton_t *closure = new geosmethod##_baton_t();                 \
            closure->geom = geom;                                                       \
            closure->geom2 = geom2;                                                     \
            closure->cb.Reset(isolate, Persistent<Function>(isolate, f));               \
            uv_work_t *req = new uv_work_t;                                             \
            req->data = closure;                                                        \
            uv_queue_work(uv_default_loop(), req, EIO_##cppmethod, EIO_After##cppmethod);\
            geom->Ref();                                                                \
            geom2->_ref();                                                              \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        } else {                                                                        \
            try {                                                                       \
                args.GetReturnValue().Set(                                              \
                  geom->_geom->geosmethod(geom2->_geom) ? True(isolate) : False(isolate)\
                );                                                                      \
                return;                                                                 \
            } catch(geos::util::GEOSException exception) {                              \
                isolate->ThrowException(                                                \
                  Exception::Error(String::NewFromUtf8(isolate, exception.what()))      \
                );                                                                      \
            }                                                                           \
            args.GetReturnValue().Set(Undefined(isolate));                              \
        }                                                                               \
    }


#define NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(cppmethod, geosmethod)               \
    void Geometry::cppmethod(const FunctionCallbackInfo<Value>& args) {         \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());             \
        geos::geom::Geometry* result = geom->_geom->geosmethod();               \
        args.GetReturnValue().Set(Geometry::New(result));                       \
    }                                                                           \

#define NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(cppmethod, geosmethod)              \
    void Geometry::cppmethod(const FunctionCallbackInfo<Value>& args) {         \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());             \
        Geometry *geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());    \
        geos::geom::Geometry* result = geom->_geom->geosmethod(geom2->_geom);   \
        args.GetReturnValue().Set(Geometry::New(result));                       \
    }                                                                           \

#define NODE_GEOS_DOUBLE_GETTER(cppmethod, geosmethod)                          \
    void Geometry::cppmethod(const FunctionCallbackInfo<Value>& args) {\
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());             \
        args.GetReturnValue().Set(geom->_geom->geosmethod());                   \
    }                                                                           \

#define NODE_GEOS_V8_FUNCTION(cppmethod) \
    static void cppmethod(const FunctionCallbackInfo<Value>& args); \
    static void EIO_##cppmethod(uv_work_t *req); \
    static void EIO_After##cppmethod(uv_work_t *req, int status); \

class Geometry : public ObjectWrap {
 public:
    geos::geom::Geometry *_geom;
    Geometry();
    Geometry(geos::geom::Geometry *geom);
    ~Geometry();
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(geos::geom::Geometry* geometry);

    void _ref() { Ref(); };
    void _unref() { Unref(); };

 protected:
    static void New(const FunctionCallbackInfo<Value>& args);
    // GEOS unary predicates
    NODE_GEOS_V8_FUNCTION(IsSimple);
    NODE_GEOS_V8_FUNCTION(IsValid);
    NODE_GEOS_V8_FUNCTION(IsEmpty);
    NODE_GEOS_V8_FUNCTION(IsRectangle);

    // GEOS binary predicates
    NODE_GEOS_V8_FUNCTION(Disjoint);
    NODE_GEOS_V8_FUNCTION(Touches);
    NODE_GEOS_V8_FUNCTION(Intersects);
    NODE_GEOS_V8_FUNCTION(Crosses);
    NODE_GEOS_V8_FUNCTION(Within);
    NODE_GEOS_V8_FUNCTION(Contains);
    NODE_GEOS_V8_FUNCTION(Overlaps);
    NODE_GEOS_V8_FUNCTION(Equals);
    NODE_GEOS_V8_FUNCTION(Covers);
    NODE_GEOS_V8_FUNCTION(CoveredBy);

    //static void EqualsExact(const FunctionCallbackInfo<Value>& args);
    static void IsWithinDistance(const FunctionCallbackInfo<Value>& args);

    // GEOS topologic function
    static void Intersection(const FunctionCallbackInfo<Value>& args);
    static void Union(const FunctionCallbackInfo<Value>& args);
    static void Difference(const FunctionCallbackInfo<Value>& args);
    static void SymDifference(const FunctionCallbackInfo<Value>& args);

    static void GetBoundary(const FunctionCallbackInfo<Value>& args);
    static void GetEnvelope(const FunctionCallbackInfo<Value>& args);
    static void ConvexHull(const FunctionCallbackInfo<Value>& args);

    static void Buffer(const FunctionCallbackInfo<Value>& args);

    static void Distance(const FunctionCallbackInfo<Value>& args);

    static void GetArea(const FunctionCallbackInfo<Value>& args);
    static void GetLength(const FunctionCallbackInfo<Value>& args);

    static void GetSRID(const FunctionCallbackInfo<Value>& args);
    static void SetSRID(const FunctionCallbackInfo<Value>& args);

    static void GetGeometryType(const FunctionCallbackInfo<Value>& args);

    static void ToJSON(const FunctionCallbackInfo<Value>& args);

 private:
    static Persistent<Function> constructor;
    static void ToString(const FunctionCallbackInfo<Value>& args);

};
#endif
