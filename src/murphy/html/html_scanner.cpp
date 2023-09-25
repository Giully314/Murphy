/*
* murphy/html/html_scanner.cpp
*/


module murphy.html.html_scanner;

import <cctype>;


namespace murphy::html
{
	auto Scanner::MakeCodePointToken(Character c) -> Token
	{
		auto token = Token{ TokenType::Character };
		token.SetCodePoint(c);
		return token;
	}

	auto Scanner::MakeEOFToken() -> Token
	{
		auto token = Token{ TokenType::EndOfFile };
		return token;
	}

	
	auto Scanner::Scan() -> Token
	{
		emit_token = std::nullopt;
		current_char = std::nullopt;

		while (true)
		{
			switch (current_state)
			{
			case State::Data:
				DataState();
				break;
			
			case State::RCDATA:
				RCDATAState();
				break;
			
			case State::RawText:
				RAWTEXTState();
				break;
			}


			if (emit_token)
			{
				return *emit_token;
			}
		}
	}


	auto Scanner::NullCharError(Character c) -> void
	{
		// This state is unexpected null char parse error.
		// emit the current char as a char token.
		Error::ScannerNullCharParseError(current_state, current_idx);
		// TODO: set scanner internal error.
		emit_token = MakeCodePointToken(c);
	}


	auto Scanner::DataState() -> void
	{
		if (current_char = Advance(); current_char)
		{
			switch (*current_char)
			{
			case '&':
			{
				return_state = State::Data;
				current_state = State::CharacterReference;
				return;
			}
			case '<':
			{
				current_state = State::TagOpen;
				return;
			}
			case '\0':
			{
				NullCharError();
				return;
			}
			default:
			{
				// emit the current input character as a char token.
				emit_token = MakeCodePointToken(*current_char);
				return;
			}
			}
		}
		else
		{
			// emit an end of file token.
			emit_token = MakeEOFToken();
		}
	}
	

	auto Scanner::RCDATAState() -> void
	{
		if (current_char = Advance(); current_char)
		{
			switch (*current_char)
			{
			case '&':
			{
				return_state = State::RCDATA;
				current_state = State::CharacterReference;
				return;
			}
			case '<':
			{
				current_state = State::RCDATALessThanSign;
				return;
			}
			case '\0':
			{
				NullCharError(*current_char);
				return;
			}
			default:
			{
				// emit the current input character as a char token.
				emit_token = MakeCodePointToken(*current_char);
				return;
			}
			}
		}
		else
		{
			emit_token = MakeEOFToken();
		}
	}


	auto Scanner::RAWTEXTState() -> void
	{
		if (current_char = Advance(); current_char)
		{
			switch (*current_char)
			{
			case '<':
			{
				current_state = State::RAWTEXTLessThanSign;
				return;
			}
			case '\0':
			{
				NullCharError(*current_char);
				return;
			}
			default:
			{
				// emit the current input character as a char token.
				emit_token = MakeCodePointToken(*current_char);
				return;
			}
			}
		}
		else
		{
			emit_token = MakeEOFToken();
		}
	}


	auto Scanner::ScriptDataState() -> void
	{
		if (current_char = Advance(); current_char)
		{
			switch (*current_char)
			{
			case '<':
			{
				current_state = State::ScriptDataLessThanSign;
				return;
			}
			case '\0':
			{
				// Replacement character
				NullCharError(0xfffd);
				return;
			}
			default:
			{
				emit_token = MakeCodePointToken(*current_char);
				return;
			}
			}
		}
		else
		{
			emit_token = MakeEOFToken();
		}
	}

} // namespace murphy::html