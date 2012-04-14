/*
GeoJSONReader class

Reads a GeoJSON (v1.0) object and returns a Geometry object (wrapped or unwrapped).
For the The GeoJSON Format Specification see http://geojson.org/geojson-spec.html

- Only geometry objects (no Feature(Collection)) are supported.
- The "coordinates" property of the passed GeoJSON object may be null to instantiate an empty geometry.
- The "geometries" property of a passed GeoJSON object with type GeometryCollection must be an array.
- The "crs" property is not handled. But you may pass a GeometryFactory instance to the constructor.
- 2/3-dimensional coordinates are supported.
*/


#ifndef GEOJSONREADER_HPP
#define GEOJSONREADER_HPP

#include <vector>
#include <string.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/PrecisionModel.h>
#include "binding.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"

class GeoJSONReader : public ObjectWrap {

    protected:
        const geos::geom::GeometryFactory* geometryFactory;
        const geos::geom::PrecisionModel* precisionModel;
        const geos::geom::CoordinateSequenceFactory* coordinateSequenceFactory;

    private:
        static Persistent<FunctionTemplate> constructor;

    public:
        GeoJSONReader();
        GeoJSONReader(const geos::geom::GeometryFactory* gf);
        ~GeoJSONReader();
        geos::geom::Geometry* read(Handle<Value> value);

        static void Initialize(Handle<Object> target);
        static Handle<Value> New(const Arguments& args);

    protected:
        Handle<Value> getCoordsArray(Handle<Object> geojson);
        Handle<Array> getGeomsArray(Handle<Object> geojson);
        geos::geom::Coordinate getCoordinate(Handle<Value> coords);
        geos::geom::Coordinate getCoordinate(Handle<Value> coords, bool acceptArrayOnly);
        geos::geom::CoordinateSequence* getCoordinates(Handle<Value> value);
        geos::geom::LinearRing* getLinearRing(Handle<Value> value);
        geos::geom::Point* getPoint(Handle<Value> coords);
        geos::geom::LineString* getLineString(Handle<Value> coords);
        geos::geom::Polygon* getPolygon(Handle<Value> coords);
        geos::geom::MultiPoint* getMultiPoint(Handle<Value> coords);
        geos::geom::MultiLineString* getMultiLineString(Handle<Value> coords);
        geos::geom::MultiPolygon* getMultiPolygon(Handle<Value> coords);
        geos::geom::GeometryCollection* getGeometryCollection(Handle<Array> array);

        static Handle<Value> Read(const Arguments& args);
        static double valueToDouble(Handle<Value> value);

};

#endif
