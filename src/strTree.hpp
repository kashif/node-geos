#ifndef STRTREE_HPP
#define STRTREE_HPP

#include "binding.hpp"
#include "geometry.hpp"

class StrTree : public ObjectWrap {
    private:
       static void QueryCallback(void *item, void *userdata);
    protected:

        static Handle<Value> New(const Arguments& args);
        static Handle<Value> Insert(const Arguments& args);
        static Handle<Value> Remove(const Arguments& args);
        static Handle<Value> Query(const Arguments& args);
    public:

        GEOSSTRtree *geos_strTree_;
        StrTree(int size);
        ~StrTree();
        static void Initialize(Handle<Object> target);

};

#endif
