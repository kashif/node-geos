#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

WKTReader = (require "../geos").WKTReader
Geometry = (require "../geos").Geometry
Quadtree = (require "../geos").Quadtree

geom = (new WKTReader()).read "POINT (1 1)"
item = null

tests = (vows.describe "Quadtree").addBatch

  "A Quadtree":
    topic: ->
      new Quadtree()

    "a new instance should be an instance of Quadtree": (tree) ->
      assert.instanceOf tree, Quadtree

    "should have a insert function": (tree) ->
      assert.isFunction tree.insert
      assert.isUndefined tree.insert geom, 1

    "should have a queryAll function": (tree) ->
      assert.isFunction tree.queryAll
      r = tree.queryAll()
      assert.isArray r
      assert.equal r[0], 1

    "shoudl have a query function": (tree) ->
      assert.isFunction tree.query
      r = tree.query geom
      assert.isArray r
      assert.equal r[0], 1

    "should have a remove function": (tree) ->
      assert.isFunction tree.remove
      assert.isTrue tree.remove geom, 1

    "should handle big integers": (tree) ->
      #max key size is Math.pow 2, 30
      tree.insert geom, Math.pow 2, 30
      assert.deepEqual tree.queryAll(), [Math.pow 2, 30]



tests.export module
