#ifndef _IDATABASE_H_
#define _IDATABASE_H_

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace Core {
class	IRow
{
	public:
		virtual ~IRow() {}

		// return false if the value is NULL, true otherwise
		virtual	bool	getString(std::string const &columnName, std::string &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint32_t &data) = 0;
		virtual bool	getFloat(std::string const &columnName, float &data) = 0;
};

class	IResult
{
	public:
		typedef std::vector<std::unique_ptr<IRow>>::iterator		iterator;
		typedef std::vector<std::unique_ptr<IRow>>::const_iterator	const_iterator;

		IResult() : current_row_(0) {}
		virtual ~IResult() {}

		virtual void		useRow(uint32_t _row) {current_row_ = _row;}
		virtual bool		incrementRow() {++current_row_; return true;}
		virtual bool		decrementRow() {uint32_t tmp = current_row_; current_row_ = current_row_ <= 0 ? 0 : current_row_ - 1; return tmp;}
		virtual uint32_t	size() const = 0;

		// return false if the value is NULL, true otherwise
		virtual	bool	getString(std::string const &columnName, std::string &data) = 0;
		virtual bool	getInt(std::string const &columnName, uint32_t &data) = 0;
		virtual bool	getFloat(std::string const &columnName, float &data) = 0;

		virtual iterator	begin() {return rows_.begin();}
		virtual iterator	end() {return rows_.end();}
		virtual const_iterator	cbegin() const {return rows_.cbegin();}
		virtual const_iterator	cend() const {return rows_.cend();}

	protected:
		uint32_t				current_row_;
		std::vector<std::unique_ptr<IRow>>	rows_;
};

class IDatabase
{
public:
	virtual ~IDatabase() {};

	virtual void	Connect(std::string _host, std::string _database, std::string _user, std::string _password) = 0;

	virtual void				QExecute(std::string _query) = 0;
	virtual std::unique_ptr<IResult>	QStore(std::string _query) = 0;
};
}

#endif
