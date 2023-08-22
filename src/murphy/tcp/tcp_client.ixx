/*
* murphy/tcp/tcp_client.ixx
* 
* PURPOSE: basic TCP client.
* 
* CLASSES:
*	Client: client class for tcp connections with IP4.
* 
* DESCRIPTION:
*	
* 
* USAGE:
*	
* 
*/

module;

#include <winsock2.h>

//#include <windows.h>

export module murphy.tcp.tcp_client;

import <string_view>;
import <span>;
import <utility>;
import <expected>;

import murphy.types;
import murphy.ds.buffer;
import murphy.winsock.winsock_errors;


namespace murphy::tcp
{
	export
	class Client final : private NonCopyable
	{
	public:

		// Can throw: InvalidSocket
		// Postcondition: the client socket is created and ready to be used.
		[[nodiscard]]
		Client();

		[[nodiscard]]
		Client(Client&& other) noexcept : sock(std::exchange(other.sock, INVALID_SOCKET)),
			server_info(std::move(server_info)),
			is_connected(std::exchange(other.is_connected, false))
		{

		}
		
		[[nodiscard]]
		auto operator=(Client&& other) noexcept -> Client&
		{
			sock = std::exchange(other.sock, INVALID_SOCKET);
			server_info = std::move(server_info);
			is_connected = std::exchange(other.is_connected, false);
			return *this;
		}

		~Client();


		// SocketError: IPConversionError | ConnectionError.
		// ip: must be null terminated.
		// Postcondition: client is connected to ip/port.
		[[nodiscard]]
		auto Connect(const std::wstring_view ip, const u16 port) 
			-> std::expected<void, winsock::SocketError>;
		

		// SocketError: DisconnectionError.
		// Try to disconnect from the server.
		[[nodiscard]]
		auto Disconnect() -> std::expected<void, winsock::SocketError>;


		// SocketError: SendError.
		// Precondition: client must be connected to a server.
		[[nodiscard]]
		auto Send(non_owned_res<byte> data, u32 size) 
			-> std::expected<i32, winsock::SocketError>;


		// SocketError: RecvError.
		// Precondition: client must be connected to a server.
		[[nodiscard]]
		auto Recv(non_owned_res<byte> data, u32 size) 
			-> std::expected<i32, winsock::SocketError>;


		// Return true if the socket is connected.
		[[nodiscard]]
		auto IsConnected() const noexcept -> bool
		{
			return is_connected;
		}

		// Return the socket handler.
		[[nodiscard]]
		auto Socket() const noexcept -> SOCKET
		{
			return sock;
		}

	private:
		// Try to disconnect from the server. If errors occur, return false.
		[[nodiscard]]
		auto TryDisconnect() -> bool;


	private:
		SOCKET sock = INVALID_SOCKET;
		SOCKADDR_IN server_info;
		bool is_connected = false;
	};
	
} // namespace murphy::tcp