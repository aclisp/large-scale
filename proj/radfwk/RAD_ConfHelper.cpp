#include "RAD_ConfHelper.h"
#include "RAD_Util.h"

// It's not so good to keep ConfSection as a pointer in ConfSectionMap.
// Because when using RAD_ConfHelper as a multi-index cache, ConfSection
// (which can be take as a row in a table here) may be inserted and
// deleted very frequently.

// When you really need pointer semantic (what I never suggest),
// uncomment following line.
// #define CONFHELPER_KEEP_CONFSEC_AS_POINTER 1

class RAD_ConfHelper::Detail
{
public:
    typedef list_map<string, string> ConfSection;
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
    typedef list_map<string, ConfSection*> ConfSectionMap;
#else
    typedef list_map<string, ConfSection> ConfSectionMap;
#endif
    Detail(RAD_ConfHelper* thisConfHelper) : enclosure(thisConfHelper)
                                 , secHasName2(true)
        {}
    ~Detail()
        { enclosure->clear(); }

    RAD_ConfHelper* enclosure;
    bool secHasName2;
    string secName;
    ConfSectionMap confSecMap;
};

RAD_ConfHelper::RAD_ConfHelper(const char* conffile, const char* section)
    : RAD_Object("RAD_ConfHelper")
{
    pImpl = new Detail(this);
    rad_assert(pImpl != NULL);
    load(conffile, section);
}

RAD_ConfHelper::RAD_ConfHelper(CONF_SECTION* conf, const char* section)
    : RAD_Object("RAD_ConfHelper")
{
    pImpl = new Detail(this);
    rad_assert(pImpl != NULL);
    load(conf, section);
}

RAD_ConfHelper::~RAD_ConfHelper()
{
    delete pImpl;
}

const char* RAD_ConfHelper::findByName(const char* name, const char* key) const
{
    rad_assert(name != NULL);

    Detail::ConfSectionMap::const_iterator iter = pImpl->confSecMap.find(name);
    if(iter == pImpl->confSecMap.end())
        return NULL;

    if(key == NULL)
        return name;

#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::const_iterator iter2 = iter->second->find(key);
    if(iter2 == iter->second->end())
#else
    Detail::ConfSection::const_iterator iter2 = iter->second.find(key);
    if(iter2 == iter->second.end())
#endif
        return NULL;

    return iter2->second.c_str();
}

size_t RAD_ConfHelper::totalCount() const
{
    return pImpl->confSecMap.size();
}

const char* RAD_ConfHelper::findByIndex(unsigned int index, const char* key) const
{
    if(index < 0 || index >= totalCount())
        return NULL;

    Detail::ConfSectionMap::const_iterator iter = pImpl->confSecMap.begin();
    advance(iter, index);

    if(key == NULL)
        return iter->first.c_str();

#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::const_iterator iter2 = iter->second->find(key);
    if(iter2 == iter->second->end())
#else
    Detail::ConfSection::const_iterator iter2 = iter->second.find(key);
    if(iter2 == iter->second.end())
#endif
        return NULL;

    return iter2->second.c_str();
}

int RAD_ConfHelper::addOrUpdate(const char* name, const char* key, const char* value)
{
    // Insert if section doesn't exist
    if(findByName(name, NULL) == NULL) {
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
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
        radlog(L_WARN|L_CONS, "[RAD_ConfHelper::addOrUpdate] name '%s' already exists, "
               "but key or value is NULL. Operation has no effect.", name);
        return -1;
    }

    Detail::ConfSectionMap::iterator iter = pImpl->confSecMap.find(name);

    // Insert if key doesn't exist
    if(findByName(name, key) == NULL) {
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
        iter->second->insert(Detail::ConfSection::value_type(key, value));
#else
        iter->second.insert(Detail::ConfSection::value_type(key, value));
#endif
        return 0;
    }

    // Update
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
    Detail::ConfSection::iterator iter2 = iter->second->find(key);
#else
    Detail::ConfSection::iterator iter2 = iter->second.find(key);
#endif
    iter2->second = value;
    return 0;
}

int RAD_ConfHelper::remove(const char* name, const char* key)
{
    if(findByName(name, key) == NULL) {
        radlog(L_WARN|L_CONS, "[RAD_ConfHelper::remove] name '%s' or key '%s' can not be found. "
               "Operation has no effect.", name, key?key:"<NULL>");
        return -1;
    }

    Detail::ConfSectionMap::iterator iter = pImpl->confSecMap.find(name);

    if(key == NULL) {
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
        delete iter->second;
#else
        // do nothing
#endif
        pImpl->confSecMap.erase(iter);
    }
    else {
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
        iter->second->erase(key);
#else
        iter->second.erase(key);
#endif
    }

    return 0;
}

int RAD_ConfHelper::save(const char* filename) const
{
    FILE* file = fopen(filename, "w");
    if(file == NULL) {
        radlog(L_CONS|L_ERROR, "[RAD_ConfHelper::save] can not open file '%s' for write", filename);
        return -1;
    }

    int ret = save(file);
    fclose(file);

    return ret;
}

int RAD_ConfHelper::save(FILE* file) const
{
    fprintf(file, "\n");
    fprintf(file, "######################################################################\n");
    fprintf(file, "# Configuration group '%s' generated by [RAD_ConfHelper::save]\n\n",
            pImpl->secName.c_str());

    for(Detail::ConfSectionMap::const_iterator i = pImpl->confSecMap.begin();
        i != pImpl->confSecMap.end(); ++i) {
        fprintf(file, "%s ", pImpl->secName.c_str());
        if(pImpl->secHasName2)
            fprintf(file, "%s ", i->first.c_str());
        fprintf(file, "{\n");

#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
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

int RAD_ConfHelper::load(const char* conffile, const char* section)
{
    pImpl->secName = section;

    CONF_SECTION *conf = conf_read(__FILE__, __LINE__, conffile, NULL);
    if(conf == NULL) {
        radlog(L_CONS|L_ERROR, "[RAD_ConfHelper::load] can not read '%s'", conffile);
        return -1;
    }

    int ret = load(conf, section);

    // free config handle
    cf_section_free(&conf);

    return ret;
}

int RAD_ConfHelper::load(CONF_SECTION* conf, const char* section)
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
                radlog(L_CONS|L_WARN, "[RAD_ConfHelper::load] section '%s' at line %d "
                       "without name2 and any key would be ignored.",
                       section, cf_section_lineno(subconf));
                continue;
            }
            // Set name to first value if the *first* section has no name2
            name = cf_pair_value(pair);
        } else if (name == NULL) { // Some section lacks name2, but the first section does have
            radlog(L_CONS|L_WARN, "[RAD_ConfHelper::load] section '%s' at line %d "
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

const char* RAD_ConfHelper::find(const char* name, const char* key, const char* def)
{
    const char* val = findByName(name, key);
    if(val == NULL) {
        addOrUpdate(name, key, def);
        return def;
    } else {
        return val;
    }
}

const char* RAD_ConfHelper::find(const char* name, const char* key, const char* def) const
{
    const char* val = findByName(name, key);
    if(val == NULL) {
        return def;
    } else {
        return val;
    }
}

const char* RAD_ConfHelper::name() const
{
    return pImpl->secName.c_str();
}

void RAD_ConfHelper::clear()
{
#if CONFHELPER_KEEP_CONFSEC_AS_POINTER
    for_each(pImpl->confSecMap.begin(), pImpl->confSecMap.end(),
                  delete_pointer<Detail::ConfSectionMap::value_type>());
#else
    // do nothing
#endif
    pImpl->confSecMap.clear();
}
