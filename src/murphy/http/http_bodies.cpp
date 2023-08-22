/*
* murphy/http/http_bodies.cpp
*/

module murphy.http.http_bodies;

import <string_view>;

namespace murphy::http
{
	auto StringBody::Serialize(ds::DynamicBuffer& db, std::string& body) -> void
	{
		db.Append(body.data(), body.size());
	}

	auto StringBody::Parse(ds::NonOwnedBuffer& rb, std::string& body) -> void
	{
		std::string_view msg{ rb.Data(), rb.Size() };
		body = std::string(msg);
	}
} // namespace murphy::http