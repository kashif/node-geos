{
  _Geometry
  WKTWriter
  GeoJSONWriter
} = exports = module.exports = require("../build/Release/geos")

_Geometry = exports.Geometry

Geometry = ->
  throw new Error "Do not call Geometry constructor directly. Use WKTReader.read instead."

Geometry.prototype = _Geometry.prototype

Geometry.prototype.toGeoJSON = Geometry.prototype.toJSON

exports.Geometry = Geometry


_setRoundingPrecision = WKTWriter.prototype.setRoundingPrecision

WKTWriter.prototype.setRoundingPrecision = (decimals) ->
  if not (typeof decimals == "number") then throw new Error "TypeError: input must be of type number (int)"
  else
    _setRoundingPrecision.call @, decimals


_write = GeoJSONWriter.prototype.write

GeoJSONWriter.prototype.write = (geom) ->
  if not (geom instanceof Geometry) then throw new Error "TypeError: input (geom) must be of type Geometry."
  else
    _write.call @, geom

_writeBbox = GeoJSONWriter.prototype.writeBbox

GeoJSONWriter.prototype.writeBbox = (geom) ->
  if not (geom instanceof Geometry) then throw new Error "TypeError: input (geom) must be of type Geometry."
  else
    _writeBbox.call @, geom

_setBbox = GeoJSONWriter.prototype.setBbox

GeoJSONWriter.prototype.setBbox = (b) ->
  if not (typeof b == "boolean") then throw new Error "TypeError: input must be of type Boolean"
  else
    _setBbox.call @, b

_setRoundingPrecision = GeoJSONWriter.prototype.setRoundingPrecision

GeoJSONWriter.prototype.setRoundingPrecision = (decimals) ->
  if not (typeof decimals == "number") then throw new Error "TypeError: input must be of type Number (int)"
  else
    _setRoundingPrecision.call @, decimals