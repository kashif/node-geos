MODE = 1 # 1: just testing  2: check for memory leaks, loop ITERATIONS times
ITERATIONS = 1000




GeoJSONReader = (require "../lib").GeoJSONReader

reader = new GeoJSONReader()
if MODE == 2
	reader.read = ->

console.log process.memoryUsage()

log = if MODE == 2
	->
else
	console.log

max = if MODE == 2
	ITERATIONS
else
	1

for i in [0...max]

	[
		-> reader.read( null )
		-> reader.read( [] )
		-> reader.read( {} )
		-> reader.read( type:'xyz',coordinates:null )

		-> reader.read( type:'Point' )
		-> reader.read( type:'Point',coordinates:1 )
		-> reader.read( type:'Point',coordinates:[] )
		-> reader.read( type:'Point',coordinates:[{}, '2'] )
		-> reader.read( type:'Point',coordinates:[1, 2, 3],abc:1 )

		-> reader.read( type:'LineString',coordinates:null )
		-> reader.read( type:'LineString',coordinates:[] )
		-> reader.read( type:'LineString',coordinates:[[1,2]] )
		-> reader.read( type:'LineString',coordinates:[[1,2],[1,2]] ) 
		-> reader.read( type:'LineString',coordinates:[[1,2,3],[1,2]] )
		-> reader.read( type:'LineString',coordinates:[[1,2,3],[1,2],[1,2]] )
		
		-> reader.read( type:'Polygon',coordinates:null )
		-> reader.read( type:'Polygon',coordinates:[] )
		-> reader.read( type:'Polygon',coordinates:[1] )
		-> reader.read( type:'Polygon',coordinates:[[]] )
		-> reader.read( type:'Polygon',coordinates:[[[1,2],[3,4],[5,6]]] )
		-> reader.read( type:'Polygon',coordinates:[[[1,2],[3,4],[1,2]]] )
		-> reader.read( type:'Polygon',coordinates:[[[1,2],[3,4],[5,6],[1,2]]] )
		-> reader.read( type:'Polygon',coordinates:[[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]]] )
		
		-> reader.read( type:'MultiPoint',coordinates:null )
		-> reader.read( type:'MultiPoint',coordinates:[] )
		-> reader.read( type:'MultiPoint',coordinates:[1,2] )
		-> reader.read( type:'MultiPoint',coordinates:[[1,2],[1,2]] )

		-> reader.read( type:'MultiLineString',coordinates:null )
		-> reader.read( type:'MultiLineString',coordinates:[] )
		-> reader.read( type:'MultiLineString',coordinates:[1] )
		-> reader.read( type:'MultiLineString',coordinates:[[[1,2]]] )
		-> reader.read( type:'MultiLineString',coordinates:[[[1,2],[2,4]]] )
		-> reader.read( type:'MultiLineString',coordinates:[[[1,2],[2,4]],[[1,2],[2,4]],[[1,2],[2,4]]] )

		-> reader.read( type:'MultiPolygon',coordinates:null )
		-> reader.read( type:'MultiPolygon',coordinates:[] )
		-> reader.read( type:'MultiPolygon',coordinates:[1] )
		-> reader.read( type:'MultiPolygon',coordinates:[[]] )
		-> reader.read( type:'MultiPolygon',coordinates:[[[1,2],[3,4],[5,6]]] )
		-> reader.read( type:'MultiPolygon',coordinates:[[[[1,2],[3,4],[5,6]]]] )
		-> reader.read( type:'MultiPolygon',coordinates:[[[[1,2],[3,4],[1,2]]]] )
		-> reader.read( type:'MultiPolygon',coordinates:[[[[1,2],[3,4],[5,6],[1,2]]]] )
		-> reader.read( type:'MultiPolygon',coordinates:[[[[1,2],[3,4],[5,6],[1,2]]],[[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]],[[1,2],[3,4],[5,6],[1,2]]]] )

		-> reader.read( type:'GeometryCollection',coordinates:null )
		-> reader.read( type:'GeometryCollection',geometries:null )
		-> reader.read( type:'GeometryCollection',geometries:[] )
		-> reader.read( type:'GeometryCollection',geometries:[1] )
		-> reader.read( type:'GeometryCollection',geometries:[{type:'Point',coordinates:[1,2]},{type:'LineString',coordinates:null}] )

	].forEach((fun) ->
		try
			log fun().toJSON()
		catch e
			log e.stack
	)

console.log process.memoryUsage()