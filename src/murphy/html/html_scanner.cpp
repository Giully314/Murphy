/*
* murphy/html/html_scanner.cpp
*/


module murphy.html.html_scanner;

import <cctype>;

namespace murphy::html
{
	auto Scanner::MakeToken(TokenType type) -> Token
	{
		Token t{ type };
		return t;
	}

	auto Scanner::MakeEOFToken() -> Token
	{
		Token t{ TokenType::EndOfFile };
		return t;
	}


	auto Scanner::MakeCodePointToken(Character c) -> Token
	{
		Token t{ TokenType::Character };
		t.SetCodePoint(c);
		return t;
	}



	auto Scanner::FlushCodePointsConsumedAsReference() -> std::optional<Token>
	{
		switch (return_state)
		{
		case State::AttributeValueDoubleQuoted:
		case State::AttributeValueSingleQuoted:
		case State::AttributeValueUnquoted:
			// from temporary_buffer to current attribute value
			return std::nullopt;

		default:
			return 
		}
	}


	auto Scanner::SkipWhiteSpaces() -> void
	{
		constexpr static auto f = [](char ch) -> bool
			{
				return std::isspace(static_cast<unsigned char>(ch));
			};

		while (!IsEnd() && f(text[current_idx++]));
	}


	auto Scanner::Scan() -> Token
	{
		//auto c = Advance();
		//if (!c)
		//{
		//	// Emit EOF token.
		//}

		//current_char = *c;
		while (true) 
		{
			switch (state)
			{
			case State::Data:
				if (auto t = DataState(); t) return *t;
				break;

			case State::CharacterReference:

			}


		}
	}


	auto Scanner::DataState() -> std::optional<Token>
	{
		auto c = Advance();

		// This is EOF
		if (!c)
		{
			return MakeEOFToken();
		}

		switch (*c)
		{
		case '&':
			return_state = State::Data;
			state = State::CharacterReference;
			break;

		case '<':
			state = State::RCDATALessThanSign;
			break;
		case '\0':
			Error::ScannerError("Null character error");
			auto t = MakeToken(TokenType::Character);
			t.SetCodePoint(*c);
			return t;
		default:
			return MakeCodePointToken(*c);
		}

		return std::nullopt;
	}

	auto Scanner::CharacterReferenceState() -> void
	{
		temporary_buffer = "&";
		auto c = Advance();
		
		// error 
		if (!c)
		{
			// anything else 
		}
		

		if (IsAlphaNumeric(*c))
		{
			Back();
			state = State::NamedCharacterReference;
			return;
		}
		else if (*c == '#')
		{
			temporary_buffer.push_back(*c);
			state = State::NumericCharacterReference;
			return;
		}
		else
		{
			FlushCodePointsConsumedAsReference();
			state = return_state;
			Back();
		}
	}

} // namespace murphy::html