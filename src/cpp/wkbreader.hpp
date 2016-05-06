#ifndef WKBREADER_HPP
#define WKBREADER_HPP

#include <geos/io/WKBReader.h>
#include <geos/io/ParseException.h>

#include "binding.hpp"
#include "geometry.hpp"
#include "geometryfactory.hpp"

class WKBReader : public ObjectWrap {

    public:
        WKBReader();
        WKBReader(const geos::geom::GeometryFactory *gf);
        ~WKBReader();
        static void Initialize(Handle<Object> target);
        static Persistent<Function> constructor;
        static void New(const FunctionCallbackInfo<Value>& args);
        static void ReadHEX(const FunctionCallbackInfo<Value>& args);

    protected:

    private:
        geos::io::WKBReader* _reader;

};
#endif
