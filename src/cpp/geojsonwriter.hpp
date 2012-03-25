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
#include "geometry.hpp"

class GeoJSONWriter : public ObjectWrap{

    /*public:*/

    protected:
        int decimalPlaces;
        double factor;
        bool bbox;

    private:
        static Persistent<FunctionTemplate> constructor;

    public:
        GeoJSONWriter();
        ~GeoJSONWriter();
        void setRoundingPrecision(int places);
        void setBbox(bool bbox);
        Handle<Value> write(const geos::geom::Geometry* geom);
        Handle<Value> writeBbox(const geos::geom::Geometry* geom);

        static void Initialize(Handle<Object> target);
        static Handle<Value> New(const Arguments& args);

    protected:
        double roundNumber(double coord);
        Handle<Array> coordinateToArray(const geos::geom::Coordinate* coord);
        Handle<Array> coordinateSequenceToArray(const geos::geom::CoordinateSequence* seq);
        Handle<Array> geometryCollectionToArrayOfArrays(const geos::geom::GeometryCollection* geom);
        Handle<Array> geometryCollectionToArrayOfObjects(const geos::geom::GeometryCollection* geom);
        Handle<Value> getCoordsOrGeom(const geos::geom::Geometry* geom);

        static Handle<Value> SetRoundingPrecision(const Arguments& args);
        static Handle<Value> SetBbox(const Arguments& args);
        static Handle<Value> Write(const Arguments& args);
        static Handle<Value> WriteBbox(const Arguments& args);

};

#endif
