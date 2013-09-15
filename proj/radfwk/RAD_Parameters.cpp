#include "RAD_Parameters.h"

RAD_Parameters::RAD_Parameters()
    : RAD_Object("RAD_Parameters")
{
}

RAD_Parameters::~RAD_Parameters()
{
}

void RAD_Parameters::Swap(RAD_Parameters& other)
{
    m_Data.swap( other.m_Data );
}

RAD_Parameters::RAD_Parameters(const RAD_Parameters& other)
    : RAD_Object("RAD_Parameters")
    , m_Data( other.m_Data )
{
}
