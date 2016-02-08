#include "cdatabase_manager.h"

static CDatabaseMgr* pInstance = nullptr;

CDatabaseMgr& CDatabaseMgr::getInstance()
{
	if( pInstance == nullptr )
		pInstance = new CDatabaseMgr();

	return *pInstance;
}

void CDatabaseMgr::deleteInstance()
{
	if( pInstance != nullptr )
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

CDatabaseMgr::CDatabaseMgr()
{

}

CDatabaseMgr::~CDatabaseMgr()
{

}
