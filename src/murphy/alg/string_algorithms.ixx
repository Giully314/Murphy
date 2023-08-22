/*
* murphy/alg/string_algorithms.ixx
*/

export module murphy.alg.string_algorithms;

import <string_view>;
import <string>;
import <algorithm>;
import <cctype>;

namespace murphy::alg
{
	export 
	struct Str
	{
		[[nodiscard]]
		constexpr 
		static auto ToLower(std::string_view s) -> std::string
		{
			std::string res{ s };
			std::ranges::transform(res, res.begin(),
				[](unsigned char c) -> unsigned char {return std::tolower(c); });
			return res;
		}
	};
	
}