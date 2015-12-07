#ifndef _IDATABASE_H_
#define _IDATABASE_H_

class IDatabase
{
public:
	IDatabase() {};
	virtual ~IDatabase() {};

	virtual bool Connect() = 0;
	virtual void Disconnect() = 0;
	virtual bool QExecute(std::string _query) = 0;
};

#endif
