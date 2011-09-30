#include "geometry.hpp"

Geometry::Geometry() {
}

Geometry::Geometry(geos::geom::Geometry* geom) : ObjectWrap() {
    _geom = geom;
}
Geometry::~Geometry()
{
}

Persistent<FunctionTemplate> Geometry::constructor;

void Geometry::Initialize(Handle<Object> target)
{
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

    //GEOS topologic function
    NODE_SET_PROTOTYPE_METHOD(constructor, "intersection", Geometry::Intersection);
    //NODE_SET_PROTOTYPE_METHOD(constructor, "union", Geometry::Union);
    NODE_SET_PROTOTYPE_METHOD(constructor, "difference", Geometry::Difference);
    NODE_SET_PROTOTYPE_METHOD(constructor, "symDifference", Geometry::SymDifference);

    target->Set(String::NewSymbol("Geometry"), constructor->GetFunction());
}

Handle<Value> Geometry::New(geos::geom::Geometry* geometry) {
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

// GEOS topologic functions
NODE_GEOS_TOPOLOGIC_FUNCTION(Intersection, intersection);
//NODE_GEOS_TOPOLOGIC_FUNCTION(Union, union); TODO fix the keyword problem
NODE_GEOS_TOPOLOGIC_FUNCTION(Difference, difference);
NODE_GEOS_TOPOLOGIC_FUNCTION(SymDifference, symDifference);
