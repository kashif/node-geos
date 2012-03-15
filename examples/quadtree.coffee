#!/usr/bin/env coffee

WKTReader = (require "../geos").WKTReader
Quadtree = (require "../geos").Quadtree

tree = new Quadtree()
reader = new WKTReader()
rss = process.memoryUsage().rss
console.log process.memoryUsage()

geometries = []

for i in [1..100000]
  geom = reader.read "POINT(#{Math.floor Math.random()*100} #{Math.floor Math.random()*100})"
  tree.insert geom, i
  geometries[i] = geom



console.log tree.queryAll().length

bbox = reader.read "POLYGON((1.12345678 1, 1.12345678 2, 2 2, 2 1, 1.12345678 1))"

# Give Geometry::toJSON a new default: max 7 digits, includes bbox
g = require("../geos").Geometry
toJSON = g::toJSON
g::toJSON = -> toJSON.call(@, 7, on)
console.log(JSON.stringify(bbox))

r = tree.query bbox

console.log r

for i in r
  if !geometries[i]?
  	console.log(i, 'is undefined')
  else
    console.log geometries[i].toString()

console.log process.memoryUsage()
console.log "Memory in MBytes: " + (process.memoryUsage().rss - rss)/1024/1024

