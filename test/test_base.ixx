export module murphy.test.test_base;

import murphy.log;

export
namespace murphy::test
{
	class TestBase 
	{
	public:
		TestBase()
		{
			console_log.SetLoggerName("TestBase");
			console_log.SetLevel(log::Level::Debug);

			mconsole_log.SetLoggerName("MTestBase");
			mconsole_log.SetLevel(log::Level::Debug);
		}

	protected:
		log::Logger<log::ConsoleStream> console_log;
		log::Logger<log::MutexConsoleStream> mconsole_log;
	};
} // namespace murphy::test