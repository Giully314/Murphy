/*
* murphy/http/http_concepts.ixx
* 
* PURPOSE: define concepts for http.
* 
* CONCEPTS:
*	header_container: specify the requirements for the http header container.
*	body_container: specify the requirements for the http body container.
*/

export module murphy.http.http_concepts;

import <concepts>;
import <string_view>;

import murphy.types;
import murphy.ds.buffer;

export
namespace murphy::http
{
	template <typename T>
	concept header_container = requires (T h, ds::DynamicBuffer db, ds::NonOwnedBuffer rb,
		std::string_view f, std::string_view v)
	{
		{ h[f] } -> std::convertible_to<std::string_view>;
		{ h.Set(f, v) };
		{ h.Has(f) } -> std::same_as<bool>;

		{ h.Serialize(db) };
		{ h.Parse(rb) };
		{ h.Size() } -> std::same_as<u64>;
	};


	template <typename T>
	concept body_container = requires (T a, typename T::ValueType x,
		ds::DynamicBuffer db, ds::NonOwnedBuffer rb)
	{
		typename T::ValueType;
		
		{ T::Serialize(db, x) };
		{ T::Parse(rb, x) };
	};
} // namespace murphy::http