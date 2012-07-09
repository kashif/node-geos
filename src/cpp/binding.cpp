#include "binding.hpp"
#include "wktreader.hpp"
#include "wktwriter.hpp"
#include "wkbwriter.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"
#include "precisionmodel.hpp"
#include "geojsonwriter.hpp"
#include "geojsonreader.hpp"

extern "C" {
    void init (Handle<Object> target) {
        HandleScope scope;

        target->Set(String::NewSymbol("geosversion"), String::NewSymbol(geos::geom::geosversion().data()));
        target->Set(String::NewSymbol("jtsport"), String::NewSymbol(geos::geom::jtsport().data()));

        Geometry::Initialize(target);
        WKTReader::Initialize(target);
        WKTWriter::Initialize(target);
        WKBWriter::Initialize(target);
        GeometryFactory::Initialize(target);
        PrecisionModel::Initialize(target);
        GeoJSONWriter::Initialize(target);
        GeoJSONReader::Initialize(target);
    }

    NODE_MODULE(geos, init)
}
