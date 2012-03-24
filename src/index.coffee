_geos = require("../build/Release/geos")
exports.WKTReader = _geos.WKTReader
exports.WKTWriter = _geos.WKTWriter
_Geometry = _geos.Geometry
exports.GeometryFactory = _geos.GeometryFactory
exports.PrecisionModel = _geos.PrecisionModel
exports.Quadtree = _geos.Quadtree
exports.geosversion = _geos.geosversion
exports.jtsport = _geos.jtsport

Geometry = ->
  throw new Error "Do not call Geometry constructor directly. Use WKTReader.read instead."

Geometry.prototype= _Geometry.prototype

exports.Geometry = Geometry
