# QtMock
Super simple mocking framework for QtTest

The other day I was working on a C++ project and wanted to write some unit tests.
I quickly ran into wanting to set up mocks of a few objects and was amazed to find that there were no good, simple mocking frameworks for C++, so I set out to write one myself, inspired by the excellent Moq framework for C#.

With my project being this simple I am sure there are things that won't work.
For instance, I have no idea how portable this solution is. It's tested and works on GCC 7.2 at least.

Example of usage
----------------

```C++
class IInterface
{
	public:
		virtual void foo() = 0;
		virtual void bar(int oof) = 0;

		virtual int baz() = 0;
};
```

```C++
Mock<IInterface> interfaceMock;

interfaceMock
	.setup(&IInterface::foo)
	.simpleCallback([]
	{
		qDebug() << "foo";
	});

interfaceMock
	.setup(&IInterface::bar)
	.callback([](IInterface *, int oof)
	{
		qDebug() << oof;
	});

interfaceMock
	.setup(&IInterface::baz)
	.simpleCallback([]
	{
		return 23;
	});

interfaceMock.object().foo();
interfaceMock.object().bar(22);

qDebug() << interfaceMock.object().baz();
```

Running this will produce the following output:

foo\
22\
23

Support for verifying invocations will come in due time.
