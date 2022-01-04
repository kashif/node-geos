#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKBReader = (require "../lib").WKBReader
Geometry = (require "../lib").Geometry
GeometryFactory = (require "../lib").GeometryFactory
PrecisionModel = (require "../lib").PrecisionModel

tests = (vows.describe "WKBReader").addBatch

  "A WKBReader":
    topic: ->
      new WKBReader()

    "a new instance should be an instance of WKBReader": (reader) ->
      assert.instanceOf reader, WKBReader

    "should have a read function": (reader) ->
      assert.isFunction reader.readHEX

    "should read a WKB POINT(1 1)": (reader) ->
      geom = reader.readHEX "0101000000000000000000F03F000000000000F03F"
      assert.instanceOf geom, Geometry
      assert.equal geom.toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    "should throw an exception on undefined input": (reader) ->
      fn = ->
        reader.readHEX()
      assert.throws fn, Error

    "should throw another exception on invalid WKB": (reader) ->
      fn = ->
        reader.readHEX "010000000000000000000000000000000000000000"
      assert.throws fn, Error

    "should read a wellformed WKB which represents a invalid geometry": (reader) ->
      multipolygon = "01060000000300000001030000000200000005000000000000000000F03F000000000000F03F0000000000001440000000000000F03F00000000000014400000000000001440000000000000F03F0000000000001440000000000000F03F000000000000F03F050000000000000000000040000000000000004000000000000008400000000000000040000000000000084000000000000008400000000000000040000000000000084000000000000000400000000000000040010300000001000000040000000000000000000840000000000000084000000000000018400000000000000040000000000000184000000000000010400000000000000840000000000000084001030000000200000004000000000000000080534000000000008046400000000000405040000000000000414000000000000059400000000000004B400000000000805340000000000080464005000000000000000000104000000000004050400000000000004B40000000000000374000000000000081400000000000A075400000000000000040000000000000F03F00000000000010400000000000405040"
      geom = reader.readHEX multipolygon
      assert.isFalse geom.isValid()

    "should read an EMPTY Geometry": (reader) ->
      geom = reader.readHEX "010700000000000000"
      assert.equal geom.toString(), "GEOMETRYCOLLECTION EMPTY"
      assert.isTrue geom.isEmpty()

    "should throw an Error on read with empty string": (reader) ->
      fn = ->
        reader.readHEX ""
      assert.throws fn, Error

    "should throw an Error on read without any input": (reader) ->
      fn = ->
        reader.readHEX()
      assert.throws fn, Error

tests.export module
