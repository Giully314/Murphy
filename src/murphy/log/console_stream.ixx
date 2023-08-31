/*
murphy/log/consoel_stream.ixx

PURPOSE: Console output stream.

CLASSES:
	ConsoleStream: std::cout wrapper class.

DESCRIPTION:
	This is a temporary class and should be used only for test.
*/

export module murphy.log.console_stream;

import <iostream>;
import <string_view>;


export namespace murphy::log
{
	//To use ONLY for debug on local machine.
	class ConsoleStream
	{
	public:
		auto Flush() -> ConsoleStream&
		{
			std::cout.flush();
			return *this;
		}
	}; 

	inline ConsoleStream& operator<<(ConsoleStream& os, std::string_view s)
	{
		std::cout << s;
		return os;
	}

} //namespace gargantua::log