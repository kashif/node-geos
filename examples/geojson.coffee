#!/usr/bin/env coffee

WKTReader = (require "../lib").WKTReader

reader = new WKTReader()

geom = reader.read "POINT(23 42)"

console.log geom.toGeoJSON()

console.log geom.toJSON()
