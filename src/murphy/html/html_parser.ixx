/*
* murphy/html/html_parser.ixx 
* 
* PURPOSE:
* 
* CLASSES:
* 
* DESCRIPTION:
* 
*/

export module murphy.html.html_parser;

import <concepts>;
import <string>;
import <utility>;

import murphy.types;
import murphy.dom.dom_node;
import murphy.html.html_token;
import murphy.html.html_scanner;

namespace murphy::html
{
	enum class ParserState : u8
	{
		StartTag, // This state is for the first '<'
		ReadAttribute,
		EndTag, // This state is for the '/' if we are in the StartTag
		ReadContent,
	};


	export 
	class Parser
	{
	public:
		template <std::constructible_from<std::string> U>
		Parser(U&& text) : scanner(std::forward<U>(text))
		{

		}

		// Return the root node.
		auto ParseToDOM() -> unique_res<dom::Node>;

	private:
		Scanner scanner;
		ParserState state = ParserState::StartTag;
	};
} // namespace murphy::html