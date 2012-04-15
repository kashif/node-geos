#include "geojsonreader.hpp"

Persistent<FunctionTemplate> GeoJSONReader::constructor;

GeoJSONReader::GeoJSONReader()
        :
        geometryFactory(geos::geom::GeometryFactory::getDefaultInstance()),
        precisionModel(geometryFactory->getPrecisionModel()),
        coordinateSequenceFactory(geometryFactory->getCoordinateSequenceFactory())
{}

GeoJSONReader::GeoJSONReader(const geos::geom::GeometryFactory *gf)
        :
        geometryFactory(gf),
        precisionModel(gf->getPrecisionModel()),
        coordinateSequenceFactory(gf->getCoordinateSequenceFactory())
{}

GeoJSONReader::~GeoJSONReader() {}

void GeoJSONReader::Initialize(Handle<Object> target) {
    HandleScope scope;

    constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(GeoJSONReader::New));
    constructor->InstanceTemplate()->SetInternalFieldCount(1);
    constructor->SetClassName(String::NewSymbol("GeoJSONReader"));

    NODE_SET_PROTOTYPE_METHOD(constructor, "read", GeoJSONReader::Read);

    target->Set(String::NewSymbol("GeoJSONReader"), constructor->GetFunction());
}

Handle<Value> GeoJSONReader::New(const Arguments& args) {
    HandleScope scope;
    GeoJSONReader* reader;

    if(args.Length() == 1) {
        GeometryFactory *factory = ObjectWrap::Unwrap<GeometryFactory>(args[0]->ToObject());
        reader = new GeoJSONReader(factory->_factory);
    } else {
        reader = new GeoJSONReader();
    }

    reader->Wrap(args.This());
    return args.This();
}

Handle<Value> GeoJSONReader::Read(const Arguments& args) {
    GeoJSONReader* reader = ObjectWrap::Unwrap<GeoJSONReader>(args.This());
    try {
        geos::geom::Geometry* g = reader->read(args[0]);
        return Geometry::New(g);
    }
    catch (const char* e) {
        return ThrowException(Exception::Error(String::New(e)));
    }
    catch (geos::util::GEOSException e) {
        return ThrowException(Exception::Error(String::New(e.what())));
    }
}

geos::geom::Coordinate GeoJSONReader::getCoordinate(Handle<Value> coords) {

    return getCoordinate(coords, 1);
}

double GeoJSONReader::valueToDouble(Handle<Value> value) {

    double number = value->NumberValue();
    if (number != number)
        throw "A coordinate value must be numeric";


    return number;
}

geos::geom::Coordinate GeoJSONReader::getCoordinate(Handle<Value> value, bool acceptArrayOnly) {

    bool isArray = value->IsArray();

    if (acceptArrayOnly) {
        if (!isArray)
            throw "A coordinate must be an instance of Array";


    }
    else {
        if (
            !isArray
            && !value->IsNull()
            && !value->IsUndefined()
        )
            throw "A coordinate must be an instance of Array or null";


        if (!isArray)
            return geos::geom::Coordinate::getNull();
    }


    Handle<Array> array = Handle<Array>::Cast(value);
    uint32_t length = array->Length();
    if (length < 2)
        throw "A coordinate's length must be >= 2";


    geos::geom::Coordinate coord;

    coord.x = valueToDouble(array->Get(0));
    coord.y = valueToDouble(array->Get(1));
    if (length > 2) {
        coord.z = valueToDouble(array->Get(2));
    }
    precisionModel->makePrecise(&coord);

    return coord;
}

geos::geom::CoordinateSequence* GeoJSONReader::getCoordinates(Handle<Value> value) {

    if (!value->IsArray())
        throw "A coordinate sequence must be an instance of Array";


    Handle<Array> array = Handle<Array>::Cast(value);

    uint32_t length = array->Length();
    geos::geom::CoordinateSequence* sequence = coordinateSequenceFactory->create(length, 3);

    try {
        for (uint32_t i = 0; i < length; i++) {

            sequence->setAt(getCoordinate(array->Get(i)), (std::size_t)i);
        }
    }
    catch (...) {
        delete sequence;
        throw;
    }


    return sequence;
}

geos::geom::LinearRing* GeoJSONReader::getLinearRing(Handle<Value> value) {

    geos::geom::CoordinateSequence* coordinates = getCoordinates(value);
    return geometryFactory->createLinearRing(coordinates);
}

Handle<Value> GeoJSONReader::getCoordsArray(Handle<Object> geojson) {

    Handle<String> coordsKey = String::New("coordinates");
    if (!geojson->HasOwnProperty(coordsKey))
        throw "Property \"coordinates\" is missing";


    Handle<Value> coords = geojson->Get(coordsKey);
    if (
        !coords->IsArray()
        && !coords->IsNull()
        && !coords->IsUndefined()
    )
        throw "Property \"coordinates\" must be an instance of Array or null";


    return coords;
}

Handle<Array> GeoJSONReader::getGeomsArray(Handle<Object> geojson) {

    Handle<String> geomsKey = String::New("geometries");
    if (!geojson->HasOwnProperty(geomsKey))
        throw "Property \"geometries\" is missing";


    Handle<Value> geoms = geojson->Get(geomsKey);
    if (
        !geoms->IsArray()
    )
        throw "Property \"geometries\" must be an instance of Array";


    return Handle<Array>::Cast(geoms);
}

geos::geom::Geometry* GeoJSONReader::read(Handle<Value> value) {

    if (!value->IsObject()) {
        throw "GeoJSON must be an instance of Object";
    }

    HandleScope scope;

    Handle<Object> obj = Handle<Object>::Cast(value);

    Handle<String> typeKey = String::New("type");

    if (!obj->HasOwnProperty(typeKey)) {
        throw "Property \"type\" is missing";
    }

    std::string string = *String::Utf8Value(obj->Get(typeKey)->ToString());

    geos::geom::Geometry* g;

    try {
        if (string == "Point") {
            g = getPoint(getCoordsArray(obj));
        } else if (string == "LineString") {
            g = getLineString(getCoordsArray(obj));
        } else if (string == "Polygon") {
            g = getPolygon(getCoordsArray(obj));
        } else if (string == "MultiPoint") {
            g = getMultiPoint(getCoordsArray(obj));
        } else if (string == "MultiLineString") {
            g = getMultiLineString(getCoordsArray(obj));
        } else if (string == "MultiPolygon") {
            g = getMultiPolygon(getCoordsArray(obj));
        } else if (string == "GeometryCollection") {
            g = getGeometryCollection(getGeomsArray(obj));
        }
        else {
            throw "Property \"type\" has a value that is not supported or allowed";
        }
    }
    catch (...) {
        throw;
    }

    return g;
}

geos::geom::Point* GeoJSONReader::getPoint(Handle<Value> coords) {

    geos::geom::Coordinate coord = getCoordinate(coords, 0);

    return geometryFactory->createPoint(coord);
}

geos::geom::LineString* GeoJSONReader::getLineString(Handle<Value> coords) {

    if (coords->IsArray()) {

        return geometryFactory->createLineString(getCoordinates(coords));
    }


    return geometryFactory->createLineString();
}

geos::geom::Polygon* GeoJSONReader::getPolygon(Handle<Value> coords) {

    if (coords->IsArray()) {

        Handle<Array> array = Handle<Array>::Cast(coords);
        if (array->Length() == 0)
            throw "The number of the linear rings must be >= 1";


        geos::geom::LinearRing* shell = getLinearRing(array->Get(0));
        uint32_t length = array->Length();
        std::vector<geos::geom::Geometry*>* holes = new std::vector<geos::geom::Geometry*>();
        try {
            for (uint32_t i = 1; i < length; i++) {
                geos::geom::LinearRing* g = getLinearRing(array->Get(i));
                holes->push_back(g);
            }
        }
        catch (...) {
            delete shell;
            unsigned size = holes->size();
            for (unsigned int i = 0; i < size; i++)
                delete (*holes)[i];
            delete holes;
            throw;
        }


        return geometryFactory->createPolygon(shell, holes);
    }


    return geometryFactory->createPolygon();
}

geos::geom::MultiPoint* GeoJSONReader::getMultiPoint(Handle<Value> coords) {

    if (coords->IsArray()) {

        return geometryFactory->createMultiPoint(*getCoordinates(coords));
    }


    return geometryFactory->createMultiPoint();
}

geos::geom::MultiLineString* GeoJSONReader::getMultiLineString(Handle<Value> coords) {

    if (coords->IsArray()) {

        Handle<Array> array = Handle<Array>::Cast(coords);
        uint32_t length = array->Length();
        std::vector<geos::geom::Geometry*>* geoms = new std::vector<geos::geom::Geometry*>();
        try {
            for (uint32_t i = 0; i < length; i++) {
                geos::geom::LineString* g = getLineString(array->Get(i));
                geoms->push_back(g);
            }
        }
        catch (...) {
            unsigned size = geoms->size();
            for (unsigned int i = 0; i < size; i++)
                delete (*geoms)[i];
            delete geoms;
            throw;
        }


        return geometryFactory->createMultiLineString(geoms);
    }


    return geometryFactory->createMultiLineString();
}

geos::geom::MultiPolygon* GeoJSONReader::getMultiPolygon(Handle<Value> coords) {

    if (coords->IsArray()) {

        Handle<Array> array = Handle<Array>::Cast(coords);
        uint32_t length = array->Length();
        std::vector<geos::geom::Geometry*>* geoms = new std::vector<geos::geom::Geometry*>();
        try {
            for (uint32_t i = 0; i < length; i++) {
                geos::geom::Polygon* g = getPolygon(array->Get(i));
                geoms->push_back(g);
            }
        }
        catch (...) {
            unsigned size = geoms->size();
            for (unsigned int i = 0; i < size; i++)
                delete (*geoms)[i];
            delete geoms;
            throw;
        }


        return geometryFactory->createMultiPolygon(geoms);
    }


    return geometryFactory->createMultiPolygon();
}

geos::geom::GeometryCollection* GeoJSONReader::getGeometryCollection(Handle<Array> array) {

    uint32_t length = array->Length();
    std::vector<geos::geom::Geometry*>* geoms = new std::vector<geos::geom::Geometry*>();
    try {
        for (uint32_t i = 0; i < length; i++) {
            geos::geom::Geometry* g = read(array->Get(i));
            geoms->push_back(g);
        }
    }
    catch (...) {
        unsigned size = geoms->size();
        for (unsigned int i = 0; i < size; i++)
            delete (*geoms)[i];
        delete geoms;
        throw;
    }


    return geometryFactory->createGeometryCollection(geoms);
}
