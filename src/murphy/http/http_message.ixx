/*
* murphy/http/http_message.ixx
* 
* PURPOSE: implementation of HTTP protocol.
* 
* CLASSES:
*	HeaderBody: header and body for Request and Response.
*	Request: represent a http request.
*	Response: represent a http response.
* 
* DESCRIPTION:
*	Implementation of response and request message of HTTP protol.
*	The two main functionalities are Serialize() and Parse().
*/

export module murphy.http.http_message;

import <concepts>;
import <string>;
import <unordered_map>;

import murphy.types;
import murphy.ds.buffer;


namespace murphy::http
{
	inline constexpr u32 crlf_size = 2;
	inline constexpr u32 space_size = 1;
	inline constexpr u32 colon_size = 1;

	struct HeaderBody
	{
	protected:
		// These sizes take into account the spaces, \r\n and so on.
		auto HeaderSizeBytes() -> u32;
		auto BodySizeBytes() -> u32;

		auto SerializeHeaders(ds::DynamicBuffer& buffer) -> void;
		auto SerializeBody(ds::DynamicBuffer& buffer) -> void;

	public:
		std::unordered_map<std::string, std::string> fields;
		std::string body;
	};


	export 
	struct Request final : public HeaderBody
	{	
		// Serialize the request message into bytes to be sent to a server.
		auto Serialize() -> ds::DynamicBuffer;

		// Parse a request message and create a struct.
		static auto Parse(std::string msg) -> Request;

		// Request line
		std::string method;
		std::string url;
		std::string version;
	};


	export 
	struct Response final : public HeaderBody
	{
		// Serialize the response message into bytes to be sent to a client.
		auto Serialize() -> ds::DynamicBuffer;
		
		// Parse a response message and create a struct.
		static auto Parse(std::string msg) -> Response;

		// Status line
		std::string version;
		std::string status_code;
		std::string status_text;
	};


	export
	template <typename T>
	concept is_message = std::same_as<T, Request> || std::same_as<T, Response>;
} // namespace murphy::http