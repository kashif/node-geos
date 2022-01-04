#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

tests = (vows.describe "geos module").addBatch

  "The module":
    topic: ->
      require __dirname + "/../lib"

    "should have a geosversion": (module) ->
      assert.isString module.geosversion

    "should have a jtsport": (module) ->
      assert.isString module.jtsport

tests.export module
