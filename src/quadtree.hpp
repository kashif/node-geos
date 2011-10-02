#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "binding.hpp"
#include "quadtree.hpp"

#include "geos/index/quadtree/Quadtree.h"

class Quadtree : public ObjectWrap {
    public:
        Quadtree();
        ~Quadtree();
        static void Initialize(Handle<Object> target);
        static Persistent<FunctionTemplate> constructor;
        static Handle<Value> New(const Arguments& args);

    protected:

    private:
        geos::index::quadtree::Quadtree *_quadtree;
};
#endif
