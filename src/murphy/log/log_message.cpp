/*
murphy/log/log_message.cpp
*/

module murphy.log.logger;

import <unordered_map>;
import <string>;

namespace murphy::log
{
	std::unordered_map<Level, std::string> LogMessage::level_to_string = {
				{Level::Debug,   "[DEBUG]"},
				{Level::Info,	 "[INFO]"},
				{Level::Warning, "[WARNING]"},
				{Level::Error,	 "[ERROR]"},
				{Level::Off,	 "[OFF]"},
	};
} //namespace gargantua::log