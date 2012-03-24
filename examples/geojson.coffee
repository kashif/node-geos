#!/usr/bin/env coffee

WKTReader = (require "../lib").WKTReader
GeoJSONWriter = (require "../lib").GeoJSONWriter

reader = new WKTReader()
writer = new GeoJSONWriter()

geom = reader.read "POINT(23 42)"

console.log geom.toGeoJSON()

console.log geom.toJSON()

# or use the GeoJSONWriter

console.log writer.write geom

# return a BBox as well on every write
writer.setBBox true

console.log writer.write geom

# use to decimals
writer.setRoundingPrecision 2

console.log writer.write reader.read "POINT(0.1234 5.6789)"
