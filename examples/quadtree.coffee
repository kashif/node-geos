#!/usr/bin/env coffee

WKTReader = (require "../lib").WKTReader
Quadtree = (require "../lib").Quadtree

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

bbox = reader.read "POLYGON((1 1, 1 2, 2 2, 2 1, 1 1))"

r = tree.query bbox

console.log r

for i in r
  console.log geometries[i].toString()

console.log process.memoryUsage()
console.log "Memory in MBytes: " + (process.memoryUsage().rss - rss)/1024/1024

