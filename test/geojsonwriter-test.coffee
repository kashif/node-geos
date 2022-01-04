#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../lib").Geometry
WKTReader = (require "../lib").WKTReader
GeoJSONWriter = (require "../lib").GeoJSONWriter

reader = new WKTReader()

tests = (vows.describe "GeoJSONWriter").addBatch

  "An empty geometry JSON":
    topic: ->
      (reader.read("POINT EMPTY")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'Point'

    "has undefined coordinates and bbox": (json) ->
      assert.isNull json.coordinates
      assert.isNull json.bbox

  "An empty geometry collection JSON":
    topic: ->
      (reader.read("GEOMETRYCOLLECTION EMPTY")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'GeometryCollection'

    "has correct geometries and bbox": (json) ->
      assert.deepEqual json.geometries, []
      assert.isNull json.bbox

  "A geometry JSON":
    topic: ->
      (reader.read("POINT(-179.12345678 179.98765432)")).toJSON(7, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'Point'

    "has correct coordinate rounding": (json) ->
      assert.deepEqual json.coordinates, [ -179.1234568, 179.9876543 ]
      assert.deepEqual json.bbox, [ -179.1234568, 179.9876543, -179.1234568, 179.9876543 ]

  "A geometry JSON without bbox":
    topic: ->
      (reader.read("POINT(-179.12345678 179.98765432)")).toJSON(0, no)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'Point'

    "has correct coordinate rounding": (json) ->
      assert.deepEqual json.coordinates, [ -179, 180 ]
      assert.deepEqual json.bbox, undefined

  "A LineString JSON":
    topic: ->
      (reader.read("LINESTRING(0 0, 1 1, 1 3)")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'LineString'

    "has correct coordinates and bbox": (json) ->
      assert.deepEqual json.coordinates, [ [ 0, 0 ], [ 1, 1 ], [ 1, 3 ] ]
      assert.deepEqual json.bbox, [ 0, 0, 1, 3 ]

  "A Polygon JSON":
    topic: ->
      (reader.read("POLYGON((0 0, 5 0, 5 5, 0 5, 0 0),(1 1, 2 1, 2 2, 1 2, 1 1),(3 3, 4 3, 4 4, 3 4, 3 3))")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'Polygon'

    "has correct coordinates and bbox": (json) ->
      assert.deepEqual json.coordinates, [
        [ [ 0, 0 ], [ 5, 0 ], [ 5, 5 ], [ 0, 5 ], [ 0, 0 ] ]
        [ [ 1, 1 ], [ 2, 1 ], [ 2, 2 ], [ 1, 2 ], [ 1, 1 ] ]
        [ [ 3, 3 ], [ 4, 3 ], [ 4, 4 ], [ 3, 4 ], [ 3, 3 ] ]
      ]
      assert.deepEqual json.bbox, [ 0, 0, 5, 5 ]

  "A MultiPoint JSON":
    topic: ->
      (reader.read("MULTIPOINT(0 0, 1 1, 1 3, 0 0)")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'MultiPoint'

    "has correct coordinates and bbox": (json) ->
      assert.deepEqual json.coordinates, [ [ 0, 0 ], [ 1, 1 ], [ 1, 3 ], [ 0, 0 ] ]
      assert.deepEqual json.bbox, [ 0, 0, 1, 3 ]

  "A MultiLineString JSON":
    topic: ->
      (reader.read("MULTILINESTRING((0 0, 1 1, 1 3),(2 2, 5 5, 1 3))")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'MultiLineString'

    "has correct coordinates and bbox": (json) ->
      assert.deepEqual json.coordinates, [
        [ [ 0, 0 ], [ 1, 1 ], [ 1, 3 ] ]
        [ [ 2, 2 ], [ 5, 5 ], [ 1, 3 ] ]
      ]
      assert.deepEqual json.bbox, [ 0, 0, 5, 5 ]

  "A MultiPolygon JSON":
    topic: ->
      (reader.read("MULTIPOLYGON(((0 0, 5 0, 5 5, 0 5, 0 0),(1 1, 2 1, 2 2, 1 2, 1 1),(3 3, 4 3, 4 4, 3 4, 3 3)),((0 0, 1 1, 0 1, 0 0)))")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'MultiPolygon'

    "has correct coordinates and bbox": (json) ->
      assert.deepEqual json.coordinates, [
        [
          [ [ 0, 0 ], [ 5, 0 ], [ 5, 5 ], [ 0, 5 ], [ 0, 0 ] ]
          [ [ 1, 1 ], [ 2, 1 ], [ 2, 2 ], [ 1, 2 ], [ 1, 1 ] ]
          [ [ 3, 3 ], [ 4, 3 ], [ 4, 4 ], [ 3, 4 ], [ 3, 3 ] ]
        ]
        [
          [ [ 0, 0 ], [ 1, 1 ], [ 0, 1 ], [ 0, 0 ] ]
        ]
      ]
      assert.deepEqual json.bbox, [ 0, 0, 5, 5 ]

  "A GeometryCollection JSON":
    topic: ->
      (reader.read("GEOMETRYCOLLECTION(POLYGON((0 0, 5 0, 5 5, 0 5, 0 0),(1 1, 2 1, 2 2, 1 2, 1 1),(3 3, 4 3, 4 4, 3 4, 3 3)),MULTILINESTRING((0 0, 1 1, 1 3),(2 2, 5 5, 1 3)))")).toJSON(-1, on)

    "is an object with correct type": (json) ->
      assert.equal json.type, 'GeometryCollection'

    "is correct": (json) ->
      assert.deepEqual json, {
        type: 'GeometryCollection'
        geometries: [
          {
            type: 'Polygon'
            coordinates: [
              [ [ 0, 0 ], [ 5, 0 ], [ 5, 5 ], [ 0, 5 ], [ 0, 0 ] ]
              [ [ 1, 1 ], [ 2, 1 ], [ 2, 2 ], [ 1, 2 ], [ 1, 1 ] ]
              [ [ 3, 3 ], [ 4, 3 ], [ 4, 4 ], [ 3, 4 ], [ 3, 3 ] ]
            ]
            bbox: [ 0, 0, 5, 5 ]
          }
          {
            type: 'MultiLineString'
            coordinates: [
              [ [ 0, 0 ], [ 1, 1 ], [ 1, 3 ] ]
              [ [ 2, 2 ], [ 5, 5 ], [ 1, 3 ] ]
            ]
            bbox: [ 0, 0, 5, 5 ]
          }
        ]
        bbox: [ 0, 0, 5, 5 ]
      }

  "A new GeoJSONWriter":
    topic: ->
      new GeoJSONWriter()

    "is an instance of GeoJSONWriter": (writer) ->
      assert.instanceOf writer, GeoJSONWriter

    "returns valid GeoJSON": (writer) ->
      assert.deepEqual writer.write(reader.read("POINT(1.0 1)")), {
        type: "Point"
        coordinates: [1.0,1]
      }

    "throws an error on invalid write input": (writer) ->
      fn = ->
        writer.write undefined
      assert.throws fn, Error

    "has a setRoundingPrecision function": (writer) ->
      assert.isFunction writer.setRoundingPrecision

    "rounds correct": (writer) ->
      writer.setRoundingPrecision 0
      assert.deepEqual (writer.write(reader.read("POINT(1.2 2.3)"))).coordinates, [1, 2]

    "throws an error on invalid input for setRoundingPrecision": (writer) ->
      fn = ->
        writer.setRoundingPrecision undefined
      assert.throws fn, Error

    "has a setBbox function": (writer) ->
      assert.isFunction writer.setBbox

    "has a working setBbox function": (writer) ->
      writer.setBbox true
      assert.deepEqual (writer.write(reader.read("POINT(1 1)"))).bbox, [1,1,1,1]

    "throws an error on invalid input for setBBox": (writer) ->
      fn = ->
        writer.setBbox undefined
      assert.throws fn, Error

    "has a writeBbox function": (writer) ->
      assert.isFunction writer.writeBbox

    "has a working writeBbox function": (writer) ->
      assert.deepEqual (writer.writeBbox(reader.read("POINT(1 1)"))), [1,1,1,1]

tests.export module
