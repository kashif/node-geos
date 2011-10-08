#!/usr/bin/env coffee

WKTReader = (require "../geos").WKTReader
Quadtree = (require "../geos").Quadtree

tree = new Quadtree()
reader = new WKTReader()
rss = process.memoryUsage().rss
console.log process.memoryUsage()

for i in [1..100000]
  geom = reader.read "POINT(#{Math.random()} #{Math.random()})"
  tree.insert geom, 1


console.log tree.queryAll().length

console.log process.memoryUsage()
console.log "Memory in MBytes: " + (process.memoryUsage().rss - rss)/1024/1024
