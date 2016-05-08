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
        static Persistent<Function> constructor;
        static void New(const FunctionCallbackInfo<Value>& args);
        static void WriteHEX(const FunctionCallbackInfo<Value>& args);

    protected:

    private:
        geos::io::WKBWriter* _writer;

};
#endif
