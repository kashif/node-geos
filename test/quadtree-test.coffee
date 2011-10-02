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

tests.export module
