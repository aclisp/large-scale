#include "RAD_Global.h"
#include "RAD_Object.h"
#include "RAD_Cron.h"

void RAD_Global::Initialize()
{
    radlog(L_INFO, "[RAD_Global::Initialize]");

    RAD_Object::StartObjectTrack();

    socket_init();

    RAD_Cron::m_pGlobalCron = rad_new RAD_Cron;
    RAD_Cron::m_pGlobalCron->m_ThreadName = "RAD_GlobalCron";
    RAD_Cron::m_pGlobalCron->m_bShouldWatch = false;
}

void RAD_Global::Uninitialize()
{
    radlog(L_INFO, "[RAD_Global::Uninitialize]");

    delete RAD_Cron::m_pGlobalCron;

    socket_clean();

    RAD_Object::StopObjectTrack();
}
