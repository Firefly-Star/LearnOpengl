#pragma once


namespace Firefly
{
		class A
		{
		public:
			static A& GetInstance()
			{
				static A instance;
				return instance;
			}
		};
}