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

Persistent<FunctionTemplate> GeometryFactory::constructor;

void GeometryFactory::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(GeometryFactory::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("GeometryFactory"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "getPrecisionModel", GetPrecisionModel);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getSRID", GetSRID);
    NODE_SET_PROTOTYPE_METHOD(constructor, "destroy", Destroy);

    target->Set(String::NewSymbol("GeometryFactory"), constructor->GetFunction());
}

Handle<Value> GeometryFactory::New(const Arguments& args) {
    HandleScope scope;
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
    return args.This();
}

Handle<Value> GeometryFactory::GetSRID(const Arguments& args) {
    HandleScope scope;
    GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
    return scope.Close(Integer::New(factory->_factory->getSRID()));
}

Handle<Value> GeometryFactory::GetPrecisionModel(const Arguments& args) {
    HandleScope scope;
    GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
    return scope.Close(PrecisionModel::New(factory->_factory->getPrecisionModel()));
}

Handle<Value> GeometryFactory::Destroy(const Arguments& args) {
  HandleScope scope;
  GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args.This());
  Geometry *geom = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
  factory->_factory->destroyGeometry(geom->_geom);
  return Undefined();
}
