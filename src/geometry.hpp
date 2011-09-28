#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <geos/geom/Geometry.h>
#include "binding.hpp"

class Geometry : public ObjectWrap {
 public:
    Geometry();
    Geometry(geos::geom::Geometry* geom);
    ~Geometry();
    static void Initialize(Handle<Object> target);
    static Handle<Value> New(geos::geom::Geometry* geometry);

 protected:
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Equals(const Arguments& args);

 private:
    static Persistent<FunctionTemplate> constructor;
    geos::geom::Geometry* _geom;
    static Handle<Value> ToString(const Arguments& args);

};
#endif
