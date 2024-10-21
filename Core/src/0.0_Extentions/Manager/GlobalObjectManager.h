#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#if 0
namespace Firefly
{
	class GlobalObjectManager
	{
	public:
		static GlobalObjectManager& GetInstance();

		template<typename T, typename... Args>
		unsigned int Registration(Args... args);

		void Remove(unsigned int index);

		template<typename T>
		T& Write(unsigned int index);

		template<typename T>
		T& Read(unsigned int index);

	public:
		GlobalObjectManager(GlobalObjectManager&& other) = delete;
		GlobalObjectManager(const GlobalObjectManager& other) = delete;
		
		GlobalObjectManager& operator=(GlobalObjectManager&& other) = delete;
		GlobalObjectManager& operator=(const GlobalObjectManager& other) = delete;
	private:
		std::unordered_map<unsigned int, std::unique_ptr<void>> m_Table;
		std::vector<unsigned int> m_InsertOrder;
		std::mutex m_Mutex;
		std::shared_lock<std::mutex> m_mutex;
		unsigned int m_Index{0};
	private:
		GlobalObjectManager() = default;
		~GlobalObjectManager() = default;
	};


	template<typename T, typename ...Args>
	inline unsigned int GlobalObjectManager::Registration(Args ...args)
	{
		m_Mutex.lock();
		++m_Index;
		m_Table[m_Index] = std::move(std::unique_ptr<void>(new T(args...)), [](void* ptr) {
			delete reinterpret_cast<T*>(ptr);
			});
		m_InsertOrder.push_back(m_Index);
		m_Mutex.unlock();
		return m_Index;
	}
}
#endif