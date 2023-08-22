/*
* murphy/http/http_errors.ixx
* 
* PURPOSE:
* 
* CLASSES:
* 
* DESCRIPTION:
*/

export module murphy.http.http_errors;

export 
namespace murphy::http
{
	enum class HTTPError
	{
		BadRequest,
		InvalidRequestLine,
		InvalidField,
		
		BadResponse,
		InvalidStatusLine,

	};



} // namespace murphy::http