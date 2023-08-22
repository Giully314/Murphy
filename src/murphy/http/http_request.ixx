/*
* murphy/http/http_request.ixx
*
* PURPOSE: Model the http request message.
*
* CLASSES:
*	Request: Class for the object representation of the http request.
*
* DESCRIPTION:
*	The Request class manages all the operations of an http message. Access of the fields,
*	serialization for sending messages or parse for reading messages.
*
*
* TODO:
*	- add error checking.
*
* USAGE:
*	Request<MyHeader, MyBody> request;
*	request.Parse(buffer);
*	
*	if (request.Method() == "get")
*	{
*		...
*	}
*
*	//--------
*	request.SetMethod("GET");
*	request.SetURL("www.test.com");
*	auto buffer = request.Serialize();
*	client.Send(buffer).
*/

export module murphy.http.http_request;

import <string>;
import <utility>;
import <string_view>;
import <format>;


import murphy.types;
import murphy.ds.buffer;
import murphy.alg.string_algorithms;
import murphy.http.http_header;
import murphy.http.http_bodies;
import murphy.http.http_concepts;
import murphy.http.http_utility;

namespace murphy::http
{
	export
	template <header_container THeader = MapHeader, body_container TBody = StringBody>
	class [[nodiscard]] Request final : public THeader, TBody::ValueType
	{
	public:
		using BaseHeader = THeader;
		using BaseBody = TBody;
		using BodyType = typename TBody::ValueType;

		[[nodiscard]]
		auto Body() -> BodyType&
		{
			return *this;
		}

		[[nodiscard]]
		auto Body() const -> const BodyType&
		{
			return *this;
		}
		

		[[nodiscard]]
		auto Method() const noexcept -> std::string_view
		{
			return method;
		}

		[[nodiscard]]
		auto URL() const noexcept -> std::string_view
		{
			return url;
		}

		[[nodiscard]]
		auto Version() const noexcept -> std::string_view
		{
			return version;
		}

		auto SetMethod(std::string_view m) -> void
		{
			method = alg::Str::ToLower(m);
		}

		auto SetURL(std::string_view u) -> void
		{
			url = alg::Str::ToLower(u);
		}


		auto SetVersion(std::string_view v) -> void
		{
			version = alg::Str::ToLower(v);
		}

		[[nodiscard]]
		auto Serialize() -> ds::DynamicBuffer
		{
			// First compute the size in bytes of the message to pre-allocate memory.
			u64 size = 0;
			size += method.size() + url.size() + version.size() + 2 * space_size + crlf_size;

			size += BaseHeader::Size();

			// end of headers is: "\r\n"
			size += crlf_size;


			// TODO: check if there is content-length in header and add to the size for the body.

			ds::DynamicBuffer buffer;
			buffer.Reserve(size);

			// Serialize request line.
			buffer.Append(method.data(), method.size());
			buffer.Append(" ", space_size);
			buffer.Append(url.data(), url.size());
			buffer.Append(" ", space_size);
			buffer.Append(version.data(), version.size());
			buffer.Append("\r\n", crlf_size);

			BaseHeader::Serialize(buffer);

			// End of headers
			buffer.Append("\r\n", crlf_size);

			BaseBody::Serialize(buffer, Body());

			return buffer;
		}


		// TODO: add exceptions.
		auto Parse(ds::NonOwnedBuffer b) -> void
		{
			std::string_view msg{ b.Data(), b.Size() };

			// General notes: the http message ends with \r\n\r\n 
			// (one is from a header, the other is to signal blank line).

			// Parse request line.
			// The request line is composed of 3 parts: method, url, version. 
			// Example: "method url version\r\n"
			auto end_of_request_line = msg.find("\r\n");
			if (end_of_request_line == std::string_view::npos)
			{
				// throw
			}

			auto request_line = msg.substr(0, end_of_request_line);

			// Extract method.
			u64 i = 0;
			u64 f = request_line.find(" ");
			// No space found, bad request line.
			if (f == std::string_view::npos)
			{
				// throw
			}
			method = std::string{ request_line.substr(i, f - i) };

			// Extract url.
			i = f + 1;
			f = request_line.find(" ", i);
			// No space found, bad request line.
			if (f == std::string_view::npos)
			{
				// throw
			}
			url = std::string{ request_line.substr(i, f - i) };

			i = f + 1;
			// We already did the check at the start for the \r\n so we can skip it now.
			//f = request_line.find("\r\n", i); 
			version = std::string{ request_line.substr(i, end_of_request_line - i) };


			// Parse the http fields.
			auto end_of_fields_pos = msg.find("\r\n\r\n");
			// No end of http message, error.
			if (end_of_fields_pos == std::string_view::npos)
			{
				// throw
			}

			if (end_of_fields_pos - end_of_request_line == 0)
			{
				// No headers.
				return;
			}
			// +2 because we skip \r\n
			end_of_request_line += 2;
			end_of_fields_pos += 4;
			ds::NonOwnedBuffer headers(b.Data() + end_of_request_line, end_of_fields_pos - end_of_request_line);
			
			// TODO: Check for errors.
			BaseHeader::Parse(headers);

			// TODO: check for errors.
			BaseBody::Parse(b, Body());
		}


		// Debug only.
		[[nodiscard]]
		auto ToString() const -> std::string
		{
			std::string s = std::format("{} {} {}\n", method, url, version);
			s += BaseHeader::ToString();
			
			// header and body
			return s;
		}

	private:
		// Request line
		std::string method;
		std::string url;
		std::string version;
	};
} // namespace murphy::http