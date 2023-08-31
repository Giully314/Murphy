/*
* murphy/html/html_errors.ixx
* 
* PURPOSE:
* 
* CLASSES:
* 
* DESCRIPTION:
* 
*/

export module murphy.html.html_errors;

import <string_view>;
import <utility>;

import murphy.log;


namespace murphy::html
{
	export
	struct Error
	{
		template <typename ...Args>
		static auto ScannerError(std::string_view msg, Args&& ...args) -> void
		{
			auto& logger = log::LoggerSystem::Instance();
			logger.Error("Scanner error");
			logger.Error(msg, std::forward<Args>(args)...);
		}
	};
} // namespace murphy::html