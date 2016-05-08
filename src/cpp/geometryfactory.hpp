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
        static Persistent<Function> constructor;
        static void New(const FunctionCallbackInfo<Value>& args);

        static void GetPrecisionModel(const FunctionCallbackInfo<Value>& args);
        static void GetSRID(const FunctionCallbackInfo<Value>& args);
        static void Destroy(const FunctionCallbackInfo<Value>& args);

    protected:

    private:

};
#endif
