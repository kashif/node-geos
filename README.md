#node-geos

[GEOS (Geometry Engine)](http://trac.osgeo.org/geos/) bindings for Node.

##Prerequisites

Make sure you have GEOS version 3.x or higher installed together with the
headers and libraries.

##Installation

To obtain and build the bindings:

    git clone git://github.com/kashif/node-geos.git
    cd node-geos
    node-waf configure build
    # npm install

or install it via [npm](http://npmjs.org/):

    npm install geos (outdated version)

##Testing

    npm test

To test the c++ binding we use vows. Just run vows on the tests in the test
directory. Make sure you have coffee-script installed as they are written in
coffee-script.

##Examples

For some examples please take a look at the examples directory.

##Bugs

Report bugs to
[http://github.com/kashif/node-geos/issues](http://github.com/kashif/node-geos/issues).
