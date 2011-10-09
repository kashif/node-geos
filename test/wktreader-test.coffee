#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../geos").WKTReader
Geometry = (require "../geos").Geometry

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

    "should throw an exception on malformed WKT": (reader) ->
      assert.throws reader.read, Error

tests.export module
