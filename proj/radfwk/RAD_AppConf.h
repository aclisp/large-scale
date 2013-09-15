// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/27 09:18:24
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_APPCONF_H__
#define __RADFWK_APPCONF_H__

#include <radfwk/RAD_Object.h>

class RAD_ConfHelper;
typedef struct main_config_t main_config_t;

class RADFWK_API RAD_AppConf : public RAD_Object
{
    class Detail;

public:
    RAD_AppConf();
    explicit RAD_AppConf(const char* conffile);
    virtual ~RAD_AppConf();

    /* Same as constructor.
     * Returns 0 if succeed.
     */
    int load(const char* conffile);

    /* Save all the configuration data to filename.
     * Returns 0 if succeed.
     */
    int save(const char* filename, bool separatedConfHelper = false) const;

    /* Find a `ConfHelper' to query or modify. */
    RAD_ConfHelper* findConfHelper(const char* section);
    RAD_ConfHelper* findConfHelper(unsigned int index);

    size_t totalConfHelper();

    /* Add a new `ConfHelper'.
     * Returns NULL and delete confHelper pointer if there already exists one.
     */
    RAD_ConfHelper* addConfHelper(RAD_ConfHelper* confHelper);

    void delConfHelper(const char* section);
    void delAllConfHelper();

    main_config_t* mainconfig();

private:
    RAD_AppConf(const RAD_AppConf& );
    RAD_AppConf& operator=(const RAD_AppConf& other);

    Detail* pImpl;
};

void strcpydup(char** dst, const char* src);

#endif // __RADFWK_APPCONF_H__
