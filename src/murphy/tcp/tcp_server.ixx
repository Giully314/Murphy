/*
* murphy/tcp/tcp_server.ixx
* 
* PURPOSE:
* 
* CLASSES:
* 
* DESCRIPTION:
*	This class is used for local tests. Some of the operations are identical between
*	client and server, so in a different scenario I would make a base class with send/recv
*	functions.
*	Also in a real server application the connections would be handled with async.
* 
* USAGE:
* 
*/

module;

#include <winsock2.h>

export module murphy.tcp.tcp_server;

import <utility>;
import <string_view>;
import <expected>;

import murphy.types;
import murphy.winsock.winsock_errors;
import murphy.ds.buffer;

namespace murphy::tcp
{	
	export
	class Server final : private NonCopyable
	{
	public:

		Server();
		
		Server(Server&& other) noexcept : sock(std::exchange(other.sock, INVALID_SOCKET)),
			info(std::move(info)),
			connected_client(std::exchange(other.connected_client, INVALID_SOCKET))
		{

		}

		auto operator=(Server&& other) noexcept -> Server&
		{
			sock = std::exchange(other.sock, INVALID_SOCKET);
			info = std::move(info);
			connected_client = std::exchange(other.connected_client, INVALID_SOCKET);
			return *this;
		}
		
		~Server();


		// SocketError: BindError.
		// ip must be null terminated.
		[[nodiscard]]
		auto Bind(const std::wstring_view ip, const u16 port) -> std::expected<void, winsock::SocketError>;

		// SocketError: ListenError.
		// Set the max number of pending connections.
		[[nodiscard]]
		auto Listen(u32 n = 1) -> std::expected<void, winsock::SocketError>;


		// SocketERror: ConnectionError.
		// Accept a pending connection.
		[[nodiscard]]
		auto AcceptConnection() -> std::expected<void, winsock::SocketError>;


		// SocketError: SendError.
		// Precondition: client must be connected to a server.
		[[nodiscard]]
		auto Send(non_owned_res<byte> data, u32 size) -> std::expected<i32, winsock::SocketError>;


		// SocketError: RecvError.
		// Precondition: client must be connected to a server.
		[[nodiscard]]
		auto Recv(non_owned_res<byte> data, u32 size) -> std::expected<i32, winsock::SocketError>;



		auto SetTimeout(u32 time_ms) -> void
		{
			//DWORD recvTimeoutMs = 2000;
			//DWORD sendTimeoutMs = 3000;
			setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&time_ms, sizeof(time_ms));
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time_ms, sizeof(time_ms));
		}


		[[nodiscard]]
		auto Socket() const -> SOCKET
		{
			return sock;
		}

	private:
		SOCKET sock = INVALID_SOCKET;
		SOCKADDR_IN info;

		SOCKET connected_client = INVALID_SOCKET;
	};
} // namespace murphy::tcp