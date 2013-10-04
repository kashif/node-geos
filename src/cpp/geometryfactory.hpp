#ifndef GEOMETRY_FACTORY_HPP
#define GEOMETRY_FACTORY_HPP

#include "binding.hpp"
#include "geometry.hpp"
#include "precisionmodel.hpp"
#include "geos/geom/GeometryFactory.h"
#include "geos/geom/PrecisionModel.h"

class GeometryFactory : public ObjectWrap {

    public:
        geos::geom::GeometryFactory* _factory;

        GeometryFactory();
        GeometryFactory(const geos::geom::PrecisionModel *pm);
        GeometryFactory(const geos::geom::PrecisionModel *pm, int newSRID);
        ~GeometryFactory();

        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);

        static Handle<Value> GetPrecisionModel(const Arguments& args);
        static Handle<Value> GetSRID(const Arguments& args);
        static Handle<Value> Destroy(const Arguments& args);

    protected:

    private:

};
#endif
