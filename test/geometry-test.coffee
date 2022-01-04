#!/usr/bin/env coffee

vows = require "vows"
assert = require "assert"

Geometry = (require "../lib").Geometry
WKTReader = (require "../lib").WKTReader

tests = (vows.describe "Geometry").addBatch

  "A Geometry":
    topic: ->
      (new WKTReader()).read "POINT(1 1)"

    "a new instance should be an instance of Geometry": (geom) ->
      assert.instanceOf geom, Geometry

    "should have a toString function" : (geom) ->
      assert.isFunction geom.toString
      assert.equal geom.toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    "should have a toGeoJSON function": (geom) ->
      assert.isFunction geom.toGeoJSON

    "should have a toJSON (alias) function": (geom) ->
      assert.isFunction geom.toJSON

    # geos unary predicates
    # TODO real tests!
    "should have a isSimple function": (geom) ->
      assert.isFunction geom.isSimple
      assert.isTrue geom.isSimple()
    "should have a async isSimple function": (geom) ->
      geom.isSimple (error, isSimple) ->
        assert.isTrue isSimple
    "should have a isValid function": (geom) ->
      assert.isFunction geom.isValid
    "should have a async isValid function": (geom) ->
      geom.isValid (error, isValid) ->
        assert.isTrue isValid
    "should have a isEmpty function": (geom) ->
      assert.isFunction geom.isEmpty
      assert.isFalse geom.isEmpty()
    "should have a async isEmpty function": (geom) ->
      geom.isEmpty (error, isEmpty) ->
        assert.isFalse isEmpty
    "should have a isRectangle function": (geom) ->
      assert.isFunction geom.isRectangle
      assert.isFalse geom.isRectangle()
    "should have a async isRectangle function": (geom) ->
      geom.isRectangle (error, isRectangle) ->
        assert.isFalse isRectangle

    # geos binary predicates
    "should have a disjoint function": (geom) ->
      assert.isFunction geom.disjoint
      assert.isFalse geom.disjoint geom

    "shoudl have an async disjoint function": (geom) ->
      geom.disjoint geom, (error, disjoint) ->
        assert.isFalse disjoint

    "should have a touches function": (geom) ->
      assert.isFunction geom.touches
      assert.ok not geom.touches geom

    "should have an async touches function": (geom) ->
      geom.touches geom, (error, touches) ->
        assert.isFalse touches

    "should have a intersects function": (geom) ->
      assert.isFunction geom.intersects
      assert.isTrue geom.intersects geom

    "should have an async intersects function": (geom) ->
      geom.intersects geom, (error, intersects) ->
        assert.isTrue intersects

    "should have a crosses function": (geom) ->
      assert.isFunction geom.crosses
      assert.isFalse geom.crosses geom

    "should have an async crosses function": (geom) ->
      geom.crosses geom, (error, crosses) ->
        assert.isFalse crosses

    "should have a within function": (geom) ->
      assert.isFunction geom.within
      assert.isTrue geom.within geom

    "should have an async within function": (geom) ->
      geom.within geom, (error, within) ->
        assert.isTrue within

    "should have a contains function": (geom) ->
      assert.isFunction geom.contains
      assert.isTrue geom.contains geom

    "should have an async contains function": (geom) ->
      geom.contains geom, (error, contains) ->
        assert.isTrue contains

    "should have a overlaps function": (geom) ->
      assert.isFunction geom.overlaps
      assert.isFalse geom.overlaps geom

    "should have a async overlaps function": (geom) ->
      geom.overlaps geom, (error, overlaps) ->
        assert.isFalse overlaps

    "should have a equals function": (geom) ->
      assert.isFunction geom.equals
      assert.isTrue geom.equals geom

    "should have an async equals function": (geom) ->
      geom.equals geom, (error, equals) ->
        assert.isTrue equals

    "should have a covers function": (geom) ->
      assert.isFunction geom.covers
      assert.isTrue geom.covers geom

    "should have an async covers function": (geom) ->
      geom.covers geom, (error, covers) ->
        assert.isTrue covers

    "should have a coveredBy function": (geom) ->
      assert.isFunction geom.coveredBy
      assert.isTrue geom.coveredBy geom

    "should have an async coveredBy function": (geom) ->
      geom.coveredBy geom, (error, coveredBy) ->
        assert.isTrue coveredBy

    #topologic functions
    "should have a intersection function": (geom) ->
      assert.isFunction geom.intersection
      assert.ok (geom.intersection geom).equals geom

    "should have a union function": (geom) ->
      assert.isFunction geom.union
      geom2 = new WKTReader().read "POINT (2 2)"
      assert.equal (geom.union geom2).toString(),
        "MULTIPOINT (1.0000000000000000 1.0000000000000000, 2.0000000000000000 2.0000000000000000)"

    "should have a difference function": (geom) ->
      assert.isFunction geom.difference
      #TODO use real geometries
      assert.equal (geom.difference geom).toString(), "GEOMETRYCOLLECTION EMPTY"

    "should have a symDifference function": (geom) ->
      assert.isFunction geom.symDifference
      assert.equal (geom.symDifference geom).toString(), "GEOMETRYCOLLECTION EMPTY"

    "should have a distance function": (geom) ->
      assert.isFunction geom.distance
      assert.equal (geom.distance geom), 0

    "should have a isWithinDistance": (geom) ->
      assert.isFunction geom.isWithinDistance
      assert.ok geom.isWithinDistance geom, 10

    "should have a getEnvelope function": (geom) ->
      assert.isFunction geom.getEnvelope
      assert.equal geom.getEnvelope().toString(), "POINT (1.0000000000000000 1.0000000000000000)"

    "should return a valid envelope for a linestring": ->
      geom = (new WKTReader()).read "LINESTRING (0 0, 1 1)"
      #TODO remove this ugly wkt
      assert.equal geom.getEnvelope().toString(), "POLYGON ((0.0000000000000000 0.0000000000000000, 1.0000000000000000 0.0000000000000000, 1.0000000000000000 1.0000000000000000, 0.0000000000000000 1.0000000000000000, 0.0000000000000000 0.0000000000000000))"

    "should have a getBoundary function": (geom) ->
      assert.isFunction geom.getBoundary
      assert.equal geom.getBoundary().toString(), "GEOMETRYCOLLECTION EMPTY"

    "should return a valid boundary for a linestring": ->
      geom = (new WKTReader()).read "LINESTRING (0 0, 1 1)"
      #TODO remove this ugly wkt
      assert.equal geom.getEnvelope().toString(), "POLYGON ((0.0000000000000000 0.0000000000000000, 1.0000000000000000 0.0000000000000000, 1.0000000000000000 1.0000000000000000, 0.0000000000000000 1.0000000000000000, 0.0000000000000000 0.0000000000000000))"

    "should have a buffer function": (geom) ->
      assert.isFunction geom.buffer

    "should return a valid buffer given a distance": (geom) ->
      #TODO remove this even ulgier wkt
      assert.equal (geom.buffer 1.0).toString(), "POLYGON ((2.0000000000000000 1.0000000000000000, 1.9807852804032304 0.8049096779838719, 1.9238795325112870 0.6173165676349106, 1.8314696123025456 0.4444297669803983, 1.7071067811865481 0.2928932188134531, 1.5555702330196031 0.1685303876974553, 1.3826834323650909 0.0761204674887137, 1.1950903220161297 0.0192147195967698, 1.0000000000000016 0.0000000000000000, 0.8049096779838736 0.0192147195967692, 0.6173165676349122 0.0761204674887125, 0.4444297669803995 0.1685303876974537, 0.2928932188134541 0.2928932188134509, 0.1685303876974562 0.4444297669803957, 0.0761204674887143 0.6173165676349077, 0.0192147195967701 0.8049096779838688, 0.0000000000000000 0.9999999999999968, 0.0192147195967689 1.1950903220161249, 0.0761204674887118 1.3826834323650863, 0.1685303876974525 1.5555702330195991, 0.2928932188134495 1.7071067811865446, 0.4444297669803942 1.8314696123025427, 0.6173165676349064 1.9238795325112852, 0.8049096779838678 1.9807852804032295, 0.9999999999999962 2.0000000000000000, 1.1950903220161249 1.9807852804032311, 1.3826834323650867 1.9238795325112881, 1.5555702330195995 1.8314696123025469, 1.7071067811865455 1.7071067811865497, 1.8314696123025438 1.5555702330196044, 1.9238795325112859 1.3826834323650921, 1.9807852804032300 1.1950903220161304, 2.0000000000000000 1.0000000000000000))"

    "should return a valid buffer given a distance and quadrantSegments": (geom) ->
      #TODO check if this and the one above fails on different cpus
      assert.equal (geom.buffer 1.0, 2).toString(), "POLYGON ((2.0000000000000000 1.0000000000000000, 1.7071067811865481 0.2928932188134531, 1.0000000000000016 0.0000000000000000, 0.2928932188134541 0.2928932188134509, 0.0000000000000000 0.9999999999999968, 0.2928932188134495 1.7071067811865446, 0.9999999999999953 2.0000000000000000, 1.7071067811865435 1.7071067811865515, 2.0000000000000000 1.0000000000000000))"

    "should return a valid buffer given a distance, quadrantSegments and endCapStyle": (geom) ->
      #TODO stupid test!?!
      assert.equal (geom.buffer 1.0, 2, 1).toString(), "POLYGON ((2.0000000000000000 1.0000000000000000, 1.7071067811865481 0.2928932188134531, 1.0000000000000016 0.0000000000000000, 0.2928932188134541 0.2928932188134509, 0.0000000000000000 0.9999999999999968, 0.2928932188134495 1.7071067811865446, 0.9999999999999953 2.0000000000000000, 1.7071067811865435 1.7071067811865515, 2.0000000000000000 1.0000000000000000))"

    "should have a convexHull function": (geom) ->
      assert.isFunction geom.convexHull
      assert.ok geom.convexHull().equals geom

    "should return a valid convexHull for a linestring": ->
      #TODO change geometry
      geom = (new WKTReader()).read "LINESTRING (0 0, 1 1)"
      assert.equal geom.convexHull().toString(), "LINESTRING (0.0000000000000000 0.0000000000000000, 1.0000000000000000 1.0000000000000000)"

    "should have a getArea function": (geom) ->
      assert.isFunction geom.getArea
      assert.equal geom.getArea(), 0

    "should have a getLength function": (geom) ->
      assert.isFunction geom.getLength
      assert.equal geom.getLength(), 0

    "should have a getSRID function": (geom) ->
      assert.isFunction geom.getSRID
      assert.equal geom.getSRID(), 0

    "should have a setSRID function": (geom) ->
      assert.isFunction geom.setSRID
      assert.equal undefined, geom.setSRID 4326
      assert.equal geom.getSRID(), 4326

    "should have a getGeometryType function": (geom) ->
      assert.isFunction geom.getGeometryType
      assert.isString geom.getGeometryType()
      assert.equal geom.getGeometryType(), "Point"

  "A invalid Geometry":

    topic: ->
      multipolygon = "MULTIPOLYGON(((1 1,5 1,5 5,1 5,1 1),(2 2, 3 2, 3 3, 2 3,2 2)),
        ((3 3,6 2,6 4,3 3)),
        ((78 45,65 34,100 54,78 45),(4 65, 54 23, 544 346, 2 1, 4 65)))"
      (new WKTReader()).read multipolygon

    "should return false on isValid": (geom) ->
      assert.isFalse geom.isValid()
      geom.isValid (error, isValid) ->
        assert.isFalse isValid

    "should throw an error on spatial operations": (geom) ->
      fn = ->
        geom.intersects geom
      assert.throws fn, Error

  "An empty Geometry":

    topic: ->
      (new WKTReader()).read "POINT EMPTY"

    "should return true on isEmpty": (geom) ->
      assert.isTrue geom.isEmpty()

  "A direct call to Geometry constructor":

    topic: ->
      ->
        Geometry

    "should throw an Error": (call) ->
      assert.throws call(), Error

tests.export module
