#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../lib").Geometry
WKTReader = (require "../lib").WKTReader
GeoJSONReader = (require "../lib").GeoJSONReader

reader = new WKTReader()

tests = (vows.describe "GeoJSONReader").addBatch

  "A GeoJSONReader instance with default GeometryFactory":
    topic: ->
      new GeoJSONReader

    "is correct": (reader) ->
      assert reader instanceof GeoJSONReader, true

    "read(invalid type)": (reader) ->
      assert.throws(
        -> reader.read(null)
        /instance of Object/
      )

    "read(missing type) ": (reader) ->
      assert.throws(
        -> reader.read({})
        /type.*?missing/
      )

    "read(unsupported type) ": (reader) ->
      assert.throws(
        -> reader.read( type: 1 )
        /type.*?allowed/
      )

    "read(missing coordinates) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Point' )
        /coordinates.*?missing/
      )

    "read(coordinates with wrong type) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Point', coordinates: 1 )
        /coordinates.*?Array/
      )

    "read(coordinates with too few values) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Point', coordinates: [] )
        /coordinate.*?2/
      )

    "read(Point with numeric coordinate values) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'Point', coordinates: [1,'2','3'] ).toJSON()
        type: 'Point', coordinates: [1, 2, 3]
      )


    "read(empty LineString) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'LineString', coordinates: null ).toJSON()
        type: 'LineString', coordinates: null
      )

    "read(empty LineString with 0 coordinates) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'LineString', coordinates: [] ).toJSON()
        type: 'LineString', coordinates: null
      )

    "read(wrong LineString with 1 coordinates) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'LineString', coordinates: [[1,2]] ).toJSON()
        /point.*?0.*?> *?1/
      )

    "read(LineString with more coordinates) ": (reader) ->
      coords = [[1,2],[1,2],[2,3],[4.1234567,-180.1234567]]
      assert.deepEqual(
        reader.read( type: 'LineString', coordinates: coords ).toJSON()
        type: 'LineString', coordinates: coords
      )


    "read(empty Polygon) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'Polygon', coordinates: null ).toJSON()
        type: 'Polygon', coordinates: null
      )

    "read(wrong Polygon with 0 linear rings) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Polygon', coordinates: [] ).toJSON()
        /number.*?linear rings/
      )

    "read(wrong Polygon with wrong type for shell) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Polygon', coordinates: [1] ).toJSON()
        /coordinate sequence.*?Array/
      )

    "read(empty Polygon with 0 coordinates in shell) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'Polygon', coordinates: [[]] ).toJSON()
        type: 'Polygon', coordinates: null
      ) 

    "read(wrong Polygon with unclosed LinearRing) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Polygon', coordinates: [[[1,2],[3,4],[5,6]]] ).toJSON()
        /LinearRing.*?closed linestring/
      )

    "read(wrong Polygon with too short LinearRing) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'Polygon', coordinates: [[[1,2],[3,4],[1,2]]] ).toJSON()
        /number.*?LinearRing/
      )

    "read(Polygon without holes) ": (reader) ->
      coords = [[[1,2],[3,4],[5,6],[1,2]]]
      assert.deepEqual(
        reader.read( type: 'Polygon', coordinates: coords ).toJSON()
        type: 'Polygon', coordinates: coords
      )

    "read(Polygon with holes) ": (reader) ->
      coords = [[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]]]
      assert.deepEqual(
        reader.read( type: 'Polygon', coordinates: coords ).toJSON()
        type: 'Polygon', coordinates: coords
      )


    "read(empty MultiPoint) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiPoint', coordinates: null ).toJSON()
        type: 'MultiPoint', coordinates: null
      )

    "read(empty MultiPoint with 0 coordinates) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiPoint', coordinates: [] ).toJSON()
        type: 'MultiPoint', coordinates: null
      )

    "read(MultiPoint with more coordinates) ": (reader) ->
      coords = [[1,2],[1,2],[2,3],[4.1234567,-180.1234567]]
      assert.deepEqual(
        reader.read( type: 'MultiPoint', coordinates: coords ).toJSON()
        type: 'MultiPoint', coordinates: coords
      )


    "read(empty MultiLineString) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiLineString', coordinates: null ).toJSON()
        type: 'MultiLineString', coordinates: null
      )

    "read(empty MultiLineString with no LineStrings) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiLineString', coordinates: [] ).toJSON()
        type: 'MultiLineString', coordinates: null
      )

    "read(MultiLineString with 1 empty LineString) ": (reader) ->
      coords = [[],[[1,2],[2,3]]]
      assert.deepEqual(
        reader.read( type: 'MultiLineString', coordinates: coords ).toJSON()
        type: 'MultiLineString', coordinates: coords
      )

    "read(MultiLineString) ": (reader) ->
      coords = [[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]]]
      assert.deepEqual(
        reader.read( type: 'MultiLineString', coordinates: coords ).toJSON()
        type: 'MultiLineString', coordinates: coords
      )


    "read(empty MultiPolygon) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiPolygon', coordinates: null ).toJSON()
        type: 'MultiPolygon', coordinates: null
      )

    "read(empty MultiPolygon with 0 Polygons) ": (reader) ->
      assert.deepEqual(
        reader.read( type: 'MultiPolygon', coordinates: [] ).toJSON()
        type: 'MultiPolygon', coordinates: null
      )

    "read(MultiPolygon with more Polygons) ": (reader) ->
      coords = [
        [
          [ [ 0, 0 ], [ 5, 0 ], [ 5, 5 ], [ 0, 5 ], [ 0, 0 ] ]
          [ [ 1, 1 ], [ 2, 1 ], [ 2, 2 ], [ 1, 2 ], [ 1, 1 ] ]
          [ [ 3, 3 ], [ 4, 3 ], [ 4, 4 ], [ 3, 4 ], [ 3, 3 ] ]
        ]
        [
          [ [ 0, 0 ], [ 1, 1 ], [ 0, 1 ], [ 0, 0 ] ]
        ]
      ]
      assert.deepEqual(
        reader.read( type: 'MultiPolygon', coordinates: coords ).toJSON()
        type: 'MultiPolygon', coordinates: coords
      )


    "read(GeometryCollection with missing geometry) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'GeometryCollection', coordinates: [] ).toJSON()
        /geometries.*?missing/
      )

    "read(GeometryCollection with geometries with wrong type) ": (reader) ->
      assert.throws(
        -> reader.read( type: 'GeometryCollection', geometries: null ).toJSON()
        /geometries.*?Array/
      )

    "read(GeometryCollection) ": (reader) ->
      coll = {
        type: 'GeometryCollection'
        geometries: [
          {
            type: 'Polygon'
            coordinates: [
              [ [ 0, 0 ], [ 5, 0 ], [ 5, 5 ], [ 0, 5 ], [ 0, 0 ] ]
              [ [ 1, 1 ], [ 2, 1 ], [ 2, 2 ], [ 1, 2 ], [ 1, 1 ] ]
              [ [ 3, 3 ], [ 4, 3 ], [ 4, 4 ], [ 3, 4 ], [ 3, 3 ] ]
            ]
          }
          {
            type: 'MultiLineString'
            coordinates: [
              [ [ 0, 0 ], [ 1, 1 ], [ 1, 3 ] ]
              [ [ 2, 2 ], [ 5, 5 ], [ 1, 3 ] ]
            ]
          }
        ]
      }
      assert.deepEqual(
        reader.read( coll ).toJSON()
        coll
      )

tests.export module
