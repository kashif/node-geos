#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include <proj_api.h>
#include "binding.hpp"
#include "geometry.hpp"

class ProjectionPointTransformer : public PointTransformer {
 public:
     ProjectionPointTransformer(projPJ from, projPJ to);
     ~ProjectionPointTransformer();
     virtual void Transform(double *x, double *y, double *z);
     
 private:
     projPJ from;
     projPJ to;
};

class Projection : public ObjectWrap {
 public:
    projPJ pj;
    
    Projection(const char* init);
    ~Projection();
    
    bool IsValid();
    
    static void Initialize(Handle<Object> target);

 protected:
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> GetDefinition(Local<String> name, const AccessorInfo& info);
    
    static Handle<Value> Transform(const Arguments& args);
    
 private:
    static Persistent<FunctionTemplate> projection_template_;
    static Handle<FunctionTemplate> MakeProjectionTemplate();
};

#endif