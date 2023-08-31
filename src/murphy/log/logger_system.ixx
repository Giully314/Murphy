/*
* gargantua/log/logger_system.ixx
* 
* PURPOSE: global access to the game engine logger (used only for local debug).
* 
* TODO:
*	- use the Singleton class.
*/

export module murphy.log.logger_system;

import murphy.types;
import murphy.log.logger;
import murphy.log.console_stream;


namespace murphy::log
{
	export
	class LoggerSystem
	{
	public:
		static auto Instance() -> Logger<ConsoleStream>&
		{
			static Logger<ConsoleStream> logger;
			return logger;
		}

	private:
		LoggerSystem() = default;
		LoggerSystem(const LoggerSystem&) = delete;
	};
} // namespace gargantua::log