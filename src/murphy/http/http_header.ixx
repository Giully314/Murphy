/*
* murphy/http/http_header.ixx
* 
* PURPOSE: Header container for the http headers.
* 
* CLASSES:
*	EmptyHeader: empty header.
*	MapHeader: header container using a std::unordered_map.
* 
* DESCRIPTION:
*	The headers of an http message are of the form: "field: value\r\n". The container
*	keeps the pair field value and during serialization adds the additional characters
*	specified by the protocol.
* 
* TODO:
*	- check for errors.
* 
*
* USAGE:
*	
*	Note that MyHeader must satisfy the concept body_container.
*	
*	// If ValueType is an empty type, no additional memory is used.
*	class MyRequest : public MyHeader, private MyBody::ValueType
*	{
*	// definitions...
*	// Can use the function from my header.
*	
*	};
* 
*/

export module murphy.http.http_header;

import <unordered_map>;
import <string_view>;
import <string>;
import <format>;

import murphy.types;
import murphy.alg.string_algorithms;
import murphy.http.http_utility;
import murphy.ds.buffer;


export
namespace murphy::http
{
	class EmptyHeader
	{
	public:
		auto Set(std::string_view f, std::string_view v) -> void
		{

		}

		[[nodiscard]]
		auto operator[](std::string_view) const -> std::string_view
		{
			return {};
		}

		[[nodiscard]]
		auto HeaderSize() const -> u64
		{
			return 0;
		}
	};


	class MapHeader
	{
	public:

		// Set field value representing the header: "field: value\r\n".
		auto Set(std::string_view f, std::string_view v) -> void
		{
			fields[alg::Str::ToLower(f)] = alg::Str::ToLower(v);
		}


		// Get the value of the field. If the field is not present create a new pair,
		// field: value and return the empty string.
		[[nodiscard]]
		auto operator[](std::string_view f) -> std::string_view
		{
			return fields[alg::Str::ToLower(f)];
		}


		// Check if the field is registered.
		[[nodiscard]]
		auto Has(std::string_view f) const -> bool
		{
			return fields.contains(alg::Str::ToLower(f));
		}


		// Return the size in bytes of all the headers.
		[[nodiscard]]
		auto Size() const -> u64
		{
			u64 size = 0;
			for (const auto& [key, value] : fields)
			{
				// for ':', space after ':' and \r\n at the end.
				size += key.size() + value.size() + colon_size + space_size + crlf_size;
			}
			return size;
		}


		// Serialize the headers in binary.
		auto Serialize(ds::DynamicBuffer& db) -> void
		{
			for (const auto& [key, value] : fields)
			{
				db.Append(key.data(), key.size());
				db.Append(": ", colon_size + space_size);
				db.Append(value.data(), value.size());
				db.Append("\r\n", crlf_size);
			}
		}


		// Parse the bytes inside the buffer and reconstruct
		// the headers. The buffer is used only as read only.
		// TODO: what if there is a bad header? Ignore, report error or throw? 
		auto Parse(ds::NonOwnedBuffer& rb) -> void
		{
			std::string_view msg{ rb.Data(), rb.Size() };
			for (u64 i = 0, j = msg.find("\r\n", i);
				j != std::string_view::npos || j - i == 2; // end of the headers.
				i = j+2, j = msg.find("\r\n", i)) // i = j + 2 because we skip \r\n
			{
				auto header = msg.substr(i, j - i);
				if (auto colon_pos = header.find(":"); colon_pos != std::string_view::npos)
				{
					std::string field{ header, 0, colon_pos };

					// +2 to skip ": ".
					auto value_pos = colon_pos + 2;
					std::string value{ header, value_pos, j - i };

					// Add the header to the map. TODO: what if already exists?
					fields.emplace(std::move(field), std::move(value));
				}
				else
				{
					// report error
				}
			}
		}

		// only for debug
		[[nodiscard]]
		auto ToString() const -> std::string
		{
			std::string s;
			for (const auto& [key, value] : fields)
			{
				s += std::format("{}: {}\n", key, value);
			}
			return s;
		}

	private:
		std::unordered_map<std::string, std::string> fields;
	};
} // namespace murphy::http


