#!/usr/bin/env coffee

WKTReader = (require "../lib").WKTReader

reader = new WKTReader()

geom = reader.read "POINT(1 1)"

# async call to geom.covers
geom.covers geom, (error, result) ->
  if error then console.log error
  else console.log result

# sync call to geom.covers
console.log geom.covers geom

# list of functions with async support
# isSimple
# isValid
# isEmpty
# isRectangle
# disjoint
# touches
# intersects
# crosses
# within
# contains
# overlaps
# equals
# covers
# coveredBy
