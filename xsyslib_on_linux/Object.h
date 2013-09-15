#ifndef __XSYSLIB_OBJECT_H__
#define __XSYSLIB_OBJECT_H__

#include <string>
#include <iostream>

typedef void* ClassVptr;

class Class;

class Object
{
public:
    virtual ~Object(void);
    static std::string Spaces(int count);
    static std::string StrObj(const Object *obj, bool thisptr);
    std::string ClassName(void) const;

    virtual ClassVptr GetVptr(void);
    virtual void Display(std::ostream &stream, int indent, bool verbose);
    virtual void GetSubtended(Object *objects[], int &count);
    virtual void Patch(int selector, void *arguments);

protected:
    Object(void);
    void MorphTo(Class &target);

private:
    Object *patchData_;
};

#endif // __XSYSLIB_OBJECT_H__
