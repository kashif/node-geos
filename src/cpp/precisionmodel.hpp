#ifndef PRECISION_MODEL_HPP
#define PRECISION_MODEL_HPP

#include <geos/geom/PrecisionModel.h>
#include "binding.hpp"

class PrecisionModel : public ObjectWrap {
    public:
        PrecisionModel();
        PrecisionModel(double newScale);
        PrecisionModel(geos::geom::PrecisionModel::Type nModelType);
        PrecisionModel(const geos::geom::PrecisionModel *model);
        ~PrecisionModel();
        geos::geom::PrecisionModel *_model;

        static void Initialize(Handle<Object> target);
        static Persistent<Function> constructor;
        static void New(const FunctionCallbackInfo<Value>& args);
        static Handle<Value> New(const geos::geom::PrecisionModel *model);
        static void GetType(const FunctionCallbackInfo<Value>& args);
        static void GetScale(const FunctionCallbackInfo<Value>& args);
        static void GetOffsetX(const FunctionCallbackInfo<Value>& args);
        static void GetOffsetY(const FunctionCallbackInfo<Value>& args);
        static void ToString(const FunctionCallbackInfo<Value>& args);
        static void IsFloating(const FunctionCallbackInfo<Value>& args);
        static void CompareTo(const FunctionCallbackInfo<Value>& args);

    protected:

    private:

};
#endif
