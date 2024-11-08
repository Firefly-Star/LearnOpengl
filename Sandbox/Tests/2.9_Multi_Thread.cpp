#include "EntryPoint.h"
#if CHECK_ACTIVE(2, 9, 1)

#include <benchmark/benchmark.h>
#include <thread>
#include <vector>
#include <condition_variable>
#if 0
void compute(int iterations) {
    volatile long long result = 0;
    for (int i = 0; i < iterations; ++i) {
        // Some simple work
        for (int j = 0; j < 1000; ++j) {
            result += j;
        }
    }
}

static void BM_SingleThread(benchmark::State& state) {
    for (auto _ : state) {
        compute(state.range(0));
    }
}

static void BM_MultiThread(benchmark::State& state) {
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (auto _ : state) {
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(compute, state.range(0) / num_threads);
        }
        for (auto& t : threads) {
            t.join();
        }
        threads.clear();
    }
}

#include "BenchmarkHelper.h"
#include <iostream>
using namespace Firefly;
int main()
{
    std::cout << std::thread::hardware_concurrency() << "\n";
    BENCHMARK(BM_SingleThread)->Arg(100000);
    BENCHMARK(BM_MultiThread)->Arg(100000);

    BenchmarkHelper::RunBenchmark();
}
#endif
#if 0
#include <iostream>
#include <mutex>
int x = 0;
std::mutex mtx1;
std::mutex mtx2;
void foo_1()
{
    std::unique_lock l1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::unique_lock l2(mtx2);
    
    ++x;
    l1.swap(l2);
}
void foo_2()
{
    std::unique_lock l1(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::unique_lock l2(mtx1);

    ++x;

    l1.swap(l2);
}

template<typename T>
class Ref
{
public:
    explicit Ref(T& x) : val(&x) {}
    operator T& () noexcept { return *val; }

    Ref(const Ref&) = delete;
    Ref& operator=(const Ref&) = delete;
private:
    T* val;
};

int main()
{
    std::once_flag once;
    std::call_once(once, []() {
        std::cout << "abc\n";
        });

}
#endif

#include <functional>
using namespace std;
class FooBar {
private:
    int n;
    bool isfoo;
    std::mutex mtx;
public:
    FooBar(int n) : isfoo(true) {
        this->n = n;
    }

    void foo(function<void()> printFoo) {

        for (int i = 0; i < n; i++) {
            while (true)
            {
                mtx.lock();
                if (isfoo)
                {
                    mtx.unlock();
                    break;
                }
                mtx.unlock();
            }
            // printFoo() outputs "foo". Do not change or remove this line.
            printFoo();
            mtx.lock();
            isfoo = false;
            mtx.unlock();
        }
    }

    void bar(function<void()> printBar) {

        for (int i = 0; i < n; i++) {
            while (true)
            {
                mtx.lock();
                if (!isfoo)
                {
                    mtx.unlock();
                    break;
                }
                mtx.unlock();
            }
            // printBar() outputs "bar". Do not change or remove this line.
            printBar();
            mtx.lock();
            isfoo = true;
            mtx.unlock();
        }
    }
};
#include <csignal>
#include <iostream>
typedef void(*func)();
int main()
{
    FooBar f(4);
    std::thread t1(&FooBar::foo, &f, []() {
        std::cout << "foo";
        });
    std::thread t2(&FooBar::bar, &f, []() {
        std::cout << "bar";
        });
    t1.join();
    t2.join();
}

#endif