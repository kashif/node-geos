#node-geos

[GEOS (Geometry Engine)](http://trac.osgeo.org/geos/) bindings for Node.

##Prerequisites

Required node version is 0.6.0 or higher.
Make sure you have GEOS version 3.2.2 or higher installed together with the
headers and libraries.

##Installation

To obtain and build the bindings:

    git clone git://github.com/kashif/node-geos.git
    cd node-geos
    node-gyp configure build
    # npm install

or install it via [npm](http://npmjs.org/):

    npm install geos

or

    npm install git://github.com/kashif/node-geos.git

##Testing

    npm test

To test the c++ binding we use vows. Just run vows on the tests in the test
directory or use the above command. Make sure you have coffee-script installed as they are written in
coffee-script.

As continuous integration system we use [travis](http://travis-ci.org).

Build status: [![Build Status](https://secure.travis-ci.org/kashif/node-geos.png)](http://travis-ci.org/kashif/node-geos)

##Examples

For some examples please take a look at the `examples` directory.

##Bugs

Report bugs to
[http://github.com/kashif/node-geos/issues](http://github.com/kashif/node-geos/issues).
