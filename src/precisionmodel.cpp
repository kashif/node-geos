#include "precisionmodel.hpp"

PrecisionModel::PrecisionModel() {
    _model = new geos::geom::PrecisionModel();
}

PrecisionModel::PrecisionModel(double newScale) {
    _model = new geos::geom::PrecisionModel(newScale);
}

PrecisionModel::PrecisionModel(geos::geom::PrecisionModel::Type nModelType) {
    _model = new geos::geom::PrecisionModel(nModelType);
}

PrecisionModel::~PrecisionModel() {}

Persistent<FunctionTemplate> PrecisionModel::constructor;

void PrecisionModel::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(PrecisionModel::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("PrecisionModel"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "getType", GetType);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getScale", GetScale);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getOffsetX", GetOffsetX);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getOffsetY", GetOffsetY);

    NODE_SET_PROTOTYPE_METHOD(constructor, "toString", ToString);

    NODE_SET_PROTOTYPE_METHOD(constructor, "isFloating", IsFloating);

    target->Set(String::NewSymbol("PrecisionModel"), constructor->GetFunction());
}

Handle<Value> PrecisionModel::New(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model;
    if (args.Length() == 0) {
        model = new PrecisionModel();
    } else {
        if (args[0]->IsString()) {
            //type
            if(args[0]->ToString()->Equals(String::New("FIXED"))) {
                model = new PrecisionModel(geos::geom::PrecisionModel::FIXED);
            } else if (args[0]->ToString()->Equals(String::New("FLOATING"))) {
                model = new PrecisionModel(geos::geom::PrecisionModel::FLOATING);
            } else {
                model = new PrecisionModel(geos::geom::PrecisionModel::FLOATING_SINGLE);
            }
        } else {
            //double
            model = new PrecisionModel(args[0]->NumberValue());
        }
    }
    model->Wrap(args.This());
    return args.This();
}

Handle<Value> PrecisionModel::GetType(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return scope.Close(Integer::New(model->_model->getType()));
}
//TODO add a macro for this?
Handle<Value> PrecisionModel::GetScale(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return scope.Close(Number::New(model->_model->getScale()));
}

Handle<Value> PrecisionModel::GetOffsetX(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return scope.Close(Number::New(model->_model->getOffsetX()));
}

Handle<Value> PrecisionModel::GetOffsetY(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return scope.Close(Number::New(model->_model->getOffsetY()));
}

Handle<Value> PrecisionModel::ToString(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return scope.Close(String::New(model->_model->toString().data()));
}

Handle<Value> PrecisionModel::IsFloating(const Arguments& args) {
    HandleScope scope;
    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    return model->_model->isFloating() ? True() : False();
}
