#include <radutil/radutil.h>
#include <stdio.h>
#include <assert.h>
#if defined(RAD_OS_UNIX) || defined(RAD_OS_LINUX)
#include <dlfcn.h>
#endif

typedef int (* fun_t)();

void dlcall(const char* dllname)
{
    int *var;
    fun_t fun;
    void* handle;
    handle = dlopen(dllname, RTLD_NOW|RTLD_GLOBAL);
    if(handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        return;
    }
    var = (int *)dlsym(handle, "var");
    if(var == NULL) {
        fprintf(stderr, "dlsym(var): %s\n", dlerror());
        goto close;
    }
    fun = (fun_t)dlsym(handle, "fun");
    if(fun == NULL) {
        fprintf(stderr, "dlsym(fun): %s\n", dlerror());
        goto close;
    }
    *var = 20;
    assert(fun() == 21);
close:
    assert(dlclose(handle) == 0);
}

int main()
{
#if defined(RAD_OS_UNIX) || defined(RAD_OS_LINUX)
    dlcall("libdyn.so");
#else
    dlcall("dynlib_vc.dll");
    dlcall("dynlib_mingw.dll");
#endif
    return 0;
}
