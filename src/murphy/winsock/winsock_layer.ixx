/*
* murphy/winsock/winsock_layer.ixx
* 
* PURPOSE: manage winsock library.
*/

export module murphy.winsock.winsock_layer;

import murphy.winsock.winsock_errors;

export
namespace murphy::winsock
{
	struct WinsockManager
	{
		static auto Startup() -> void;
		static auto Shutdown() -> void;

		static auto RegisterError() -> void;
		static auto RegisterError(const SocketError e) -> void;
	};

} // namespace murphy