#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../geos").WKTReader
Geometry = (require "../geos").Geometry
Quadtree = (require "../geos").Quadtree

tests = (vows.describe "Quadtree").addBatch

  "A Quadtree":
    topic: ->
      new Quadtree()

    "a new instance should be an instance of Quadtree": (tree) ->
      assert.instanceOf tree, Quadtree

    "should have a insert function": (tree) ->
      geom = (new WKTReader()).read "POINT (1 1)"
      assert.isFunction tree.insert
      assert.isUndefined tree.insert geom, "key"

    "should have a queryAll function": (tree) ->
      assert.isFunction tree.queryAll
      r = tree.queryAll()
      assert.isArray r
      assert.instanceOf r[0].geom, Geometry
      assert.equal r[0].geom.toString(), "POINT (1.0000000000000000 1.0000000000000000)"
      assert.equal r[0].key, "key"

tests.export module
