#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../lib").Geometry
WKTReader = (require "../lib").WKTReader
WKBWriter = (require "../lib").WKBWriter

reader = new WKTReader()

tests = (vows.describe "Geometry").addBatch

  "A WKTWriter":
    topic: ->
      new WKBWriter()

    "a new instance should be an instance of WKTWriter": (writer) ->
      assert.instanceOf writer, WKBWriter

    "should have a writeHEX function": (writer) ->
      assert.isFunction writer.writeHEX

    "should write a Polygon to WKB in hex format": (writer) ->
      assert.equal (writer.writeHEX reader.read "POINT(1 2)"),
        "0101000000000000000000F03F0000000000000040"

tests.export module
