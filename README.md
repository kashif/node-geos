#node-geos

[GEOS (Geometry Engine)](http://trac.osgeo.org/geos/) and [PROJ.4 (Cartographic Projections)](http://trac.osgeo.org/proj/) bindings for Node.

##Prerequisites

Make sure you have GEOS version 3.x as well as PROJ.4 version 4.7.x or higher
installed together with the headers and libraries. (This is maybe outdated. Will
be updated soon.)

## Installation

To obtain and build the bindings:

    git clone git://github.com/kashif/node-geos.git
    cd node-geos
    node-waf configure build

or install it via [npm](http://npmjs.org/):

    npm install geos (outdated version)

## Testing

To test the c++ binding we use vows. Just run vows on the tests in the test
directory. Make sure you have coffee-script installed as they are written in
coffee-script.
