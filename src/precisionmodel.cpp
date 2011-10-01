#include "precisionmodel.hpp"

PrecisionModel::PrecisionModel() {
    _model = new geos::geom::PrecisionModel();
}

PrecisionModel::~PrecisionModel() {}

Persistent<FunctionTemplate> PrecisionModel::constructor;

void PrecisionModel::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(PrecisionModel::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("PrecisionModel"));

    target->Set(String::NewSymbol("PrecisionModel"), constructor->GetFunction());
}

Handle<Value> PrecisionModel::New(const Arguments& args) {
    HandleScope scope;
    PrecisionModel* model = new PrecisionModel();
    model->Wrap(args.This());
    return args.This();
}

