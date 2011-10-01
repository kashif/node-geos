#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../geos").Geometry
WKTReader = (require "../geos").WKTReader
WKTWriter = (require "../geos").WKTWriter

tests = (vows.describe "Geometry").addBatch

  "A WKTWriter":
    topic: ->
      new WKTWriter()

    "a new instance should be an instance of WKTWriter": (writer) ->
      assert.instanceOf writer, WKTWriter

    "should have a write function": (writer) ->
      assert.isFunction writer.write

    "should write a Geometry to WKT": (writer) ->
      assert.equal (writer.write ((new WKTReader()).read "POINT(1 1)")), "POINT (1.0000000000000000 1.0000000000000000)"

tests.export module

