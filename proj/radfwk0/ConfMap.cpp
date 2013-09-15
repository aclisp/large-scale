#include "radutil.h"

#include <new>
#include <string>
#include <iterator>
#include <algorithm>

#include "ConfMap.h"
#include "functors.hpp"
#include "ListMap.hpp"

// It's not so good to keep ConfSection as a pointer in ConfSectionMap.
// Because when using ConfMap as a multi-index cache, ConfSection
// (which can be take as a row in a table here) may be inserted and
// deleted very frequently.

// When you really need pointer semantic (what I never suggest),
// uncomment following line.
// #define CONFMAP_KEEP_CONFSEC_AS_POINTER 1

class ConfMap::Detail : private boost::noncopyable
{
public:
    typedef ListMap<std::string, std::string> ConfSection;
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
    typedef ListMap<std::string, ConfSection*> ConfSectionMap;
#else
    typedef ListMap<std::string, ConfSection> ConfSectionMap;
#endif
    Detail(ConfMap* thisConfMap) : enclosure(thisConfMap)
                                 , secHasName2(true)
        {}
    ~Detail()
        { enclosure->clear(); }

    ConfMap* enclosure;
    bool secHasName2;
    std::string secName;
    ConfSectionMap confSecMap;
};

ConfMap::ConfMap(const char* conffile, const char* section)
{
    pImpl = new Detail(this);
    rad_assert(pImpl != NULL);
    load(conffile, section);
}

ConfMap::ConfMap(CONF_SECTION* conf, const char* section)
{
    pImpl = new Detail(this);
    rad_assert(pImpl != NULL);
    load(conf, section);
}

ConfMap::~ConfMap()
{
    delete pImpl;
}

const char* ConfMap::findByName(const char* name, const char* key) const
{
    rad_assert(name != NULL);

    Detail::ConfSectionMap::const_iterator iter = pImpl->confSecMap.find(name);
    if(iter == pImpl->confSecMap.end())
        return NULL;

    if(key == NULL)
        return name;

#if CONFMAP_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::const_iterator iter2 = iter->second->find(key);
    if(iter2 == iter->second->end())
#else
    Detail::ConfSection::const_iterator iter2 = iter->second.find(key);
    if(iter2 == iter->second.end())
#endif
        return NULL;

    return iter2->second.c_str();
}

ConfMap::uint ConfMap::totalCount() const
{
    return pImpl->confSecMap.size();
}

const char* ConfMap::findByIndex(uint index, const char* key) const
{
    if(index < 0 || index >= totalCount())
        return NULL;

    Detail::ConfSectionMap::const_iterator iter = pImpl->confSecMap.begin();
    std::advance(iter, index);

    if(key == NULL)
        return iter->first.c_str();

#if CONFMAP_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::const_iterator iter2 = iter->second->find(key);
    if(iter2 == iter->second->end())
#else
    Detail::ConfSection::const_iterator iter2 = iter->second.find(key);
    if(iter2 == iter->second.end())
#endif
        return NULL;

    return iter2->second.c_str();
}

int ConfMap::addOrUpdate(const char* name, const char* key, const char* value)
{
    // Insert if section doesn't exist
    if(findByName(name, NULL) == NULL) {
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
        Detail::ConfSection* confSec = new Detail::ConfSection;
        rad_assert(confSec != NULL);
        if(key && value)
            confSec->insert(Detail::ConfSection::value_type(key, value));
#else
        Detail::ConfSection confSec;
        if(key && value)
            confSec.insert(Detail::ConfSection::value_type(key, value));
#endif
        pImpl->confSecMap.insert(Detail::ConfSectionMap::value_type(name, confSec));
        return 0;
    }

    if(key == NULL || value == NULL) {
        radlog(L_WARN|L_CONS, "[ConfMap::addOrUpdate] name '%s' already exists, "
               "but key or value is NULL. Operation has no effect.", name);
        return -1;
    }

    Detail::ConfSectionMap::iterator iter = pImpl->confSecMap.find(name);

    // Insert if key doesn't exist
    if(findByName(name, key) == NULL) {
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
        iter->second->insert(Detail::ConfSection::value_type(key, value));
#else
        iter->second.insert(Detail::ConfSection::value_type(key, value));
#endif
        return 0;
    }

    // Update
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::iterator iter2 = iter->second->find(key);
#else
    Detail::ConfSection::iterator iter2 = iter->second.find(key);
#endif
    iter2->second = value;
    return 0;
}

int ConfMap::remove(const char* name, const char* key)
{
    if(findByName(name, key) == NULL) {
        radlog(L_WARN|L_CONS, "[ConfMap::remove] name '%s' or key '%s' can not be found. "
               "Operation has no effect.", name, key?key:"<NULL>");
        return -1;
    }

    Detail::ConfSectionMap::iterator iter = pImpl->confSecMap.find(name);

    if(key == NULL) {
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
        delete iter->second;
#else
        // do nothing
#endif
        pImpl->confSecMap.erase(iter);
    }
    else {
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
        iter->second->erase(key);
#else
        iter->second.erase(key);
#endif
    }

    return 0;
}

int ConfMap::save(const char* filename) const
{
    FILE* file = fopen(filename, "w");
    if(file == NULL) {
        radlog(L_CONS|L_ERROR, "[ConfMap::save] can not open file '%s' for write", filename);
        return -1;
    }

    int ret = save(file);
    fclose(file);

    return ret;
}

int ConfMap::save(FILE* file) const
{
    fprintf(file, "\n");
    fprintf(file, "######################################################################\n");
    fprintf(file, "# Configuration group '%s' generated by [ConfMap::save]\n\n",
            pImpl->secName.c_str());

    for(Detail::ConfSectionMap::const_iterator i = pImpl->confSecMap.begin();
        i != pImpl->confSecMap.end(); ++i) {
        fprintf(file, "%s ", pImpl->secName.c_str());
        if(pImpl->secHasName2)
            fprintf(file, "%s ", i->first.c_str());
        fprintf(file, "{\n");

#if CONFMAP_KEEP_CONFSEC_AS_POINTER
        for(Detail::ConfSection::const_iterator j = i->second->begin();
            j != i->second->end(); ++j)
#else
        for(Detail::ConfSection::const_iterator j = i->second.begin();
            j != i->second.end(); ++j)
#endif
            fprintf(file, "    %s = %s\n", j->first.c_str(), j->second.c_str());

        fprintf(file, "}\n\n");
    }

    return 0;
}

int ConfMap::load(const char* conffile, const char* section)
{
    pImpl->secName = section;

    CONF_SECTION *conf = conf_read(__FILE__, __LINE__, conffile, NULL);
    if(conf == NULL) {
        radlog(L_CONS|L_ERROR, "[ConfMap::load] can not read '%s'", conffile);
        return -1;
    }

    int ret = load(conf, section);

    // free config handle
    cf_section_free(&conf);

    return ret;
}

int ConfMap::load(CONF_SECTION* conf, const char* section)
{
    pImpl->confSecMap.clear();
    pImpl->secName = section;

    CONF_SECTION* subconf = NULL;

    // Check first section
    subconf = cf_subsection_find_next(conf, NULL, section);
    if(subconf == NULL) {
        return 0;
    }
    pImpl->secHasName2 = (cf_section_name2(subconf) != NULL);

    // Iterate from first section
    subconf = NULL;
    while((subconf = cf_subsection_find_next(conf, subconf, section))) {
        const char* name = cf_section_name2(subconf);
        CONF_PAIR* pair = NULL;
        if(!pImpl->secHasName2) { // Check first key if section has no name2
            pair = cf_pair_find_next(subconf, NULL, NULL);
            if(pair == NULL) {
                radlog(L_CONS|L_WARN, "[ConfMap::load] section '%s' at line %d "
                       "without name2 and any key would be ignored.",
                       section, cf_section_lineno(subconf));
                continue;
            }
            // Set name to first value if the *first* section has no name2
            name = cf_pair_value(pair);
        } else if (name == NULL) { // Some section lacks name2, but the first section does have
            radlog(L_CONS|L_WARN, "[ConfMap::load] section '%s' at line %d "
                   "without name2 would be ignored!!!", section, cf_section_lineno(subconf));
            continue;
        }
        // Iterate from first pair
        pair = NULL;
        while((pair = cf_pair_find_next(subconf, pair, NULL))) {
            const char* key   = cf_pair_attr(pair);
            const char* value = cf_pair_value(pair);
            addOrUpdate(name, key, value);
        }
    }
    return totalCount();
}

const char* ConfMap::find(const char* name, const char* key, const char* def)
{
    const char* val = findByName(name, key);
    if(val == NULL) {
        addOrUpdate(name, key, def);
        return def;
    } else {
        return val;
    }
}

const char* ConfMap::find(const char* name, const char* key, const char* def) const
{
    const char* val = findByName(name, key);
    if(val == NULL) {
        return def;
    } else {
        return val;
    }
}

const char* ConfMap::name() const
{
    return pImpl->secName.c_str();
}

void ConfMap::clear()
{
#if CONFMAP_KEEP_CONFSEC_AS_POINTER
    std::for_each(pImpl->confSecMap.begin(), pImpl->confSecMap.end(),
                  delete_pointer<Detail::ConfSectionMap::value_type>());
#else
    // do nothing
#endif
    pImpl->confSecMap.clear();
}
