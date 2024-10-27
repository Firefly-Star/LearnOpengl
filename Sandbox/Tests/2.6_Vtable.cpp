#include "Entrypoint.h"
#if CHECK_ACTIVE(2, 6, 1)

#include "BenchmarkHelper.h"
#include <iostream>
#include <benchmark/benchmark.h>

// CRTP base class
template<typename Derived>
class CRTP_Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }
protected:
    int x = 0;
};

// CRTP derived class
class CRTP_Derived : public CRTP_Base<CRTP_Derived> {
public:
    void implementation() {
        int sum = 0;
        for (int i = 0; i < 100; ++i) {
            sum += i; // 添加一些计算
        }
    }
};

// 基于虚函数的基类
class Base {
public:
    virtual void interface() {
        std::cout << "Base.\n";
    }
protected:
    int x = 0;
};

class Derived : public Base {
public:
    void interface() override {
        std::cout << "D1.\n";
    }
};

class Derived2 : public Base
{
public:
    void interface() override {
        std::cout << "D2.\n";
    }
};

// CRTP benchmark
static void BM_CRTP(benchmark::State& state) {
    CRTP_Derived derived;
    for (auto _ : state) {
        derived.interface();  // 测试 CRTP 调用
    }
}

// Virtual function benchmark
static void BM_Virtual(benchmark::State& state) {
    Derived derived;
    for (auto _ : state) {
        derived.interface();  // 测试虚函数调用
    }
}

BENCHMARK(BM_CRTP);
BENCHMARK(BM_Virtual);

void A()
{
    std::cout << "A\n";
}

typedef void(Base::*Func)();

int main()
{
    Base* derivedptr = new Derived();
    uintptr_t derivedPtrValue = reinterpret_cast<uintptr_t>(derivedptr);
    std::cout << std::hex << derivedPtrValue << "\n";
    
    Base* derived2ptr = new Derived2();
    uintptr_t derived2PtrValue = reinterpret_cast<uintptr_t>(derived2ptr);
    std::cout << std::hex << derived2PtrValue << "\n";

    derivedptr->interface();
    derived2ptr->interface();

    uintptr_t** a = reinterpret_cast<uintptr_t**>(derivedptr);
    uintptr_t** b = reinterpret_cast<uintptr_t**>(derived2ptr);

    uintptr_t* vtableDerived = *a;
    uintptr_t* vtableDerived2 = *b;

    uintptr_t temp = *vtableDerived;
    *vtableDerived = *vtableDerived2;
    *vtableDerived2 = temp;

    derivedptr->interface();
    derived2ptr->interface();
    
}

#endif
