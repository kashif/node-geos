#ifndef WKTREADER_HPP
#define WKTREADER_HPP

#include <geos/io/WKTReader.h>
#include <geos/io/ParseException.h>

#include "binding.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"

class WKTReader : public ObjectWrap {

    public:
        WKTReader();
        WKTReader(const geos::geom::GeometryFactory *gf);
        ~WKTReader();
        static void Initialize(Handle<Object> target);
        static Persistent<Function> constructor;
        static void New(const FunctionCallbackInfo<Value>& args);
        static void Read(const FunctionCallbackInfo<Value>& args);

    protected:

    private:
        geos::io::WKTReader* _reader;

};
#endif
