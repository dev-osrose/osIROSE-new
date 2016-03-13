
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T>
class Singleton {
	public:
		static T& getInstance() {
			static T instance;
			return instance;
		}

	protected:
		virtual ~Singleton() {}

	private:
		Singleton() = delete;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(Singleton const&) = delete;
};

#endif /* !_SINGLETON_H_ */
