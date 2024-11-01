#pragma once

#include <memory>
#include <stdexcept>
#include <queue>
namespace Firefly
{
	template<typename T>
	class Stack
	{
	public:
		Stack(unsigned int size = 8)
			:m_Capability(size), m_Data(new T[size]), m_Size(0)
		{
		}
		~Stack()
		{
			delete[] m_Data;
		}
		T top()
		{
			return m_Data[m_Size-1];
		}
		void pop()
		{
			if (m_Size > 0)
			{
				--m_Size;
			}
		}
		unsigned int size()
		{
			return m_Size;
		}
		bool empty()
		{
			return m_Size == 0;
		}
		void push(T obj)
		{
			if (m_Size == m_Capability)
			{
				T* new_Data = new T[2 * m_Capability];
				std::memcpy(new_Data, m_Data, m_Capability * sizeof(T));
				delete[] m_Data;
				m_Data = new_Data;
				m_Capability = 2 * m_Capability;
			}
			m_Data[m_Size] = obj;
			++m_Size;
		}
	private:
		T* m_Data;
		unsigned int m_Capability;
		unsigned int m_Size;
	};

	template<typename T>
	struct LinkNode
	{
		T data;
		LinkNode<T>* next;
	};

	template<typename T>
	class LinkQueue
	{
	public:
		LinkQueue()
			:m_Front(nullptr), m_Back(nullptr)
		{
		}
		~LinkQueue()
		{
			while (m_Front != nullptr)
			{
				LinkNode<T>* temp = m_Front;
				m_Front = m_Front->next;
				delete temp;
			}
		}
		bool empty()
		{
			return m_Front == nullptr;
		}
		T front()
		{
			return m_Front->data;
			throw std::runtime_error("Queue is empty");
		}
		void pop()
		{
			if (m_Front != nullptr)
			{
				LinkNode<T>* temp = m_Front;
				if (m_Back == m_Front)
				{
					m_Back = nullptr;
				}
				m_Front = m_Front->next;
				delete temp;
			}
		}
		void push(T obj)
		{
			LinkNode<T>* newNode = new LinkNode<T>{ obj, nullptr };
			if (m_Front == nullptr)
			{
				m_Front = newNode;
				m_Back = newNode;
			}
			else
			{
				m_Back->next = newNode;
				m_Back = newNode;
			}
		}
	private:
		LinkNode<T>* m_Front;
		LinkNode<T>* m_Back;
	};

}