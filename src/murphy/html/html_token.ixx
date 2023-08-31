/*
* murphy/html/html/token.ixx
* 
* PURPOSE:
* 
* CLASSES:
* 
* DESCRIPTION:
*/

module;

#include <murphy/massert.hpp>

export module murphy.html.html_token;

import <string_view>;
import <vector>;
import <unordered_map>;
import <string>;
import <variant>;

import murphy.types;
import murphy.log;

export 
namespace murphy::html
{
	enum class TokenType : u8
	{
		Invalid,
		DOCTYPE, 
		StartTag, 
		EndTag, 
		Comment, 
		Character, 
		EndOfFile,
	};


	struct Attribute
	{
		std::string name;
		std::string value;
	};


	// TokenType::DOCTYPE
	struct Doctype
	{
		std::string name;
		std::string public_id;
		std::string system_id;
		bool force_quirks = false;
		bool missing = true;
	};


	// TokenType::StartTag
	// TokenType::EndTag
	struct Tag
	{
		std::string name;
		bool self_enclosing = false;
		std::vector<Attribute> attributes;
	};


	// TOkenType::Comment
	// TOkenType::Character
	struct Comment
	{
		std::string data;
	};

	
	using Character = u32;


	class Token
	{
	public:
		Token() = default;

		Token(TokenType type_) : type(type_)
		{
			switch (type)
			{
			case TokenType::Invalid:
			case TokenType::EndOfFile:
				data = Empty{}
				break;
			
			case TokenType::DOCTYPE:
				data = Doctype{};
				break;
			
			case TokenType::StartTag:
			case TokenType::EndTag:
				data = Tag{};
				break;

			case TokenType::Character:
				data = Character{ 0 };
				break;

			}
		}


		[[nodiscard]]
		constexpr
		auto IsDoctype() const noexcept -> bool
		{
			return type == TokenType::DOCTYPE;
		}

		[[nodiscard]]
		constexpr
		auto IsStartTag() const noexcept -> bool
		{
			return type == TokenType::StartTag;
		}


		[[nodiscard]]
		constexpr
		auto IsEndTag() const noexcept -> bool
		{
			return type == TokenType::EndTag;
		}

		[[nodiscard]]
		constexpr
		auto IsComment() const noexcept -> bool
		{
			return type == TokenType::Comment;
		}

		[[nodiscard]]
		constexpr
		auto IsCharacter() const noexcept -> bool
		{
			return type == TokenType::Character;
		}

		[[nodiscard]]
		constexpr
		auto IsEOF() const noexcept -> bool
		{
			return type == TokenType::EndOfFile;
		}


		[[nodiscard]]
		constexpr 
		auto Type() const noexcept -> TokenType
		{
			return type;
		}


		auto SetType(TokenType new_type) -> void
		{
			type = new_type;
		}



		auto SetCodePoint(Character c) -> void
		{
			CHECK(IsCharacter());
			std::get<Character>(data) = c;
		}




	private:
		TokenType type = TokenType::Invalid;

		// Monostate is used as the empty type.
		std::variant<Empty, Doctype, Character, Tag, Comment> data;
	};
} // namespace murphy::html