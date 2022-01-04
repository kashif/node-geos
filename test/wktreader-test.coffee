#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../lib").WKTReader
Geometry = (require "../lib").Geometry
GeometryFactory = (require "../lib").GeometryFactory
PrecisionModel = (require "../lib").PrecisionModel

tests = (vows.describe "WKTReader").addBatch

  "A WKTReader":
    topic: ->
      new WKTReader()

    "a new instance should be an instance of WKTReader": (reader) ->
      assert.instanceOf reader, WKTReader

    "should have a read function": (reader) ->
      assert.isFunction reader.read

    "should read a WKT POINT(1 1)": (reader) ->
      geom = reader.read "POINT(1 1)"
      assert.instanceOf geom, Geometry
      assert.equal geom.toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    "should throw an exception on undefined input": (reader) ->
      fn = ->
        reader.read()
      assert.throws fn, Error

    "should throw another exception on invalid WKT": (reader) ->
      fn = ->
        reader.read "POLYGON((0 0))"
      assert.throws fn, Error

    "should read a wellformed WKT which represents a invalid geometry": (reader) ->
      multipolygon = "MULTIPOLYGON(((1 1,5 1,5 5,1 5,1 1),(2 2, 3 2, 3 3, 2 3,2 2)),
        ((3 3,6 2,6 4,3 3)),
        ((78 45,65 34,100 54,78 45),(4 65, 54 23, 544 346, 2 1, 4 65)))"
      geom = reader.read multipolygon
      assert.isFalse geom.isValid()

    "should read an EMPTY Geometry": (reader) ->
      geom = reader.read "POLYGON EMPTY"
      assert.equal geom.toString(), "POLYGON EMPTY"
      assert.isTrue geom.isEmpty()

    "should throw an Error on read with empty string": (reader) ->
      fn = ->
        reader.read ""
      assert.throws fn, Error

    "should throw an Error on read without any input": (reader) ->
      fn = ->
        reader.read()
      assert.throws fn, Error

  "A WKTReader with non standard GeometryFactory":
    topic: ->
      new WKTReader new GeometryFactory new PrecisionModel "FIXED"

    "a new instance should be an instance of WKTReader": (reader) ->
      assert.instanceOf reader, WKTReader

    "should have a read function": (reader) ->
      assert.isFunction reader.read

    "should read a WKT POINT(1 1)": (reader) ->
      geom = reader.read "POINT(1 1)"
      assert.instanceOf geom, Geometry
      assert.equal geom.toString(), "POINT (1 1)"


tests.export module
