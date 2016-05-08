#include "geometry.hpp"

Geometry::Geometry() {}

Geometry::Geometry(geos::geom::Geometry *geom) : ObjectWrap() {
    _geom = geom;
}

Geometry::~Geometry() {}

Persistent<Function> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Geometry::New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Geometry"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", Geometry::ToString);
    //GEOS unary predicates
    NODE_SET_PROTOTYPE_METHOD(tpl, "isSimple", Geometry::IsSimple);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isValid", Geometry::IsValid);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isEmpty", Geometry::IsEmpty);
    NODE_SET_PROTOTYPE_METHOD(tpl, "isRectangle", Geometry::IsRectangle);
    //GEOS binary predicates
    //TODO maybe define a macro for this too
    NODE_SET_PROTOTYPE_METHOD(tpl, "disjoint", Geometry::Disjoint);
    NODE_SET_PROTOTYPE_METHOD(tpl, "touches", Geometry::Touches);
    NODE_SET_PROTOTYPE_METHOD(tpl, "intersects", Geometry::Intersects);
    NODE_SET_PROTOTYPE_METHOD(tpl, "crosses", Geometry::Crosses);
    NODE_SET_PROTOTYPE_METHOD(tpl, "within", Geometry::Within);
    NODE_SET_PROTOTYPE_METHOD(tpl, "contains", Geometry::Contains);
    NODE_SET_PROTOTYPE_METHOD(tpl, "overlaps", Geometry::Overlaps);
    NODE_SET_PROTOTYPE_METHOD(tpl, "equals", Geometry::Equals);
    NODE_SET_PROTOTYPE_METHOD(tpl, "covers", Geometry::Covers);
    NODE_SET_PROTOTYPE_METHOD(tpl, "coveredBy", Geometry::CoveredBy);

    NODE_SET_PROTOTYPE_METHOD(tpl, "isWithinDistance", Geometry::IsWithinDistance);

    //GEOS binary topologic functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "intersection", Geometry::Intersection);
    NODE_SET_PROTOTYPE_METHOD(tpl, "union", Geometry::Union);
    NODE_SET_PROTOTYPE_METHOD(tpl, "difference", Geometry::Difference);
    NODE_SET_PROTOTYPE_METHOD(tpl, "symDifference", Geometry::SymDifference);

    //GEOS unary topologic functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "getEnvelope", Geometry::GetEnvelope);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getBoundary", Geometry::GetBoundary);
    NODE_SET_PROTOTYPE_METHOD(tpl, "convexHull", Geometry::ConvexHull);

    NODE_SET_PROTOTYPE_METHOD(tpl, "buffer", Geometry::Buffer);

    NODE_SET_PROTOTYPE_METHOD(tpl, "distance", Geometry::Distance);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getArea", Geometry::GetArea);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getLength", Geometry::GetLength);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getSRID", Geometry::GetSRID);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setSRID", Geometry::SetSRID);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getGeometryType", Geometry::GetGeometryType);

    NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", Geometry::ToJSON);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "Geometry"), tpl->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry *geometry) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry *geom = new Geometry(geometry);
    Handle<Value> ext = External::New(isolate, geom);

    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Handle<Object> obj = cons->NewInstance(1, &ext);
    geom->Wrap(obj);
    return obj;
}

void Geometry::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry *geom;
    geom = new Geometry();
    geom->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

void Geometry::ToString(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, geom->_geom->toString().data()));
}

void Geometry::GetGeometryType(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, geom->_geom->getGeometryType().data()));
}

void Geometry::Distance(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    args.GetReturnValue().Set(Number::New(isolate, geom->_geom->distance(geom2->_geom)));
}

void Geometry::IsWithinDistance(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    double distance = args[0]->NumberValue();
    args.GetReturnValue().Set(
      geom->_geom->isWithinDistance(geom2->_geom, distance) ? True(isolate) : False(isolate)
    );
}

void Geometry::SetSRID(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    geom->_geom->setSRID(args[0]->IntegerValue());
    args.GetReturnValue().Set(Undefined(isolate));
}

void Geometry::ToJSON(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    GeoJSONWriter writer;
    if (args.Length() >= 1 && args[0]->IsInt32()) {
        writer.setRoundingPrecision(args[0]->Int32Value());
    }
    if (args.Length() >= 2 && args[1]->IsBoolean()) {
        writer.setBbox(args[1]->BooleanValue());
    }
    Handle<Value> json = writer.write(geom->_geom);
    args.GetReturnValue().Set(json);
}

void Geometry::Buffer(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    double distance;
    int quadrantSegments;
    Handle<Value> result;

    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    distance = args[0]->NumberValue();

    if (args.Length() == 1) {
        result = Geometry::New(geom->_geom->buffer(distance));
    } else if (args.Length() == 2) {
        quadrantSegments = args[1]->IntegerValue();
        result = Geometry::New(geom->_geom->buffer(distance, quadrantSegments));
    } else {
        quadrantSegments = args[1]->IntegerValue();
        int endCapStyle = args[2]->IntegerValue();
        result = Geometry::New(geom->_geom->buffer(distance, quadrantSegments, endCapStyle));
    }

    args.GetReturnValue().Set(result);
}

//GEOS unary predicates
NODE_GEOS_UNARY_PREDICATE(IsSimple, isSimple);
NODE_GEOS_UNARY_PREDICATE(IsValid, isValid);
NODE_GEOS_UNARY_PREDICATE(IsEmpty, isEmpty);
NODE_GEOS_UNARY_PREDICATE(IsRectangle, isRectangle);

// GEOS binary predicates
NODE_GEOS_BINARY_PREDICATE(Disjoint, disjoint);
NODE_GEOS_BINARY_PREDICATE(Touches, touches);
NODE_GEOS_BINARY_PREDICATE(Intersects, intersects);
NODE_GEOS_BINARY_PREDICATE(Crosses, crosses);
NODE_GEOS_BINARY_PREDICATE(Within, within);
NODE_GEOS_BINARY_PREDICATE(Contains, contains);
NODE_GEOS_BINARY_PREDICATE(Overlaps, overlaps);
NODE_GEOS_BINARY_PREDICATE(Equals, equals);
NODE_GEOS_BINARY_PREDICATE(Covers, covers);
NODE_GEOS_BINARY_PREDICATE(CoveredBy, coveredBy);

// GEOS unary topologic functions
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(GetEnvelope, getEnvelope);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(GetBoundary, getBoundary);
NODE_GEOS_UNARY_TOPOLOGIC_FUNCTION(ConvexHull, convexHull);

// GEOS binary topologic functions
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Intersection, intersection);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Union, Union);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(Difference, difference);
NODE_GEOS_BINARY_TOPOLOGIC_FUNCTION(SymDifference, symDifference);

NODE_GEOS_DOUBLE_GETTER(GetArea, getArea);
NODE_GEOS_DOUBLE_GETTER(GetLength, getLength);
NODE_GEOS_DOUBLE_GETTER(GetSRID, getSRID);
