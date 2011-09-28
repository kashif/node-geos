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

    "should have a equals function": (geom) ->
      assert.isFunction geom.equals
      assert.ok geom.equals geom

tests.export module
