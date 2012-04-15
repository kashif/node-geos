#include "geometry.hpp"

Geometry::Geometry() {}

Geometry::Geometry(geos::geom::Geometry *geom) : ObjectWrap() {
    _geom = geom;
}

Geometry::~Geometry() {}

Persistent<FunctionTemplate> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(Geometry::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("Geometry"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "toString", Geometry::ToString);
    //GEOS unary predicates
    NODE_SET_PROTOTYPE_METHOD(constructor, "isSimple", Geometry::IsSimple);
    NODE_SET_PROTOTYPE_METHOD(constructor, "isValid", Geometry::IsValid);
    NODE_SET_PROTOTYPE_METHOD(constructor, "isEmpty", Geometry::IsEmpty);
    NODE_SET_PROTOTYPE_METHOD(constructor, "isRectangle", Geometry::IsRectangle);
    //GEOS binary predicates
    //TODO maybe define a macro for this too
    NODE_SET_PROTOTYPE_METHOD(constructor, "disjoint", Geometry::Disjoint);
    NODE_SET_PROTOTYPE_METHOD(constructor, "touches", Geometry::Touches);
    NODE_SET_PROTOTYPE_METHOD(constructor, "intersects", Geometry::Intersects);
    NODE_SET_PROTOTYPE_METHOD(constructor, "crosses", Geometry::Crosses);
    NODE_SET_PROTOTYPE_METHOD(constructor, "within", Geometry::Within);
    NODE_SET_PROTOTYPE_METHOD(constructor, "contains", Geometry::Contains);
    NODE_SET_PROTOTYPE_METHOD(constructor, "overlaps", Geometry::Overlaps);
    NODE_SET_PROTOTYPE_METHOD(constructor, "equals", Geometry::Equals);
    NODE_SET_PROTOTYPE_METHOD(constructor, "covers", Geometry::Covers);
    NODE_SET_PROTOTYPE_METHOD(constructor, "coveredBy", Geometry::CoveredBy);

    NODE_SET_PROTOTYPE_METHOD(constructor, "isWithinDistance", Geometry::IsWithinDistance);

    //GEOS binary topologic functions
    NODE_SET_PROTOTYPE_METHOD(constructor, "intersection", Geometry::Intersection);
    NODE_SET_PROTOTYPE_METHOD(constructor, "union", Geometry::Union);
    NODE_SET_PROTOTYPE_METHOD(constructor, "difference", Geometry::Difference);
    NODE_SET_PROTOTYPE_METHOD(constructor, "symDifference", Geometry::SymDifference);

    //GEOS unary topologic functions
    NODE_SET_PROTOTYPE_METHOD(constructor, "getEnvelope", Geometry::GetEnvelope);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getBoundary", Geometry::GetBoundary);
    NODE_SET_PROTOTYPE_METHOD(constructor, "convexHull", Geometry::ConvexHull);

    NODE_SET_PROTOTYPE_METHOD(constructor, "buffer", Geometry::Buffer);

    NODE_SET_PROTOTYPE_METHOD(constructor, "distance", Geometry::Distance);

    NODE_SET_PROTOTYPE_METHOD(constructor, "getArea", Geometry::GetArea);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getLength", Geometry::GetLength);
    NODE_SET_PROTOTYPE_METHOD(constructor, "getSRID", Geometry::GetSRID);
    NODE_SET_PROTOTYPE_METHOD(constructor, "setSRID", Geometry::SetSRID);

    NODE_SET_PROTOTYPE_METHOD(constructor, "getGeometryType", Geometry::GetGeometryType);

    NODE_SET_PROTOTYPE_METHOD(constructor, "toJSON", Geometry::ToJSON);

    target->Set(String::NewSymbol("Geometry"), constructor->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry *geometry) {
    HandleScope scope;
    Geometry *geom = new Geometry(geometry);
    Handle<Value> ext = External::New(geom);
    Handle<Object> obj = constructor->GetFunction()->NewInstance(1, &ext);
    geom->Wrap(obj);
    return scope.Close(obj);
}

Handle<Value> Geometry::New(const Arguments& args) {
    Geometry *geom;
    HandleScope scope;
    geom = new Geometry();
    geom->Wrap(args.This());
    return args.This();
}

Handle<Value> Geometry::ToString(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    return scope.Close(String::New(geom->_geom->toString().data()));
}

Handle<Value> Geometry::GetGeometryType(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    return scope.Close(String::New(geom->_geom->getGeometryType().data()));
}

Handle<Value> Geometry::Distance(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    return scope.Close(Number::New(geom->_geom->distance(geom2->_geom)));
}

Handle<Value> Geometry::IsWithinDistance(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    Geometry* geom2 = ObjectWrap::Unwrap<Geometry>(args[0]->ToObject());
    double distance = args[0]->NumberValue();
    return geom->_geom->isWithinDistance(geom2->_geom, distance) ? True() : False();
}

Handle<Value> Geometry::SetSRID(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    geom->_geom->setSRID(args[0]->IntegerValue());
    return Undefined();
}

Handle<Value> Geometry::ToJSON(const Arguments& args) {
    HandleScope scope;
    Geometry* geom = ObjectWrap::Unwrap<Geometry>(args.This());
    GeoJSONWriter* writer = new GeoJSONWriter();
    if (args.Length() >= 1 && args[0]->IsInt32()) {
        writer->setRoundingPrecision(args[0]->Int32Value());
    }
    if (args.Length() >= 2 && args[1]->IsBoolean()) {
        writer->setBbox(args[1]->BooleanValue());
    }
    Handle<Value> json = writer->write(geom->_geom);
    delete writer;
    return scope.Close(json);
}

Handle<Value> Geometry::Buffer(const Arguments& args) {
    HandleScope scope;
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

    return scope.Close(result);
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
