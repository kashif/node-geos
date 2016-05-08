#include "geojsonwriter.hpp"

Persistent<Function> GeoJSONWriter::constructor;

void GeoJSONWriter::Initialize(Handle<Object> target) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, GeoJSONWriter::New);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GeoJSONWriter"));

    NODE_SET_PROTOTYPE_METHOD(tpl, "write", GeoJSONWriter::Write);
    NODE_SET_PROTOTYPE_METHOD(tpl, "writeBbox", GeoJSONWriter::WriteBbox);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setRoundingPrecision", GeoJSONWriter::SetRoundingPrecision);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setBbox", GeoJSONWriter::SetBbox);

    constructor.Reset(isolate, tpl->GetFunction());

    target->Set(String::NewFromUtf8(isolate, "GeoJSONWriter"), tpl->GetFunction());
}

void GeoJSONWriter::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    GeoJSONWriter* writer;
    writer = new GeoJSONWriter();
    writer->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
}

double GeoJSONWriter::roundNumber(double coord) {
    if (decimalPlaces == -1) {
        return coord;
    }

    return round(coord * factor) / factor;
}

Handle<Array> GeoJSONWriter::coordinateToArray(const geos::geom::Coordinate* coord) {
    Isolate* isolate = Isolate::GetCurrent();

    Handle<Array> array = Array::New(isolate);
    array->Set(0, Number::New(isolate, roundNumber(coord->x)));
    array->Set(1, Number::New(isolate, roundNumber(coord->y)));
    if (coord->z == coord->z)
        array->Set(2, Number::New(isolate, roundNumber(coord->z)));

    return array;
}

Handle<Array> GeoJSONWriter::coordinateSequenceToArray(const geos::geom::CoordinateSequence* seq) {
    Isolate* isolate = Isolate::GetCurrent();

    int size = seq->getSize();
    Handle<Array> array = Array::New(isolate, size);
    for (int i = 0; i < size; i++) {
        array->Set(i, coordinateToArray(&seq->getAt(i)));
    }

    return array;
}

Handle<Array> GeoJSONWriter::geometryCollectionToArrayOfArrays(const geos::geom::GeometryCollection* geom) {
    Isolate* isolate = Isolate::GetCurrent();

    int size = geom->getNumGeometries();
    Handle<Array> array = Array::New(isolate, size);
    for (int i = 0; i < size; i++) {
        array->Set(i, getCoordsOrGeom(geom->getGeometryN(i)));
    }

    return array;
}

Handle<Array> GeoJSONWriter::geometryCollectionToArrayOfObjects(const geos::geom::GeometryCollection* geom) {
    Isolate* isolate = Isolate::GetCurrent();

    int size = geom->getNumGeometries();
    Handle<Array> array = Array::New(isolate, size);
    for (int i = 0; i < size; i++) {
        array->Set(i, write(geom->getGeometryN(i)));
    }

    return array;
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

        Isolate* isolate = Isolate::GetCurrent();

        int rings = g->getNumInteriorRing() + 1;

        Handle<Array> array = Array::New(isolate, rings);

        const geos::geom::CoordinateSequence* seq = g->getExteriorRing()->getCoordinatesRO();
        array->Set(0, coordinateSequenceToArray(seq));

        for (int i = 0; i < rings - 1; i++) {

            const geos::geom::CoordinateSequence* seq = g->getInteriorRingN(i)->getCoordinatesRO();
            array->Set(i + 1, coordinateSequenceToArray(seq));
        }

        return array;
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

    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    return Null(isolate);
}

Handle<Value> GeoJSONWriter::writeBbox(const geos::geom::Geometry* geom) {
    Isolate* isolate = Isolate::GetCurrent();

    if (geom->isEmpty()) {
        return Null(isolate);
    }


    const geos::geom::Envelope* envelope = geom->getEnvelopeInternal();

    Handle<Array> bbox = Array::New(isolate, 4);
    bbox->Set(0, Number::New(isolate, roundNumber(envelope->getMinX())));
    bbox->Set(1, Number::New(isolate, roundNumber(envelope->getMinY())));
    bbox->Set(2, Number::New(isolate, roundNumber(envelope->getMaxX())));
    bbox->Set(3, Number::New(isolate, roundNumber(envelope->getMaxY())));

    return bbox;
}

GeoJSONWriter::GeoJSONWriter() {
    setBbox(0);
    setRoundingPrecision(-1);
}

GeoJSONWriter::~GeoJSONWriter() {}

void GeoJSONWriter::SetRoundingPrecision(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();

    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    writer->setRoundingPrecision(args[0]->Int32Value());
    args.GetReturnValue().Set(Undefined(isolate));
}

void GeoJSONWriter::SetBbox(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();

    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    writer->setBbox(args[0]->BooleanValue());
    args.GetReturnValue().Set(Undefined(isolate));
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

void GeoJSONWriter::Write(const FunctionCallbackInfo<Value>& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    args.GetReturnValue().Set(writer->write(ObjectWrap::Unwrap<Geometry>(args[0]->ToObject())->_geom));
}

void GeoJSONWriter::WriteBbox(const FunctionCallbackInfo<Value>& args) {
    GeoJSONWriter* writer = ObjectWrap::Unwrap<GeoJSONWriter>(args.This());
    args.GetReturnValue().Set(writer->writeBbox(ObjectWrap::Unwrap<Geometry>(args[0]->ToObject())->_geom));
}

Handle<Value> GeoJSONWriter::write(const geos::geom::Geometry* geom) {
    Isolate* isolate = Isolate::GetCurrent();

    Handle<Object> object = Object::New(isolate);

    int typeId = geom->getGeometryTypeId();

    object->Set(
        String::NewFromUtf8(isolate, "type"),
        String::NewFromUtf8(isolate,
            typeId == geos::geom::GEOS_LINESTRING || typeId == geos::geom::GEOS_LINEARRING
                ? "LineString"
                : geom->getGeometryType().data()
        )
    );

    if (geom->isEmpty()) {
        if (typeId != geos::geom::GEOS_GEOMETRYCOLLECTION) {
            object->Set(String::NewFromUtf8(isolate, "coordinates"), Null(isolate));
        } else {
            object->Set(String::NewFromUtf8(isolate, "geometries"), Array::New(isolate));
        }
    } else {
        Handle<Value> coordsOrGeom = getCoordsOrGeom(geom);

        if (typeId != geos::geom::GEOS_GEOMETRYCOLLECTION) {
            object->Set(String::NewFromUtf8(isolate, "coordinates"), coordsOrGeom);
        } else {
            object->Set(String::NewFromUtf8(isolate, "geometries"), coordsOrGeom);
        }
    }

    if (bbox) {
        object->Set(String::NewFromUtf8(isolate, "bbox"), writeBbox(geom));
    }

    return object;
}
