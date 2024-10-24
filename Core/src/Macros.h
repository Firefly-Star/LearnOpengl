#pragma once
#define ASSET_PREFIX "C:/Users/Summer/Desktop/OpenGL/Sandbox/Assets/"
#define ASSET(x) ASSET_PREFIX x

#define SINGLETON_DELETE(x) \
public: \
    x(x&& other) = delete; \
    x(const x& other) = delete; \
    x& operator=(x&& other) = delete; \
    x& operator=(const x& other) = delete;

#define SINGLETON_FUNC(x) \
public: \
    template <typename... Args> \
    static void Init(Args&&... args) { \
        std::call_once(s_OnceFlag, [&]() { \
            s_Instance.reset(new x(std::forward<Args>(args)...)); \
        }); \
    } \
    static void Terminate() { s_Instance.reset(); } \
    static x& GetInstance() { return *s_Instance; } \
private: \
    static std::unique_ptr<x> s_Instance; \
    static std::once_flag s_OnceFlag; \
private: \
    friend struct std::default_delete<x>;

#define SINGLETON(x) \
    SINGLETON_DELETE(x) \
    SINGLETON_FUNC(x)

#define SINGLETON_DEFINATION(x)\
std::unique_ptr<x> x::s_Instance = nullptr;\
std::once_flag x::s_OnceFlag; 

#define MOVE_ONLY(x)\
public:\
    x(x&& other) noexcept;\
    x& operator=(x&& other) noexcept;\
    x(const x& other) = delete;\
    x& operator=(const x& other) = delete;