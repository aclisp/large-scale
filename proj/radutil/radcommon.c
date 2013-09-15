#include "radutil.h"
#include <string.h>

int          lrad_str2int (
    const LRAD_NAME_NUMBER *table,
    const char             *name,
    int def
    )
{
	const LRAD_NAME_NUMBER *this;

	for (this = table; this->name != NULL; this++) {
		if (strcasecmp(this->name, name) == 0) {
			return this->number;
		}
	}

	return def;
}

const char * lrad_int2str (
    const LRAD_NAME_NUMBER *table,
    int                     number,
    const char *def
    )
{
	const LRAD_NAME_NUMBER *this;

	for (this = table; this->name != NULL; this++) {
		if (this->number == number) {
			return this->name;
		}
	}

	return def;
}

const char * lrad_stra2b (
    const LRAD_NAME_MAP *table,
    const char          *name1,
    const char *def
    )
{
	const LRAD_NAME_MAP *this;

	for (this = table; this->name1 != NULL; this++) {
		if (strcasecmp(this->name1, name1) == 0) {
			return this->name2;
		}
	}

	return def;
}

const char * lrad_strb2a (
    const LRAD_NAME_MAP *table,
    const char          *name2,
    const char *def
    )
{
	const LRAD_NAME_MAP *this;

	for (this = table; this->name1 != NULL; this++) {
		if (strcasecmp(this->name2, name2) == 0) {
			return this->name1;
		}
	}

	return def;
}
