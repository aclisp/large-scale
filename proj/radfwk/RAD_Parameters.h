// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/22 16:36:46
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_PARAMETERS_H__
#define __RADFWK_PARAMETERS_H__

#include <radfwk/RAD_Object.h>

class RADFWK_API RAD_Parameters : public RAD_Object
{
public:
    RAD_Parameters();
    virtual ~RAD_Parameters();

    void Swap(RAD_Parameters& other); /* throw() */
    RAD_Parameters(const RAD_Parameters& );
    RAD_Parameters& operator=(const RAD_Parameters& other)
        { RAD_Parameters temp(other); Swap(temp); return *this; }

    vector <string> m_Data ;
};

#endif // __RADFWK_PARAMETERS_H__
