#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

PrecisionModel = (require "../geos").PrecisionModel

tests = (vows.describe "PrecisionModel").addBatch

  "A PrecisionModel":
    topic: ->
      new PrecisionModel()

    "a new instance should be an instance of PrecisionModel": (model) ->
      assert.instanceOf model, PrecisionModel

tests.export module
