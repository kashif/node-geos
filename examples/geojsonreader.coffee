#!/usr/bin/env coffee

GeoJSONReader = (require "../lib").GeoJSONReader

reader = new GeoJSONReader

geom = reader.read( type: 'Point', coordinates: [ 1, 2 ] )

console.log('' + geom)