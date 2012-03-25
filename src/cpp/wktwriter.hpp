#ifndef WKTWRITER_HPP
#define WKTWRITER_HPP

#include <geos/io/WKTWriter.h>
#include "binding.hpp"
#include "geometry.hpp"

class WKTWriter : public ObjectWrap {

    public:
        WKTWriter();
        ~WKTWriter();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);
        static Handle<Value> Write(const Arguments& args);

    protected:
        static Handle<Value> SetRoundingPrecision(const Arguments& args);
        static Handle<Value> SetTrim(const Arguments& args);

    private:
        geos::io::WKTWriter* _writer;

};
#endif
