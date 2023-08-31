/*
* murphy/html/html_scanner.ixx
* 
* PURPOSE: html tokenizer.
* 
* CLASSES:
* 
* DESCRIPTION:
* 
* 
* RESOURCES:
*	https://html.spec.whatwg.org/multipage/parsing.html#tokenization
* 
*/

export module murphy.html.html_scanner;

import <string>;
import <string_view>;
import <concepts>;
import <utility>;
import <optional>;
import <cctype>;

import murphy.types;
import murphy.html.html_token;
import murphy.html.html_errors;


namespace murphy::html
{
	enum class State
	{
        Data,
        RCDATA,
        RAWTEXT,
        ScriptData,
        PLAINTEXT,
        TagOpen,
        EndTagOpen,
        TagName,
        RCDATALessThanSign,
        RCDATAEndTagOpen,
        RCDATAEndTagName,
        RAWTEXTLessThanSign,
        RAWTEXTEndTagOpen,
        RAWTEXTEndTagName,
        ScriptDataLessThanSign,
        ScriptDataEndTagOpen,
        ScriptDataEndTagName,
        ScriptDataEscapeStart,
        ScriptDataEscapeStartDash,
        ScriptDataEscaped,
        ScriptDataEscapedDash,
        ScriptDataEscapedDashDash,
        ScriptDataEscapedLessThanSign,
        ScriptDataEscapedEndTagOpen,
        ScriptDataEscapedEndTagName,
        ScriptDataDoubleEscapeStart,
        ScriptDataDoubleEscaped,
        ScriptDataDoubleEscapedDash,
        ScriptDataDoubleEscapedDashDash,
        ScriptDataDoubleEscapedLessThanSign,
        ScriptDataDoubleEscapeEnd,
        BeforeAttributeName,
        AttributeName,
        AfterAttributeName,
        BeforeAttributeValue,
        AttributeValueDoubleQuoted,
        AttributeValueSingleQuoted,
        AttributeValueUnquoted,
        AfterAttributeValueQuoted,
        SelfClosingStartTag,
        BogusComment,
        MarkupDeclarationOpen,
        CommentStart,
        CommentStartDash,
        CommentState,
        CommentLessThanSign,
        CommentLessThanSignBang,
        CommentLessThanSignBangDash,
        CommentLessThanSignBangDashDash,
        CommentEndDash,
        CommentEnd,
        CommentEndBang,
        DOCTYPE,
        BeforeDOCTYPEName,
        DOCTYPEName,
        AfterDOCTYPEName,
        AfterDOCTYPEPublicKeyword,
        BeforeDOCTYPEPublicIdentifier,
        DOCTYPEPublicIdentifierDoubleQuoted,
        DOCTYPEPublicIdentifierSingleQuoted,
        AfterDOCTYPEPublicIdentifier,
        BetweenDOCTYPEPublicAndSystemIdentifiers,
        AfterDOCTYPESystemKeyword,
        BeforeDOCTYPESystemIdentifier,
        DOCTYPESystemIdentifierDoubleQuoted,
        DOCTYPESystemIdentifierSingleQuoted,
        AfterDOCTYPESystemIdentifier,
        BogusDOCTYPE,
        CDATASection,
        CDATASectionBracket,
        CDATASectionEnd,
        CharacterReference,
        NamedCharacterReference,
        AmbiguousAmpersand,
        NumericCharacterReference,
        HexadecimalCharacterReferenceStart,
        DecimalCharacterReferenceStart,
        HexadecimalCharacterReference,
        DecimalCharacterReference,
        NumericCharacterReferenceEnd,
	};



	export 
	class Scanner
	{
	public:
        using CharacterType = char;


        Scanner(std::string_view text_) : text(text_), start_idx(0), current_idx(0),
            state(State::Data), return_state(State::Data)
		{

		}

        auto Scan() -> Token;

		auto IsEnd() const noexcept -> bool
		{
			return text.size() == current_idx;
		}


    // State set functions based on https://html.spec.whatwg.org/multipage/parsing.html#data-state
    private:
        auto DataState() -> std::optional<Token>;
        auto CharacterReferenceState() -> void;
        

        auto FlushCodePointsConsumedAsReference() -> std::optional<Token>;

	private:
		auto MakeToken(TokenType type) -> Token;
        auto MakeCodePointToken(Character c) -> Token;
        auto MakeEOFToken() -> Token;

		auto SkipWhiteSpaces() -> void;


        auto IsAlphaNumeric(CharacterType ch) -> bool
        {
            return std::isalnum(static_cast<unsigned char>(ch));
        }

        
        // Read current character and advance the current_idx.
        auto Advance() -> std::optional<CharacterType>
        {
            if (current_idx >= text.size())
            {
                return std::nullopt;
            }
            return text[current_idx++];
        }

        // Move the current idx back by 1 position;
        auto Back() -> void
        {
            if (current_idx > 0)
            {
                --current_idx;
            }
        }


	private:
		// TODO: possible to change in string_view (so no ownership)?
		std::string_view text;
		u32 start_idx = 0; // points to the start of the current char in NextScan
		u32 current_idx = 0; // points to the current char
	    
        std::string temporary_buffer;

        State state = State::Data;
        State return_state = State::Data;
        
        Token current_token;
    };
} // namespace murphy::html