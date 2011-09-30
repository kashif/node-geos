#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../geos").Geometry
WKTReader = (require "../geos").WKTReader

tests = (vows.describe "Geometry").addBatch

  "A Geometry":
    topic: ->
      (new WKTReader()).read "POINT(1 1)"

    "a new instance should be an instance of Geometry": (geom) ->
      assert.ok geom instanceof Geometry

    "should have a toString function" : (geom) ->
      assert.isFunction geom.toString
      assert.equal geom.toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    # geos unary predicates
    "should have a isSimple function": (geom) ->
      assert.isFunction geom.isSimple
    "should have a isValid function": (geom) ->
      assert.isFunction geom.isValid
    "should have a isEmpty function": (geom) ->
      assert.isFunction geom.isEmpty
    "should have a isRectangle function": (geom) ->
      assert.isFunction geom.isRectangle

    # geos binary predicates
    "should have a disjoint function": (geom) ->
      assert.isFunction geom.disjoint
      assert.ok not geom.disjoint geom
    "should have a touches function": (geom) ->
      assert.isFunction geom.touches
      assert.ok not geom.touches geom
    "should have a intersects function": (geom) ->
      assert.isFunction geom.intersects
      assert.ok not geom.disjoint geom
    "should have a crosses function": (geom) ->
      assert.isFunction geom.crosses
      assert.ok not geom.disjoint geom
    "should have a within function": (geom) ->
      assert.isFunction geom.within
      assert.ok not geom.disjoint geom
    "should have a contains function": (geom) ->
      assert.isFunction geom.contains
      assert.ok not geom.disjoint geom
    "should have a overlaps function": (geom) ->
      assert.isFunction geom.overlaps
      assert.ok not geom.disjoint geom
    "should have a equals function": (geom) ->
      assert.isFunction geom.equals
      assert.ok geom.equals geom
    "should have a covers function": (geom) ->
      assert.isFunction geom.covers
      assert.ok not geom.disjoint geom
    "should have a coveredBy function": (geom) ->
      assert.isFunction geom.coveredBy
      assert.ok not geom.disjoint geom

    #topologic functions
    "should have a intersection function": (geom) ->
      assert.isFunction geom.intersection
      assert.ok (geom.intersection geom).equals geom
    #"should have a union function": (geom) ->
    #  assert.isFunction geom.union
    "should have a difference function": (geom) ->
      assert.isFunction geom.difference
      #TODO use real geometries
      assert.equal (geom.difference geom).toString(), "GEOMETRYCOLLECTION EMPTY"
    "should have a symDifference function": (geom) ->
      assert.isFunction geom.symDifference
      assert.equal (geom.symDifference geom).toString(), "GEOMETRYCOLLECTION EMPTY"

    "should have a distance function": (geom) ->
      assert.isFunction geom.distance
      assert.equal (geom.distance geom), 0

    "should have a distance function": (geom) ->
      assert.isFunction geom.distance
      assert.equal (geom.distance geom), 0

    "should have a isWithinDistance": (geom) ->
      assert.isFunction geom.isWithinDistance
      assert.ok geom.isWithinDistance geom, 10

    "should have a getEnvelope function": (geom) ->
      assert.isFunction geom.getEnvelope
      assert.equal geom.getEnvelope().toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    "should return a vaild envelope for a linestring": (geom) ->
      geom = (new WKTReader()).read "LINESTRING (0 0, 1 1)"
      #TODO remove this ugly wkt
      assert.equal geom.getEnvelope().toString(), "POLYGON ((0.0000000000000000 0.0000000000000000, 1.0000000000000000 0.0000000000000000, 1.0000000000000000 1.0000000000000000, 0.0000000000000000 1.0000000000000000, 0.0000000000000000 0.0000000000000000))"

tests.export module
