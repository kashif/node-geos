#!/usr/bin/env coffee

WKTReader = (require "../lib").WKTReader
GeoJSONWriter = (require "../lib").GeoJSONWriter

reader = new WKTReader()
writer = new GeoJSONWriter()

geom = reader.read "POINT(23.5 42.1)"

console.log geom.toGeoJSON()

console.log geom.toJSON()

# Additionally, you can pass roundingPrecision and bbox as separate arguments

console.log geom.toJSON(0, true)

# or use the GeoJSONWriter

console.log writer.write geom

# return a BBox as well on every write
writer.setBbox true

console.log writer.write geom

# use to decimals
writer.setRoundingPrecision 2

console.log writer.write reader.read "POINT(0.1234 5.6789)"

# get bbox as a separate object

console.log writer.writeBbox geom
