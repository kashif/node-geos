#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "binding.hpp"
#include "quadtree.hpp"
#include "geometry.hpp"

#include "geos/index/quadtree/Quadtree.h"

class Quadtree : public ObjectWrap {
    public:
        geos::index::quadtree::Quadtree *_quadtree;

        Quadtree();
        ~Quadtree();

        static void Initialize(Handle<Object> target);

        static Persistent<FunctionTemplate> constructor;

        static Handle<Value> New(const Arguments& args);
        static Handle<Value> Insert(const Arguments& args);
        static Handle<Value> Remove(const Arguments& args);
        static Handle<Value> QueryAll(const Arguments& args);
        static Handle<Value> Query(const Arguments& args);

        static void EIO_Query(eio_req *req);
        static int EIO_AfterQuery(eio_req *req);

        static Handle<Value> Size(const Arguments& args);
        static Handle<Value> Depth(const Arguments& args);

    protected:

    private:
};
#endif
