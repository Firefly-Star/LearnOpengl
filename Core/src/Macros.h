#pragma once
#define ASSET_PREFIX "C:/Users/Summer/Desktop/OpenGL2/Sandbox/Assets/"
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