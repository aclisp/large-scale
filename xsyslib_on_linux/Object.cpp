#include "Object.h"
#include "Class.h"
#include <sstream>
#include <typeinfo>

using namespace std;

Object::Object(void)
: patchData_(NULL)
{
}

Object::~Object(void)
{
}

string Object::Spaces (int count)
{
    string s = "";
    if(count > 0) 
        s.insert(s.begin(), count, ' ');
    return s;
}

string Object::StrObj(const Object *obj, bool thisptr)
{
    if (obj != NULL)
    {
        ostringstream stream;
        // display pointer to object
        if(thisptr) stream << obj << " ";
        stream << obj->ClassName ();
        return stream.str();
    }
    return "NULL";
}

string Object::ClassName (void) const
{
    string name = typeid(*this).name();
    //if(name.compare(0, 6, "class ") == 0)
    //    return name.substr(6, name.size());
    return name; 
}

void Object::Display (ostream &stream, int indent, bool verbose)
{
    stream << Spaces (indent) << ClassName () << endl;
    indent = indent + 2;
    stream << Spaces (indent) << "this      : ";
    stream << this << endl;
    stream << Spaces (indent) << "patchData : ";
    if((patchData_ != NULL) && (verbose))
    {
        stream << endl;
        patchData_->Display (stream, indent + 2, verbose);
    }
    else
        stream << StrObj (patchData_, true) << endl;
}

void Object::GetSubtended (Object *objects[], int &count)
{
    objects[count] = this;
    count ++;
    if(patchData_ != NULL)
        patchData_->GetSubtended (objects, count);
}

void Object::Patch (int , void *)
{

}

struct ObjectVptr
{
    void *vptr; // resides at the top of each Object
};

ClassVptr Object::GetVptr ()
{
    // Return this object's vptr.
    //
    ObjectVptr *obj = (ObjectVptr*) this;
    return obj->vptr;
}

void Object::MorphTo (Class &target)
{
    // Change this object's vptr to that of the target class.
    //
    ObjectVptr *obj = (ObjectVptr*) this;
    obj->vptr = target.vptr_;
}
