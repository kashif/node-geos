_geos = require("../build/Release/geos")
exports.WKTReader = _geos.WKTReader
WKTWriter = _geos.WKTWriter
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

_setRoudingPrecision = WKTWriter.prototype.setRoundingPrecision

WKTWriter.prototype.setRoundingPrecision = (decimals) ->
  if not (typeof decimals == "number") then throw new Error "TypeError: input must be of type number (int)"
  else
    _setRoudingPrecision.call @, decimals

exports.WKTWriter = WKTWriter
