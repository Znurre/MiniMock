#ifndef MOCK_H
#define MOCK_H

#include <QtTest>

const int LIMIT = 128;

template<class T>
class Mock
{
	template<class TReturn, class ...TArguments>
	using Function = TReturn (T::*)(TArguments...);

	typedef void (*FunctionPointer)();

	class Object
	{
		public:
			Object(void **newVf)
			{
				void (Object::*stubMethod)() = &Object::stub;

				for (int i = 0; i < LIMIT; i++)
				{
					newVf[i] = *(void **)&stubMethod;
				}

				*(void **)this = newVf;
			}

			void stub()
			{
				QFAIL("Attempted to call into a stub method");
			}
	};

	template<class TReturn, class ...TArguments>
	class MockSetup
	{
		using ParameterCallback = TReturn (*)(T *, TArguments...);
		using Callback = TReturn (*)();

		public:
			MockSetup(FunctionPointer *vfptr, size_t index)
				: m_vfptr(vfptr)
				, m_index(index)
			{
			}

			void callback(ParameterCallback callback)
			{
				m_vfptr[m_index] = *(FunctionPointer *)&callback;
			}

			void simpleCallback(Callback callback)
			{
				m_vfptr[m_index] = *(FunctionPointer *)&callback;
			}

		private:
			FunctionPointer *m_vfptr;

			size_t m_index;
	};

	public:
		Mock()
			: m_object((void **)m_vfptr)
		{
		}

		template<class ...TArguments>
		MockSetup<void, TArguments...> setup(Function<void, TArguments...> function)
		{
			const size_t index = getIndex(function);

			return MockSetup<void, TArguments...>(m_vfptr, index);
		}

		template<class TReturn, class ...TArguments>
		MockSetup<TReturn, TArguments...> setup(Function<TReturn, TArguments...> function)
		{
			const size_t index = getIndex(function);

			return MockSetup<TReturn, TArguments...>(m_vfptr, index);
		}

		template<class TFunction>
		size_t getIndex(TFunction function)
		{
			return ((size_t)*(FunctionPointer *)&function) / sizeof(FunctionPointer);
		}

		T &object()
		{
			return (T&)m_object;
		}

	private:
		FunctionPointer m_vfptr[LIMIT];
		Object m_object;
};

#endif // MOCK_H
