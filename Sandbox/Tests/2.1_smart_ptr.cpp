#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 1, 1)

#include <iostream>
#include <cstddef>


template<typename T>
struct PCB // ptr control block
{
	T* ptr;
	int ref;
};

template<typename T>
class shared_ptr
{
public:
	shared_ptr() : m_PCB( new PCB<T>{nullptr, 0} ) {}

	shared_ptr(T* ptr) : m_PCB{ new PCB<T>{ptr, 1} } { ptr = nullptr; }
	
	~shared_ptr()
	{
		if (m_PCB) 
		{
			--m_PCB->ref;
			if (m_PCB->ref == 0) 
			{
				delete m_PCB->ptr;
				delete m_PCB;
			}
		}
	}

	shared_ptr(const shared_ptr& other)
		:m_PCB(other.m_PCB)
	{
		++m_PCB->ref;
	}

	shared_ptr(shared_ptr&& other)
		:m_PCB(other.m_PCB)
	{
		other.m_PCB = nullptr;
	}

	shared_ptr(std::nullptr_t)
		:m_PCB{ new PCB<T>{nullptr, 0} }
	{

	}

	int getRef() { return m_PCB->ref; }

	T* get() { return m_PCB->ptr; }

	void free()
	{
		if (m_PCB)
		{
			--m_PCB->ref;
			if (m_PCB->ref == 0)
			{
				delete m_PCB->ptr;
				delete m_PCB;
			}
			m_PCB = nullptr;
		}
	}

	shared_ptr& operator=(const shared_ptr& other)
	{
		if (this != other)
		{
			--m_PCB->ref;
			if (m_PCB->ref == 0)
			{
				delete m_PCB->ptr;
				delete m_PCB;
			}
			m_PCB = other.m_PCB;
			++m_PCB->ref;
		}
		return *this;
	}

	shared_ptr& operator=(shared_ptr&& other)
	{
		if (this != other)
		{
			--m_PCB->ref;
			if (m_PCB->ref == 0)
			{
				delete m_PCB->ptr;
				delete m_PCB;
			}
			m_PCB = other.m_PCB;
			other.m_PCB = nullptr;
		}

		return *this;
	}

	T& operator*()
	{
		return *(m_PCB->ptr);
	}

	template<typename... Args>
	friend shared_ptr<T> make_shared(Args... args);

private:
	PCB<T>* m_PCB;
};

template<typename T, typename... Args>
shared_ptr<T> make_shared(Args... args)
{
	return shared_ptr(new T(args...));
}

int main()
{
	shared_ptr<int> a = make_shared<int>(2);
	shared_ptr<int> b = a;
	shared_ptr<int> c;
	std::cout << (c.get() == nullptr) << "\n";
	std::cout << *a << "\n";
	std::cout << a.getRef() << "\n";
	b.free();
	std::cout << a.getRef() << "\n";
}


#endif