#pragma once

#include <type_traits>
#include <iostream>
#include <variant>

namespace Firefly
{
    template<typename T>
    class PoolAllocator
    {
    private:
        using storage_type = std::variant<T, size_t>;
    public:
        PoolAllocator(size_t size = 4)
            :m_Capacity(size), m_Buffer(new storage_type[size])
        {
            for (size_t i = 0; i < size; ++i)
            {
                m_Buffer[i].emplace<size_t>(i + 1);
            }
        }

        ~PoolAllocator()
        {
            delete[] m_Buffer;
        }

        template<typename... Args>
        T* Allocate(Args&&... args)
        {
            if (std::get<size_t>(m_Buffer[0]) == m_Capacity)
            {
                size_t newCapacity;
                storage_type* newBuffer;
                try
                {
                    newCapacity = 2 * m_Capacity;
                    newBuffer = new storage_type[newCapacity];
                }
                catch (std::bad_alloc e)
                {
                    std::cout << e.what() << "\n";
                    return nullptr;
                }
                for (size_t i = 0; i < m_Capacity; ++i)
                {
                    std::visit([newBuffer, i](auto& value) {
                        using value_type = std::remove_reference_t<decltype(value)>;
                        if constexpr(std::is_same_v<value_type, size_t>)
                        {
                            newBuffer[i].emplace<size_t>(value);
                        }
                        else if constexpr(std::is_same_v<value_type, T>)
                        {
                            newBuffer[i].emplace<T>(std::move(value));
                        }
                        else
                        {
                            std::cout << "no matching type.\n";
                        }
                        }, m_Buffer[i]);
                }
                delete[] m_Buffer;
                m_Buffer = newBuffer;
                for (size_t i = m_Capacity; i < 2 * m_Capacity; ++i)
                {
                    m_Buffer[i].emplace<size_t>(i + 1);
                }
                m_Capacity = newCapacity;
            }

            auto t = m_Buffer + std::get<size_t>(m_Buffer[0]);
            size_t next = std::get<size_t>(*t);
            t->template emplace<T>(std::forward<Args>(args)...);
            std::get<size_t>(m_Buffer[0]) = next;
            return std::get_if<T>(t);
        }

        void DeAllocate(T* ptr)
        {
            size_t index = (reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(m_Buffer)) / sizeof(storage_type);
            if (index < m_Capacity)
            {
                m_Buffer[index].emplace<size_t>(std::get<size_t>(m_Buffer[0]));
                std::get<size_t>(m_Buffer[0]) = index;
            }
            else
            {
                throw std::runtime_error("Failed to deallocate a pointer beyond the management of pool allocator!");
            }
        }

    private:
        storage_type* m_Buffer;

        size_t m_Capacity;
    };
}
