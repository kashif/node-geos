#ifndef GEOJSONWRITER_HPP
#define GEOJSONWRITER_HPP

#include <math.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include "binding.hpp"

class GeoJSONWriter {

    public:
        bool bbox;

    protected:
        int decimalPlaces;
        double factor;

    public:
        GeoJSONWriter();
        ~GeoJSONWriter();
        void setRoundingPrecision (int places);
        Handle<Object> write(const geos::geom::Geometry* geom);

    protected:
        double roundNumber(double coord);
        Handle<Array> coordinateToArray(const geos::geom::Coordinate* coord);
        Handle<Array> coordinateSequenceToArray(const geos::geom::CoordinateSequence* seq);
        Handle<Array> geometryCollectionToArrayOfArrays(const geos::geom::GeometryCollection* geom);
        Handle<Array> geometryCollectionToArrayOfObjects(const geos::geom::GeometryCollection* geom);
        Handle<Value> getCoordsOrGeom(const geos::geom::Geometry* geom);
        Handle<Value> getBbox(const geos::geom::Geometry* geom);

};

#endif
