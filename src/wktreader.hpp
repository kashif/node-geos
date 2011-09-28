#ifndef WKTREADER_HPP
#define WKTREADER_HPP

#include <geos/io/WKTReader.h>
#include "binding.hpp"
#include "geometry.hpp"

class WKTReader : public ObjectWrap {

    public:
        WKTReader();
        ~WKTReader();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);
        static Handle<Value> Read(const Arguments& args);

    protected:

    private:
        geos::io::WKTReader* _reader;

};
#endif
