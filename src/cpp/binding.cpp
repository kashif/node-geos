#include "binding.hpp"
#include "wktreader.hpp"
#include "wktwriter.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"
#include "precisionmodel.hpp"
#include "quadtree.hpp"
#include "geojsonwriter.hpp"

extern "C"
void init (Handle<Object> target) {
    HandleScope scope;

    target->Set(String::NewSymbol("geosversion"), String::NewSymbol(geos::geom::geosversion().data()));
    target->Set(String::NewSymbol("jtsport"), String::NewSymbol(geos::geom::jtsport().data()));

    Geometry::Initialize(target);
    WKTReader::Initialize(target);
    WKTWriter::Initialize(target);
    GeometryFactory::Initialize(target);
    PrecisionModel::Initialize(target);
    Quadtree::Initialize(target);
    GeoJSONWriter::Initialize(target);

}
