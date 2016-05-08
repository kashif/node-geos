#include "geometryfactory.hpp"

GeometryFactory::GeometryFactory() {
    _factory = new geos::geom::GeometryFactory();
}

GeometryFactory::GeometryFactory(const geos::geom::PrecisionModel *pm) {
    _factory = new geos::geom::GeometryFactory(pm);
}

GeometryFactory::GeometryFactory(const geos::geom::PrecisionModel *pm, int newSRID) {
    _factory = new geos::geom::GeometryFactory(pm, newSRID);
}

GeometryFactory::~GeometryFactory() {}

Persistent<Function> GeometryFactory::constructor;

void GeometryFactory::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, GeometryFactory::New);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GeometryFactory"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "getPrecisionModel", GetPrecisionModel);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getSRID", GetSRID);
    NODE_SET_PROTOTYPE_METHOD(tpl, "destroy", Destroy);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "GeometryFactory"), tpl->GetFunction());
}

void GeometryFactory::New(const FunctionCallbackInfo<Value>& args) {
    GeometryFactory* factory;
    if (args.Length() == 0) {
        factory = new GeometryFactory();
    } else if (args.Length() == 1) {
        PrecisionModel* model = ObjectWrap::Unwrap<PrecisionModel>(args[0]->ToObject());
        factory = new GeometryFactory(model->_model);
    } else {
        PrecisionModel* model = ObjectWrap::Unwrap<PrecisionModel>(args[0]->ToObject());
        int newSRID = args[1]->IntegerValue();
        factory = new GeometryFactory(model->_model, newSRID);
    }
    factory->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void GeometryFactory::GetSRID(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
    args.GetReturnValue().Set(Integer::New(isolate, factory->_factory->getSRID()));
}

void GeometryFactory::GetPrecisionModel(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
    args.GetReturnValue().Set(PrecisionModel::New(factory->_factory->getPrecisionModel()));
}

void GeometryFactory::Destroy(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
  factory->_factory->destroyGeometry(geom->_geom);
  args.GetReturnValue().Set(Undefined(isolate));
}
