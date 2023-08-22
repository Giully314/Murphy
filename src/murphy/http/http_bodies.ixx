/*
* murphy/http/http_bodies.ixx
* 
* PURPOSE: model the body container for the http body.
* 
* CLASSES:
*	EmptyBody: an empty body.
*	StringBody: body of type std::string.	
* 
* DESCRIPTION:
*	These classes model the concept of body_container. A body container is a type
*	that store the content of the message body. The classes Request and Response
*	inherit from the body to apply, in case of empty body, the empty base class optimization
*	where no additional memory is used (with composition this would not be possible).
* 
* 
* USAGE:
*	
*	Note that MyBody must satisfy the concept body_container.
*	
*	// If ValueType is an empty type, no additional memory is used.
*	class MyRequest : public MyHeader, private MyBody::ValueType
*	{
*	// definitions...
*	public:
*		// Use this to access the body container and assign the value
*		typename MyBody::ValueType& Body()
*		{
*			return *this;
*		}
*	
*	};
* 
*/

export module murphy.http.http_bodies;

import <string>;

import murphy.ds.buffer;

namespace murphy::http
{
	export 
	struct EmptyBody
	{
		struct ValueType {};

		static auto Serialize(ds::DynamicBuffer& db, ValueType& body) -> void
		{

		}

		static auto Parse(ds::NonOwnedBuffer& rb, ValueType& body) -> void
		{

		}
	};

	export
	struct StringBody
	{
		using ValueType = std::string;

		static auto Serialize(ds::DynamicBuffer& db, std::string& body) -> void;
		static auto Parse(ds::NonOwnedBuffer& rb, std::string& body) -> void;
	};
} // namespace murphy::http