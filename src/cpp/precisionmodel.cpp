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
PrecisionModel::PrecisionModel(const geos::geom::PrecisionModel *model) {
    _model = (geos::geom::PrecisionModel*) model; //hacky
}

PrecisionModel::~PrecisionModel() {}

Persistent<Function> PrecisionModel::constructor;

void PrecisionModel::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, PrecisionModel::New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "PrecisionModel"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "getType", GetType);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getScale", GetScale);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getOffsetX", GetOffsetX);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getOffsetY", GetOffsetY);

    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", ToString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "isFloating", IsFloating);

    NODE_SET_PROTOTYPE_METHOD(tpl, "compareTo", CompareTo);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "PrecisionModel"), tpl->GetFunction());
}

void PrecisionModel::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model;
    if (args.Length() == 0) {
        model = new PrecisionModel();
    } else {
        if (args[0]->IsString()) {
            //type
            if(args[0]->ToString()->Equals(String::NewFromUtf8(isolate, "FIXED"))) {
                model = new PrecisionModel(geos::geom::PrecisionModel::FIXED);
            } else if (args[0]->ToString()->Equals(String::NewFromUtf8(isolate, "FLOATING"))) {
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
    args.GetReturnValue().Set(args.This());
}

Handle<Value> PrecisionModel::New(const geos::geom::PrecisionModel *m) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = new PrecisionModel(m);
    Handle<Value> ext = External::New(isolate, model);
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Handle<Object> obj = cons->NewInstance(1, &ext);
    model->Wrap(obj);
    return obj;
}

void PrecisionModel::GetType(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(Integer::New(isolate, model->_model->getType()));
}
//TODO add a macro for this?
void PrecisionModel::GetScale(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(Number::New(isolate, model->_model->getScale()));
}

void PrecisionModel::GetOffsetX(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(Number::New(isolate, model->_model->getOffsetX()));
}

void PrecisionModel::GetOffsetY(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(Number::New(isolate, model->_model->getOffsetY()));
}

void PrecisionModel::ToString(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, model->_model->toString().data()));
}

void PrecisionModel::IsFloating(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    args.GetReturnValue().Set(model->_model->isFloating() ? True(isolate) : False(isolate));
}

void PrecisionModel::CompareTo(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    PrecisionModel *model = ObjectWrap::Unwrap<PrecisionModel>(args.This());
    PrecisionModel *model2 = ObjectWrap::Unwrap<PrecisionModel>(args[0]->ToObject());
    args.GetReturnValue().Set(Integer::New(isolate, model->_model->compareTo(model2->_model)));
}
