#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../lib").WKTReader
Geometry = (require "../lib").Geometry
GeometryFactory = (require "../lib").GeometryFactory
PrecisionModel = (require "../lib").PrecisionModel

tests = (vows.describe "GeometryFactory").addBatch

  "A GeometryFactory":
    topic: ->
      new GeometryFactory()

    "a new instance should be an instance of GeometryFactory": (factory) ->
      assert.instanceOf factory, GeometryFactory

  "A GeometryFactory with different PrecisionModel":
    topic: ->
      new GeometryFactory(new PrecisionModel())

    "a new instance should be an instance of GeometryFactory": (factory) ->
      assert.instanceOf factory, GeometryFactory

# TODO segfault/see explicit cast in c++ code
#    "the getPrecisionModel should return a valid PrecisionModel": (factory) ->
#      assert.isFunction factory.getPrecisionModel
#      assert.equal factory.getPrecisionModel().comapreTo(new PrecisionModel())

  "A GeometryFactory with different PrecisionModel and new SRID":
    topic: ->
      new GeometryFactory(new PrecisionModel(), 4326)

    "a new instance should be an instance of GeometryFactory": (factory) ->
      assert.instanceOf factory, GeometryFactory

    "its SRID should be 4326": (factory) ->
      assert.isFunction factory.getSRID
      assert.equal factory.getSRID(), 4326

tests.export module
