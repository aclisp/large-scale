#ifndef RADFWK_CONFIG_H
#define RADFWK_CONFIG_H

#include <radfwk/noncopyable.hpp>
#include <radutil/radconffile.h>

class ConfMap;
class Config;
typedef struct main_config_t main_config_t;

class Config : private boost::noncopyable
{
    class Detail;
    typedef unsigned int uint;

public:
    Config();
    Config(const char* conffile);
    ~Config();

    /* Same as constructor.
     * Returns 0 if succeed.
     */
    int load(const char* conffile);

    /* Save all the configuration data to filename.
     * Returns 0 if succeed.
     */
    int save(const char* filename, bool separatedConfMap = false) const;

    /* Find a `ConfMap' to query or modify. */
    ConfMap* findConfMap(const char* section);
    ConfMap* findConfMap(uint index);

    uint totalConfMap();

    /* Add a new `ConfMap'.
     * Returns NULL and delete confMap pointer if there already exists one.
     */
    ConfMap* addConfMap(ConfMap* confMap);

    void delConfMap(const char* section);
    void delAllConfMap();

    main_config_t* mainconfig();

private:
    Detail* pImpl;
};

void strcpydup(char** dst, const char* src);

#endif /* RADFWK_CONFIG_H */

// Local Variables:
// mode:C++
// End:
