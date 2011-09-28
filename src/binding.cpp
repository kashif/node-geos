#include "binding.hpp"
#include "wktreader.hpp"
#include "geometry.hpp"

extern "C"
void init (Handle<Object> target) {
    HandleScope scope;

    target->Set(String::NewSymbol("geosversion"), String::NewSymbol(geos::geom::geosversion().data()));

    Geometry::Initialize(target);
    WKTReader::Initialize(target);
}
