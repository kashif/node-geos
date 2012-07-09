#ifndef WKBWRITER_HPP
#define WKBWRITER_HPP

#include <sstream>
#include <geos/io/WKBWriter.h>
#include "binding.hpp"
#include "geometry.hpp"

class WKBWriter : public ObjectWrap {

    public:
        WKBWriter();
        ~WKBWriter();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);
        static Handle<Value> WriteHEX(const Arguments& args);

    protected:

    private:
        geos::io::WKBWriter* _writer;

};
#endif
