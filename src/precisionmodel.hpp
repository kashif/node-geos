#ifndef PRECISION_MODEL_HPP
#define PRECISION_MODEL_HPP

#include <geos/geom/PrecisionModel.h>
#include "binding.hpp"

class PrecisionModel : public ObjectWrap {
    public:
        PrecisionModel();
        ~PrecisionModel();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);

    protected:

    private:
        geos::geom::PrecisionModel* _model;

};
#endif
