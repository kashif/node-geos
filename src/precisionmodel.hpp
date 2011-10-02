#ifndef PRECISION_MODEL_HPP
#define PRECISION_MODEL_HPP

#include <geos/geom/PrecisionModel.h>
#include "binding.hpp"

class PrecisionModel : public ObjectWrap {
    public:
        PrecisionModel();
        PrecisionModel(double newScale);
        PrecisionModel(geos::geom::PrecisionModel::Type nModelType);
        ~PrecisionModel();
        geos::geom::PrecisionModel *_model;

        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);
        static Handle<Value> GetType(const Arguments& args);
        static Handle<Value> GetScale(const Arguments& args);
        static Handle<Value> GetOffsetX(const Arguments& args);
        static Handle<Value> GetOffsetY(const Arguments& args);
        static Handle<Value> ToString(const Arguments& args);
        static Handle<Value> IsFloating(const Arguments& args);

    protected:

    private:

};
#endif
