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
    "should have a crosses function": (geom) ->
      assert.isFunction geom.crosses
    "should have a within function": (geom) ->
      assert.isFunction geom.within
    "should have a contains function": (geom) ->
      assert.isFunction geom.contains
    "should have a overlaps function": (geom) ->
      assert.isFunction geom.overlaps
    "should have a equals function": (geom) ->
      assert.isFunction geom.equals
      assert.ok geom.equals geom
    "should have a covers function": (geom) ->
      assert.isFunction geom.covers
    "should have a coveredBy function": (geom) ->
      assert.isFunction geom.coveredBy

tests.export module
