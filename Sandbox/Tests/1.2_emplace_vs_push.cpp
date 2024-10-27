#include "Entrypoint.h"
#if ACTIVE_LESSON_MAJOR == 1 && ACTIVE_LESSON_MINOR == 2 && ISTEST == 1

#include <iostream>
#include <vector>
#include <chrono>

class Mesh
{
public:
    Mesh(long a, long b, long c, long d, long e)
        : a(a), b(b), c(c), d(d), e(e)
    {
    }

private:
    long a;
    long b;
    long c;
    long d;
    long e;
};

Mesh create(long a)
{
    long b = a + 1;
    long c = b + 1;
    long d = c + 1;
    long e = d + 1;
    return Mesh(a, b, c, d, e);
}

template <typename Func>
void MeasurePerformance(const char* description, Func func)
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << description << ": " << diff.count() << " seconds" << std::endl;
}

int main()
{
    const long numMeshes = 1000000;  // 设置要插入的对象数量

    // 使用push_back，不预先reserve
    MeasurePerformance("push_back (no reserve)", [numMeshes]() {
        std::vector<Mesh> meshes;
        for (long i = 0; i < numMeshes; ++i)
        {
            Mesh m = create(i);
            meshes.push_back(m);  // push_back 插入对象
        }
        });

    // 使用push_back，预先reserve
    MeasurePerformance("push_back (with reserve)", [numMeshes]() {
        std::vector<Mesh> meshes;
        meshes.reserve(numMeshes);  // 预先reserve
        for (long i = 0; i < numMeshes; ++i)
        {
            Mesh m = create(i);
            meshes.push_back(m);  // push_back 插入对象
        }
        });

    // 使用emplace_back，不预先reserve
    MeasurePerformance("emplace_back (no reserve)", [numMeshes]() {
        std::vector<Mesh> meshes;
        for (long i = 0; i < numMeshes; ++i)
        {
            Mesh m = create(i);
            meshes.emplace_back(m);  // emplace_back 直接构造对象
        }
        });

    // 使用emplace_back，预先reserve
    MeasurePerformance("emplace_back (with reserve)", [numMeshes]() {
        std::vector<Mesh> meshes;
        meshes.reserve(numMeshes);  // 预先reserve
        for (long i = 0; i < numMeshes; ++i)
        {
            Mesh m = create(i);
            meshes.emplace_back(m);  // emplace_back 直接构造对象
        }
        });

    return 0;
}

#endif