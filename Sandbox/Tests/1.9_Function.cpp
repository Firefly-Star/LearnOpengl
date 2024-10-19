#include "Entrypoint.h"
#if CHECK_ACTIVE(1, 9, 1)

#include <iostream>
#include <memory>

#if 1
template<typename ReturnType, typename... Args>
struct FuncBase
{
	virtual ~FuncBase() = default;
	virtual ReturnType Invoke(Args... args) = 0;
};

template<typename Callable, typename ReturnType, typename... Args>
struct FunctionHolder : public FuncBase<ReturnType, Args...>
{
	Callable callable;

	FunctionHolder(Callable c): callable(c) {}

	virtual ~FunctionHolder() = default;
	virtual ReturnType Invoke(Args... args) override
	{
		return callable(std::forward<Args>(args)...);
	}
};

template<typename ReturnType, typename... Args>
class function
{
public:
	template<typename Callable>
	function(Callable c)
		:callableBase(std::make_unique<FunctionHolder<Callable, ReturnType, Args...>>(c))
	{
	}

	ReturnType operator()(Args... args)
	{
		return callableBase->Invoke(args...);
	}

private:
	std::unique_ptr<FuncBase<ReturnType, Args...>> callableBase;
};
#endif


int main()
{
	auto f = [](int a, int b) {return a + b; };
	function<int, int, int> container(f);
	std::cout << container(2, 3) << "\n";
}



#endif