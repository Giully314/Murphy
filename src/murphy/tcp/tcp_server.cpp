/*
* murphy/tcp/tcp_server.ixx
*/

module;

#include <winsock2.h>
#include <Ws2tcpip.h> // for InetPton

module murphy.tcp.tcp_server;


namespace murphy::tcp
{
	Server::Server()
	{
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		info.sin_family = AF_INET;
	}
	

	Server::~Server()
	{
		shutdown(connected_client, SD_BOTH);
		shutdown(sock, SD_BOTH);
		closesocket(connected_client);
		closesocket(sock);
	}


	auto Server::Bind(const std::wstring_view ip, const u16 port) -> std::expected<void, winsock::SocketError>
	{
		info.sin_port = htons(port);
		InetPton(AF_INET, ip.data(), &info.sin_addr.S_un.S_addr);

		if (bind(sock, reinterpret_cast<SOCKADDR*>(&info), sizeof(info)) == SOCKET_ERROR)
		{
			return std::unexpected(winsock::SocketError::Bind);
		}
		return {};
	}


	auto Server::Listen(u32 n) -> std::expected<void, winsock::SocketError>
	{
		if (listen(sock, n) == SOCKET_ERROR)
		{
			return std::unexpected(winsock::SocketError::Listen);
		}
		return {};
	}

	auto Server::AcceptConnection() -> std::expected<void, winsock::SocketError>
	{
		if (connected_client = accept(sock, nullptr, nullptr);
			connected_client == INVALID_SOCKET)
		{
			return std::unexpected(winsock::SocketError::Connection);
		}
		return {};
	}

	auto Server::Send(non_owned_res<byte> data, u32 size) -> std::expected<i32, winsock::SocketError>
	{
		i32 bytes_sent = send(connected_client, data, size, 0);
		// Possible disconnection
		if (bytes_sent == SOCKET_ERROR)
		{
			return std::unexpected(winsock::SocketError::Send);
		}
		return bytes_sent;
	}

	auto Server::Recv(non_owned_res<byte> data, u32 size) -> std::expected<i32, winsock::SocketError>
	{
		i32 num_of_bytes_recv = recv(connected_client, data, size, 0);
		if (num_of_bytes_recv == SOCKET_ERROR)
		{
			return std::unexpected(winsock::SocketError::Recv);
		}
		return num_of_bytes_recv;
	}

} // namespace murphy::tcp