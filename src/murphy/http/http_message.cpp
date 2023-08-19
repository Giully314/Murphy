/*
* murphy/http/http_message.ixx
*/

module murphy.http.http_message;


namespace murphy::http
{

	// ********************* HEADER BODY *******************************

	auto HeaderBody::HeaderSizeBytes() -> u32
	{
		u32 size = 0;
		for (const auto& [key, value] : fields)
		{
			// for ':', space after ':' and \r\n at the end.
			size += key.size() + value.size() + colon_size + space_size + crlf_size;
		}
		return size;
	}

	auto HeaderBody::BodySizeBytes() -> u32
	{
		return body.size() + 
	}

	auto HeaderBody::SerializeHeaders(ds::DynamicBuffer& buffer) -> void
	{
		for (const auto& [key, value] : fields)
		{
			buffer.Append(key.data(), key.size());
			buffer.Append(": ", colon_size + space_size);
			buffer.Append(value.data(), value.size());
			buffer.Append("\r\n", crlf_size);
		}
	}

	auto HeaderBody::SerializeBody(ds::DynamicBuffer& buffer) -> void
	{
		if (body.size() > 0)
		{

		}
	}


	// ********************** REQUEST ******************************************

	auto Request::Serialize() -> ds::DynamicBuffer
	{
		// First compute the size in bytes of the message to pre-allocate memory.
		u32 size = 0;
		size += method.size();
		size += space_size;
		size += url.size();
		size += space_size;
		size += version.size();
		size += crlf_size; // /r/n.

		size += HeaderSizeBytes();

		// end of headers is: "\r\n\r\n"
		size += 2 * crlf_size;

		size += BodySizeBytes();
		
		ds::DynamicBuffer buffer;
		buffer.Reserve(size);
		
		// Serialize request line.
		buffer.Append(method.data(), method.size());
		buffer.Append(" ", space_size);
		buffer.Append(url.data(), url.size());
		buffer.Append(" ", space_size);
		buffer.Append(version.data(), version.size());
		buffer.Append("\r\n", crlf_size);
	
		SerializeHeaders(buffer);

		// End of headers
		buffer.Append("\r\n\r\n", 2 * crlf_size);
		
		SerializeBody(buffer);

		return buffer;
	}




	// ******************** RESPONSE *********************************


	auto Response::Serialize() -> ds::DynamicBuffer
	{
		// First compute the size in bytes of the message to pre-allocate memory.
		u32 size = 0;
		size += version.size();
		size += space_size;
		size += status_code.size();
		size += space_size;
		size += status_text.size();
		size += crlf_size; // /r/n.

		size += HeaderSizeBytes();

		// end of headers is: "\r\n\r\n"
		size += 2 * crlf_size;

		size += BodySizeBytes();

		ds::DynamicBuffer buffer;
		buffer.Reserve(size);

		// Serialize request line.
		buffer.Append(version.data(), version.size());
		buffer.Append(" ", space_size);
		buffer.Append(status_code.data(), status_code.size());
		buffer.Append(" ", space_size);
		buffer.Append(status_text.data(), status_text.size());
		buffer.Append("\r\n", crlf_size);

		SerializeHeaders(buffer);

		// End of headers
		buffer.Append("\r\n\r\n", 2 * crlf_size);

		SerializeBody(buffer);

		return buffer;
	}
} // namespace murphy::http