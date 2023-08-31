/*
* murphy/html/html_parser.cpp
*/


module murphy.html.html_parser;


namespace murphy::html
{
	auto Parser::ParseToDOM() -> unique_res<dom::Node>
	{
		auto root = CreateUniqueRes<dom::Node>();
		state = ParserState::StartTag;

		std::optional<Token> token;
		do
		{
			//token = scanner.NextToken();


		} while (token);

		return root;
	}

} // namespace murphy::html