#ifndef __CDATABASE_MANAGER_H__
#define __CDATABASE_MANAGER_H__

class CDatabaseMgr
{
public:
	static CDatabaseMgr& getInstance();
	static void deleteInstance();
private:
	CDatabaseMgr();
	~CDatabaseMgr();

};

#endif
