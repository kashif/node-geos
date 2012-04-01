#include "geojsonwriter.hpp"

Persistent<FunctionTemplate> GeoJSONWriter::constructor;

void GeoJSONWriter::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(GeoJSONWriter::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("GeoJSONWriter"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "write", GeoJSONWriter::Write);
    NODE_SET_PROTOTYPE_METHOD(constructor, "writeBbox", GeoJSONWriter::WriteBbox);
    NODE_SET_PROTOTYPE_METHOD(constructor, "setRoundingPrecision", GeoJSONWriter::SetRoundingPrecision);
    NODE_SET_PROTOTYPE_METHOD(constructor, "setBbox", GeoJSONWriter::SetBbox);

    target->Set(String::NewSymbol("GeoJSONWriter"), constructor->GetFunction());
}

Handle<Value> GeoJSONWriter::New(const Arguments& args) {
    GeoJSONWriter* writer;
    HandleScope scope;
    writer = new GeoJSONWriter();
    writer->Wrap(args.This());
    return args.This();
}

double GeoJSONWriter::roundNumber(double coord) {
    if (decimalPlaces == -1) {
        return coord;
    }

    return round(coord * factor) / factor;
}

Handle<Array> GeoJSONWriter::coordinateToArray(const geos::geom::Coordinate* coord) {
    HandleScope scope;

    Handle<Array> array = Array::New(2);
    array->Set(0, Number::New(roundNumber(coord->x)));
    array->Set(1, Number::New(roundNumber(coord->y)));

    return scope.Close(array);
}

Handle<Array> GeoJSONWriter::coordinateSequenceToArray(const geos::geom::CoordinateSequence* seq) {
    HandleScope scope;

    int size = seq->getSize();
    Handle<Array> array = Array::New(size);
    for (int i = 0; i < size; i++) {
        array->Set(i, coordinateToArray(&seq->getAt(i)));
    }

    return scope.Close(array);
}

Handle<Array> GeoJSONWriter::geometryCollectionToArrayOfArrays(const geos::geom::GeometryCollection* geom) {
    HandleScope scope;

    int size = geom->getNumGeometries();
    Handle<Array> array = Array::New(size);
    for (int i = 0; i < size; i++) {
        array->Set(i, getCoordsOrGeom(geom->getGeometryN(i)));
    }

    return scope.Close(array);
}

Handle<Array> GeoJSONWriter::geometryCollectionToArrayOfObjects(const geos::geom::GeometryCollection* geom) {
    HandleScope scope;

    int size = geom->getNumGeometries();
    Handle<Array> array = Array::New(size);
    for (int i = 0; i < size; i++) {
        array->Set(i, write(geom->getGeometryN(i)));
    }

    return scope.Close(array);
}

Handle<Value> GeoJSONWriter::getCoordsOrGeom(const geos::geom::Geometry* geom) {
    int typeId = geom->getGeometryTypeId();

    if (typeId == geos::geom::GEOS_POINT) {
        const geos::geom::Point* g = dynamic_cast< const geos::geom::Point* >(geom);
        return coordinateToArray(g->getCoordinate());
    }

    if (typeId == geos::geom::GEOS_LINESTRING || typeId == geos::geom::GEOS_LINEARRING) {
        const geos::geom::LineString* g = dynamic_cast< const geos::geom::LineString* >(geom);
        return coordinateSequenceToArray(g->getCoordinatesRO());
    }

    if (typeId == geos::geom::GEOS_POLYGON) {
        const geos::geom::Polygon* g = dynamic_cast< const geos::geom::Polygon* >(geom);

        HandleScope scope;

        int rings = g->getNumInteriorRing() + 1;

        Handle<Array> array = Array::New(rings);

        const geos::geom::CoordinateSequence* seq = g->getExteriorRing()->getCoordinatesRO();
        array->Set(0, coordinateSequenceToArray(seq));

        for (int i = 0; i < rings - 1; i++) {

            const geos::geom::CoordinateSequence* seq = g->getInteriorRingN(i)->getCoordinatesRO();
            array->Set(i + 1, coordinateSequenceToArray(seq));
        }

        return scope.Close(array);
    }

    if (typeId == geos::geom::GEOS_MULTIPOINT) {
        const geos::geom::MultiPoint* g = dynamic_cast< const geos::geom::MultiPoint* >(geom);
        return geometryCollectionToArrayOfArrays(g);
    }

    if (typeId == geos::geom::GEOS_MULTILINESTRING) {
        const geos::geom::MultiLineString* g = dynamic_cast< const geos::geom::MultiLineString* >(geom);
        return geometryCollectionToArrayOfArrays(g);
    }

    if (typeId == geos::geom::GEOS_MULTIPOLYGON) {
        const geos::geom::MultiPolygon* g = dynamic_cast< const geos::geom::MultiPolygon* >(geom);
        return geometryCollectionToArrayOfArrays(g);
    }

    if (typeId == geos::geom::GEOS_GEOMETRYCOLLECTION) {
        const geos::geom::GeometryCollection* g = dynamic_cast< const geos::geom::GeometryCollection* >(geom);
        return geometryCollectionToArrayOfObjects(g);
    }

    HandleScope scope;
    return scope.Close(Null());
}

Handle<Value> GeoJSONWriter::writeBbox(const geos::geom::Geometry* geom) {
    HandleScope scope;

    if (geom->isEmpty()) {
        return scope.Close(Null());
    }


    const geos::geom::Envelope* envelope = geom->getEnvelopeInternal();

    Handle<Array> bbox = Array::New(4);
    bbox->Set(0, Number::New(roundNumber(envelope->getMinX())));
    bbox->Set(1, Number::New(roundNumber(envelope->getMinY())));
    bbox->Set(2, Number::New(roundNumber(envelope->getMaxX())));
    bbox->Set(3, Number::New(roundNumber(envelope->getMaxY())));

    return scope.Close(bbox);
}

GeoJSONWriter::GeoJSONWriter() {
    setBbox(0);
    setRoundingPrecision(-1);
}

GeoJSONWriter::~GeoJSONWriter() {}

Handle<Value> GeoJSONWriter::SetRoundingPrecision(const Arguments& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    writer->setRoundingPrecision(args[0]->Int32Value());
    return Undefined();
}

Handle<Value> GeoJSONWriter::SetBbox(const Arguments& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    writer->setBbox(args[0]->BooleanValue());
    return Undefined();
}

void GeoJSONWriter::setRoundingPrecision(int places) {
    if (places > 15 || places < 0) {
        places = -1;
    }
    decimalPlaces = places;
    factor = pow(10, decimalPlaces);
}

void GeoJSONWriter::setBbox(bool _bbox) {
    bbox = _bbox;
}

Handle<Value> GeoJSONWriter::Write(const Arguments& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    return writer->write(ObjectWrap::Unwrap<Geometry>(args[0]->ToObject())->_geom);
}

Handle<Value> GeoJSONWriter::WriteBbox(const Arguments& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    return writer->writeBbox(ObjectWrap::Unwrap<Geometry>(args[0]->ToObject())->_geom);
}

Handle<Value> GeoJSONWriter::write(const geos::geom::Geometry* geom) {
    HandleScope scope;

    Handle<Object> object = Object::New();

    int typeId = geom->getGeometryTypeId();

    object->Set(
        String::New("type"),
        String::New(
            typeId == geos::geom::GEOS_LINESTRING || typeId == geos::geom::GEOS_LINEARRING
                ? "LineString"
                : geom->getGeometryType().data()
        )
    );

    if (geom->isEmpty()) {
        if (typeId != geos::geom::GEOS_GEOMETRYCOLLECTION) {
            object->Set(String::New("coordinates"), Null());
        } else {
            object->Set(String::New("geometries"), Null());
        }
    } else {
        Handle<Value> coordsOrGeom = getCoordsOrGeom(geom);

        if (typeId != geos::geom::GEOS_GEOMETRYCOLLECTION) {
            object->Set(String::New("coordinates"), coordsOrGeom);
        } else {
            object->Set(String::New("geometries"), coordsOrGeom);
        }
    }

    if (bbox) {
        object->Set(String::New("bbox"), writeBbox(geom));
    }

    return scope.Close(object);
}
