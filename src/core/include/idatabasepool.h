
#ifndef _IDATABASEPOOL_H_
#define _IDATABASEPOOL_H_

#include "idatabase.h"

namespace Core {

class IDatabasePool {
	public:
		virtual IDatabase&	getDatabase() = 0;
};

class IDatabasePoolFactory {
	public:
		virtual IDatabasePool&	operator()() const = 0;
};

template <typename Factory>
class IDatabasePoolAbstractFactory {
	public:
		static IDatabasePool& getInstance() {
			static IDatabasePool &pool = Factory()();
			return pool;
		}

	private:
		IDatabasePoolAbstractFactory() = delete;
		IDatabasePoolAbstractFactory(const IDatabasePoolAbstractFactory&) = delete;
		IDatabasePoolAbstractFactory& operator=(const IDatabasePoolAbstractFactory&) = delete;
};

}

#endif /* !_IDATABASEPOOL_H_ */
