#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <geos/geom/Geometry.h>
#include "binding.hpp"

#define NODE_GEOS_UNARY_PREDICATE(cppmethod, geosmethod)                    \
    Handle<Value> Geometry::cppmethod(const Arguments& args)                \
    {                                                                       \
        HandleScope scope;                                                  \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());         \
        if (geom->_geom->geosmethod()) {                                    \
            return True();                                                  \
        } else {                                                            \
            return False();                                                 \
        }                                                                   \
    };                                                                      \

#define NODE_GEOS_BINARY_PREDICATE(cppmethod, geosmethod)                   \
    Handle<Value> Geometry::cppmethod(const Arguments& args)                \
    {                                                                       \
        HandleScope scope;                                                  \
        Geometry *geom = ObjectWrap::Unwrap<Geometry>(args.This());         \
        Geometry *geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());\
        if (geom->_geom->geosmethod(geom2->_geom)) {                        \
            return True();                                                  \
        } else {                                                            \
            return False();                                                 \
        }                                                                   \
    };                                                                      \

class Geometry : public ObjectWrap {
 public:
    Geometry();
    Geometry(geos::geom::Geometry* geom);
    ~Geometry();
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(geos::geom::Geometry* geometry);

 protected:
    static Handle<Value> New(const Arguments& args);
    // GEOS unary predicates
    static Handle<Value> IsSimple(const Arguments& args);
    static Handle<Value> IsValid(const Arguments& args);
    static Handle<Value> IsEmpty(const Arguments& args);
    static Handle<Value> IsRectangle(const Arguments& args);
    // GEOS binary predicates
    static Handle<Value> Disjoint(const Arguments& args);
    static Handle<Value> Touches(const Arguments& args);
    static Handle<Value> Intersects(const Arguments& args);
    static Handle<Value> Crosses(const Arguments& args);
    static Handle<Value> Within(const Arguments& args);
    static Handle<Value> Contains(const Arguments& args);
    static Handle<Value> Overlaps(const Arguments& args);
    static Handle<Value> Equals(const Arguments& args);
    static Handle<Value> Covers(const Arguments& args);
    static Handle<Value> CoveredBy(const Arguments& args);

    //static Handle<Value> EqualsExact(const Arguments& args);

 private:
    static Persistent<FunctionTemplate> constructor;
    geos::geom::Geometry* _geom;
    static Handle<Value> ToString(const Arguments& args);

};
#endif
