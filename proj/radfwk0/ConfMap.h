#ifndef RADFWK_CONFMAP_H
#define RADFWK_CONFMAP_H

#include <radfwk/noncopyable.hpp>
#include <radutil/radconffile.h>
#include <stdio.h>

/*********************************************************************
 * For groups of configuration data like
======================================================================
client [name1] {
    key1 = value1
    key2 = value2
    key3 = value3
    ...
}
client [name2] {
    key1 = value1
    key2 = value2
    key3 = value3
    ...
}
client [name3] {
    key1 = value1
    key2 = value2
    key3 = value3
    ...
}
...
======================================================================
* I have to read them without `cf_section_parse'.
* `ConfMap' is an interface that encapsulate these operations.
*/
class ConfMap : private boost::noncopyable
{
    class Detail;
    typedef unsigned int uint;

public:
    /* Read all sections in conffile.
     * If there is no section name2, take the first key's value instead.
     */
    ConfMap(const char* conffile, const char* section);
    ConfMap(CONF_SECTION* conf, const char* section);
    ~ConfMap();

    /* Returns section name of this `ConfMap' */
    const char* name() const;

    /* This *find* should be very fast, so can be called frequently.
     * The name is section name2 (or the first key's value when no name2).
     */
    const char* findByName(const char* name, const char* key) const;

    /* Like `findByName', but would call `addOrUpdate' when not found.
     * So this function would always be success.
     */
    const char* find(const char* name, const char* key, const char* def);
    /* Like `findByName', but would NOT call `addOrUpdate'.
     * Returns default when not found.
     */
    const char* find(const char* name, const char* key, const char* def) const;

    uint totalCount() const;
    void clear();

    /* This *find* may be extremely slow when `totalCount' is big.
     * Just call it when you want to display all the configuration
     * data. To retrieve section name2, give a NULL key.
     */
    const char* findByIndex(uint index, const char* key) const;

    /* A NULL key means to add a new section.
     * Returns 0 if succeed.
     */
    int addOrUpdate(const char* name, const char* key, const char* value);

    /* A NULL key means to remove this section
     * Returns 0 if succeed.
     */
    int remove(const char* name, const char* key);

    /* Save all the configuration data to filename.
     * Returns 0 if succeed.
     */
    int save(const char* filename) const;
    int save(FILE* file) const;

    /* Same as constructor.
     * Returns `totalCount' if succeed.
     */
    int load(const char* conffile, const char* section);
    int load(CONF_SECTION* conf, const char* section);

private:
    Detail* pImpl;
};

#endif /* RADFWK_CONFMAP_H */

// Local Variables:
// mode:C++
// End:
