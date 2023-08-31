/*
* murphy/alg/string_algorithms.ixx
* 
* PURPOSE: implementation of string algorithms.
*
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
		// Create a new string from s and transform every character to lower representation.
		// Complexity: O(n).
		[[nodiscard]]
		constexpr 
		static auto ToLower(const std::string_view s) -> std::string
		{
			std::string res{ s };
			std::ranges::transform(res, res.begin(),
				[](unsigned char c) -> unsigned char {return std::tolower(c); });
			return res;
		}
	};
	
}