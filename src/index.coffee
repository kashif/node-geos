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

Geometry.prototype.toGeoJSON = Geometry.prototype.toJSON

exports.Geometry = Geometry

GeoJSONWriter = _geos.GeoJSONWriter

_write = GeoJSONWriter.prototype.write

GeoJSONWriter.prototype.write = (geom) ->
  if not (geom instanceof Geometry) then throw new Error "TypeError: input (geom) must be of type Geometry."
  else
    _write.call @, geom

_setBBox = GeoJSONWriter.prototype.setBBox

GeoJSONWriter.prototype.setBBox = (b) ->
  if not (typeof b == "boolean") then throw new Error "TypeError: input must be of type Boolean"
  else
    _setBBox.call @, b

_setRoundingPrecision = GeoJSONWriter.prototype.setRoundingPrecision

GeoJSONWriter.prototype.setRoundingPrecision = (decimals) ->
  if not (typeof decimals == "number") then throw new Error "TypeError: input must be of type Number (int)"
  else
    _setRoundingPrecision.call @, decimals

exports.GeoJSONWriter = GeoJSONWriter
