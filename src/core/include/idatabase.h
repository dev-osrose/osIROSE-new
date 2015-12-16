#ifndef _IDATABASE_H_
#define _IDATABASE_H_

class IDatabase
{
public:
	IDatabase() {};
	virtual ~IDatabase() {};

	virtual void	QExecute(std::string _query) = 0;
};

#endif
