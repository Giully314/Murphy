/*
gargantua/log/logger.ixx

PURPOSE: Logger class.

CLASSES:
	Logger: Log message using a stream.

DESCRIPTION:
	A Logger is a class that allows to register messages and send them to the chosen stream.
	Messages are formatted using std::format.

USAGE:
	Logger<CoonsoleStream> console_logger;
	console_logger.Debug("debug message {}", code);
*/

export module murphy.log.logger;
export import :level;
export import :log_message;

import <string>;
import <source_location>;
import <string_view>;
import <utility>;
import <format>;


namespace murphy::log
{
	// TODO: requirements for StreamType.
	export 
	template <typename StreamType>
	class Logger
	{
	public:
		Logger() : level(Level::Debug), stream(), logger_name("Base")
		{

		}


		auto Flush() -> void
		{
			stream.Flush();
		}


		auto SetLoggerName(std::string name) -> void
		{
			logger_name = std::move(name);
		}

		auto GetLoggerName() const noexcept -> std::string_view
		{
			return logger_name;
		}


		auto SetLevel(Level level) -> void
		{
			this->level = level;
		}

		auto GetLevel() const noexcept -> Level
		{
			return level;
		}


		template <typename ...Args>
		auto Log(std::source_location loc, Level level, std::string_view msg, Args&& ...args) const -> void
		{
			if (ShouldLog(level))
			{
				std::string formatted_string = std::vformat(msg, std::make_format_args(args...));

				LogMessage lm{ logger_name, loc, level, formatted_string };
				stream << lm;
			}
		}

		template <typename ...Args>
		auto Log(Level level, std::string_view msg, Args&& ...args) const -> void
		{
			Log(std::source_location{}, level, msg, std::forward<Args>(args)...);
		}


		template <typename ...Args>
		auto Debug(std::string_view msg, Args&& ...args) const -> void
		{
			Log(std::source_location{}, Level::Debug, msg, std::forward<Args>(args)...);
		}

		template <typename ...Args>
		auto Info(std::string_view msg, Args&& ...args) const -> void
		{
			Log(std::source_location{}, Level::Info, msg, std::forward<Args>(args)...);
		}

		template <typename ...Args>
		auto Warning(std::string_view msg, Args&& ...args) const -> void
		{
			Log(std::source_location{}, Level::Warning, msg, std::forward<Args>(args)...);
		}


		template <typename ...Args>
		auto Error(std::string_view msg, Args&& ...args) const -> void
		{
			Log(std::source_location{}, Level::Error, msg, std::forward<Args>(args)...);
		}


	private:
		auto ShouldLog(Level l) const noexcept -> bool
		{
			return l >= level;
		}

		Level level;
		mutable StreamType stream;
		std::string logger_name;
	}; 
} //namespace gargantua::log