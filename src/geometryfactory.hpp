#ifndef GEOMETRY_FACTORY_HPP
#define GEOMETRY_FACTORY_HPP

#include "binding.hpp"
#include "geometry.hpp"
#include "geos/geom/GeometryFactory.h"

class GeometryFactory : public ObjectWrap {

    public:
        GeometryFactory();
        ~GeometryFactory();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);

    protected:

    private:
        geos::geom::GeometryFactory* _factory;

};
#endif
