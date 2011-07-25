#include <string.h>

#include "projection.hpp"

TransformerException::TransformerException(char *description) {
    strncpy(this->description, description, 1024);
}
char *TransformerException::GetDescription() { return this->description; }

Projection::Projection(const char* init)
{
    this->pj = pj_init_plus(init);
}

Projection::~Projection()
{
    pj_free(this->pj);
}

bool Projection::IsValid()
{
    return (this->pj != NULL);
}

Persistent<FunctionTemplate> Projection::projection_template_;

Handle<FunctionTemplate> Projection::MakeProjectionTemplate()
{
    HandleScope scope;
    Handle<FunctionTemplate> t = FunctionTemplate::New(New);
    
    // Setup "Static" Members
    t->Set(String::NewSymbol("transform"), FunctionTemplate::New(Transform));
    
    // Setup Instance Members
    Local<ObjectTemplate> obj_t = t->InstanceTemplate();
    obj_t->SetInternalFieldCount(1);
    obj_t->Set(String::NewSymbol("__projVersion"), String::New(pj_get_release()));
    obj_t->SetAccessor(String::NewSymbol("definition"), GetDefinition);

    return scope.Close(t);
}

void Projection::Initialize(Handle<Object> target)
{
    HandleScope scope;
    if (projection_template_.IsEmpty())
	    projection_template_ = Persistent<FunctionTemplate>::New(MakeProjectionTemplate());
    Handle<FunctionTemplate> t = projection_template_;
    target->Set(String::NewSymbol("Projection"), t->GetFunction());
}

Handle<Value> Projection::New(const Arguments& args)
{
    Projection      *proj;
    HandleScope     scope;
    
    if (args.Length() == 1 && args[0]->IsString()) {
        String::Utf8Value init(args[0]->ToString());
        proj = new Projection(*init);
        
        if (!proj->IsValid()) {
            int     *errno          = pj_get_errno_ref();
            char    *description    = pj_strerrno(*errno);
            
            return ThrowException(String::New(description));
        }
    }
    else {
        return ThrowException(String::New("No valid arguments passed for projection initialization string."));
    }
    
    proj->Wrap(args.This());
    return args.This();        
}

Handle<Value> Projection::GetDefinition(Local<String> name, const AccessorInfo& info)
{
    HandleScope     scope;
    Projection      *self = ObjectWrap::Unwrap<Projection>(info.Holder());
    char            *def;
    Handle<Value>   def_obj;
    
    def = pj_get_def(self->pj, 0);
    def_obj = String::New(def);
    
    pj_dalloc(def);
    return scope.Close(def_obj);
}

Handle<Value> Projection::Transform(const Arguments& args)
{
    HandleScope     scope;
    Handle<Value>   geom_obj;
    
    if (args.Length() < 3) {
        return ThrowException(String::New("Not enough arguments."));
    }
    else {
        Projection          *from   = ObjectWrap::Unwrap<Projection>(args[0]->ToObject());
        Projection          *to     = ObjectWrap::Unwrap<Projection>(args[1]->ToObject());
        Geometry            *geom   = ObjectWrap::Unwrap<Geometry>(args[2]->ToObject());
        PointTransformer    *trans  = new ProjectionPointTransformer(from->pj, to->pj);
        
        try {
            geom->ApplyPointTransformation(trans);
        }
        catch (TransformerException ex) {
            delete trans;
            return ThrowException(String::New(ex.GetDescription()));
        }
        
        delete trans;
    }
    
    return scope.Close(Null());
}

ProjectionPointTransformer::ProjectionPointTransformer(projPJ from, projPJ to) {
    this->from = pj_init_plus(pj_get_def(from, 0));
    this->to = pj_init_plus(pj_get_def(to, 0));
}

ProjectionPointTransformer::~ProjectionPointTransformer() {
    pj_free(this->from);
    pj_free(this->to);
}

void ProjectionPointTransformer::Transform(double *x, double *y, double *z) {
    if (pj_is_latlong(this->from)) {
        *x *= DEG_TO_RAD;
        *y *= DEG_TO_RAD;   
    }
    
    int err = pj_transform(this->from, this->to, 1, 1, x, y, z);
    
    if (pj_is_latlong(this->to)) {
        *x *= RAD_TO_DEG;
        *y *= RAD_TO_DEG;
    }
    
    if (err != 0) {
        throw TransformerException(pj_strerrno(err));
    }
}