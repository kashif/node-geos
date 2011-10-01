#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../geos").WKTReader
Geometry = (require "../geos").Geometry
GeometryFactory = (require "../geos").GeometryFactory

tests = (vows.describe "GeometryFactory").addBatch

  "A GeometryFactory":
    topic: ->
      new GeometryFactory()

    "a new instance should be an instance of GeometryFactory": (factory) ->
      assert.instanceOf factory, GeometryFactory

tests.export module
