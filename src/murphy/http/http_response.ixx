/*
* murphy/http/http_response.ixx
*
* PURPOSE: Model the http response message.
*
* CLASSES:
*	Response: Class for the object representation of the http response.
*
* DESCRIPTION:
*	The Response class manages all the operations of an http message. Access of the fields,
*	serialization for sending messages or parse for reading messages.
* 
* 
* TODO:
*	- add error checking.
* 
* USAGE:
*	Response<MyHeader, MyBody> response;
*	response.Parse(buffer);
*	
*	if (response.Has("Content-length"))
*	{
*		// access the body.
*	}
* 
*	
* 
*	//--------
*	auto buffer = response.Serialize();
*	server.Send(buffer).
* 
*/

export module murphy.http.http_response;

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
	class [[nodiscard]] Response final : public THeader, private TBody::ValueType
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
		auto Version() const noexcept -> std::string_view
		{
			return version;
		}

		[[nodiscard]]
		auto StatusCode() const noexcept -> std::string_view
		{
			return status_code;
		}


		[[nodiscard]]
		auto StatusText() const noexcept -> std::string_view
		{
			return status_text;
		}

		
		auto SetStatusText(std::string_view s) -> void
		{
			status_text = alg::Str::ToLower(s);
		}


		auto SetVersion(std::string_view v) -> void
		{
			version = alg::Str::ToLower(v);
		}

		
		auto SetStatusCode(std::string_view s) -> void
		{
			status_code = alg::Str::ToLower(s);
		}

	
		// Serialize the response message into bytes to be sent to a client.
		[[nodiscard]]
		auto Serialize() -> ds::DynamicBuffer
		{
			// First compute the size in bytes of the message to pre-allocate memory.
			u64 size = 0;
			size += version.size() + status_code.size() + status_text.size() + 2 * space_size
				+ crlf_size;
			
			size += BaseHeader::Size();

			// end of headers is: "\r\n"
			size += crlf_size;

			// TODO: check if there is content-length in header and add to the size for the body.

			ds::DynamicBuffer buffer;
			buffer.Reserve(size);

			// Serialize request line.
			buffer.Append(version.data(), version.size());
			buffer.Append(" ", space_size);
			buffer.Append(status_code.data(), status_code.size());
			buffer.Append(" ", space_size);
			buffer.Append(status_text.data(), status_text.size());
			buffer.Append("\r\n", crlf_size);

			BaseHeader::Serialize(buffer);

			// End of headers
			buffer.Append("\r\n", crlf_size);

			BaseBody::Serialize(buffer, Body());

			return buffer;
		}

		// Parse a response message and create a struct.
		auto Parse(ds::NonOwnedBuffer b) -> void
		{
			std::string_view msg{ b.Data(), b.Size() };

			// General notes: the http message ends with \r\n\r\n 
			// (one is from a header, the other is to signal blank line).

			// Parse request line.
			// The request line is composed of 3 parts: method, url, version. 
			// Example: "method url version\r\n"
			auto end_of_status_line = msg.find("\r\n");
			if (end_of_status_line == std::string_view::npos)
			{
				// throw
			}

			auto status_line = msg.substr(0, end_of_status_line);

			// Extract method.
			u64 i = 0;
			u64 f = status_line.find(" ");
			// No space found, bad request line.
			if (f == std::string_view::npos)
			{
				// throw
			}
			version = std::string{ status_line.substr(i, f - i) };

			// Extract url.
			i = f + 1;
			f = status_line.find(" ", i);
			// No space found, bad request line.
			if (f == std::string_view::npos)
			{
				// throw
			}
			status_code = std::string{ status_line.substr(i, f - i) };

			i = f + 1;
			// We already did the check at the start for the \r\n so we can skip it now.
			status_text = std::string{ status_line.substr(i, end_of_status_line - i) };


			// Parse the http fields.
			auto end_of_fields_pos = msg.find("\r\n\r\n");
			// No end of http message, error.
			if (end_of_fields_pos == std::string_view::npos)
			{
				// throw
			}

			if (end_of_fields_pos - end_of_status_line == 0)
			{
				// No headers.
				return;
			}
			// +2 because we skip \r\n
			end_of_status_line += 2;
			ds::NonOwnedBuffer headers(b.Data() + end_of_status_line, end_of_fields_pos - end_of_status_line);

			// TODO: Check for errors.
			BaseHeader::Parse(headers);

			// TODO: check for errors.
			BaseBody::Parse(b, Body());
		}


		[[nodiscard]]
		auto ToString() -> std::string
		{
			auto s = std::format("{} {} {}\n", version, status_code, status_text);
			s += BaseHeader::ToString();
			return s;
		}

	private:
		// Status line
		std::string version;
		std::string status_code;
		std::string status_text;
	};
} // namespace murphy::http