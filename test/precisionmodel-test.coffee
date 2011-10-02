#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

PrecisionModel = (require "../geos").PrecisionModel

tests = (vows.describe "PrecisionModel").addBatch

  "A PrecisionModel with default precision (FLOATING)":
    topic: ->
      new PrecisionModel()

    "a new instance should be an instance of PrecisionModel": (model) ->
      assert.instanceOf model, PrecisionModel

    "should have getType function": (model) ->
      assert.isFunction model.getType

    "a call to getType should return FLOATING (1)": (model) ->
      assert.equal model.getType(), 1

    "getScale should return a valid scale factor": (model) ->
      assert.isFunction model.getScale
      assert.equal model.getScale(), 1

    "getOffsetX should return a valid x-offset": (model) ->
      assert.isFunction model.getOffsetX
      assert.equal model.getOffsetX(), 0

    "getOffsetY should return a valid y-offset": (model) ->
      assert.isFunction model.getOffsetY
      assert.equal model.getOffsetY(), 0

  "A PrecisionModel with precision FIXED":
    topic: ->
      new PrecisionModel("FIXED")

    "it should have type equal to FIXED": (model) ->
      assert.equal model.getType(), 0

  "A PrecisionModel with precision SINGLE_FLOATING":
    topic: ->
      new PrecisionModel("SINGLE_FLOATING")

    "it should have type equal to SINGLE_FLOATING": (model) ->
      assert.equal model.getType(), 2

  "A PrecisionModel with a different scale factor":
    topic: ->
      new PrecisionModel(10)

    "it should have a different scale factor": (model) ->
      assert.equal model.getScale(), 10
      assert.equal model.getType(), 0

tests.export module
