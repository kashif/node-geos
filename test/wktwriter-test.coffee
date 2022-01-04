#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../lib").Geometry
WKTReader = (require "../lib").WKTReader
WKTWriter = (require "../lib").WKTWriter

reader = new WKTReader()

tests = (vows.describe "Geometry").addBatch

  "A WKTWriter":
    topic: ->
      new WKTWriter()

    "a new instance should be an instance of WKTWriter": (writer) ->
      assert.instanceOf writer, WKTWriter

    "should have a write function": (writer) ->
      assert.isFunction writer.write

    "should write a Geometry to WKT": (writer) ->
      assert.equal (writer.write reader.read "POINT(1 1)"), "POINT (1.0000000000000000 1.0000000000000000)"

    "should have a setRoundingPrecision function": (writer) ->
      assert.isFunction writer.setRoundingPrecision

    "should round to the right precision": (writer) ->
      writer.setRoundingPrecision 1
      wkt = writer.write reader.read "POINT(0.1234 5.6789)"
      assert.equal wkt, "POINT (0.1 5.7)"
      writer.setRoundingPrecision -1
      wkt = writer.write reader.read "POINT(1 1)"
      assert.equal wkt, "POINT (1.0000000000000000 1.0000000000000000)"

    "should throw an error on invalid input for setRoundingPrecision": (writer) ->
      fn = ->
        writer.setRoundingPrecision undefined
      assert.throws fn, Error

    "should have a setTrim function": (writer) ->
      assert.isFunction writer.setTrim

    "should trim zeros": (writer) ->
      writer.setTrim true
      wkt = writer.write reader.read "POINT (1.0000 1.0000)"
      assert.equal wkt, "POINT (1 1)"

    "should throw an error on invalid input for setTrim": (writer) ->
      fn = ->
        writer.setTrim undefined
      assert.throws fn, Error

tests.export module
