#include "binding.hpp"
#include "wktreader.hpp"
#include "wktwriter.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"
#include "precisionmodel.hpp"

extern "C"
void init (Handle<Object> target) {
    HandleScope scope;

    target->Set(String::NewSymbol("geosversion"), String::NewSymbol(geos::geom::geosversion().data()));

    Geometry::Initialize(target);
    WKTReader::Initialize(target);
    WKTWriter::Initialize(target);
    GeometryFactory::Initialize(target);
    PrecisionModel::Initialize(target);

}
