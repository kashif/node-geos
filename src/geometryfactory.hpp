#ifndef GEOMETRY_FACTORY_HPP
#define GEOMETRY_FACTORY_HPP

#include "binding.hpp"
#include "geometry.hpp"
#include "precisionmodel.hpp"
#include "geos/geom/GeometryFactory.h"
#include "geos/geom/PrecisionModel.h"

class GeometryFactory : public ObjectWrap {

    public:
        GeometryFactory();
        GeometryFactory(const geos::geom::PrecisionModel *pm);
        GeometryFactory(const geos::geom::PrecisionModel *pm, int newSRID);
        ~GeometryFactory();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);

    protected:

    private:
        geos::geom::GeometryFactory* _factory;

};
#endif
