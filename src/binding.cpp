#include "binding.hpp"
#include "wktreader.hpp"
#include "geometry.hpp"

extern "C"
void init (Handle<Object> target) {
  HandleScope scope;
  Geometry::Initialize(target);
  WKTReader::Initialize(target);
}
