#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../geos").Geometry

tests = (vows.describe "Geometry").addBatch

  "A Geometry":
    topic: ->
      new Geometry()

    "a new instance should be an instance of Geometry": (geom) ->
      assert.ok geom instanceof Geometry

    "should have a toString function" : (geom) ->
      assert.isFunction geom.toString

tests.export module
