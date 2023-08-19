/*
* murphy/tcp/tcp_client.cpp
*/

module;

#include <winsock2.h>
#include <Ws2tcpip.h>

module murphy.tcp.tcp_client;


namespace murphy::tcp
{
	Client::Client() : is_connected(false)
	{
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
		{
			throw winsock::InvalidSocket{ "TCP Client invalid socket." };
		}

		server_info.sin_family = AF_INET;
	}


	Client::~Client()
	{
		/*if (is_connected)
		{
			TryDisconnect();
		}*/
		shutdown(sock, SD_BOTH);
		closesocket(sock);
	}

	auto Client::Connect(const std::wstring_view ip, const u16 port)
		-> std::expected<void, winsock::SocketError>
	{
		server_info.sin_port = htons(port);

		if (InetPton(AF_INET, ip.data(), &server_info.sin_addr.S_un.S_addr) != 1)
		{
			return std::unexpected(winsock::SocketError::IPConversion);
		}

		if (connect(sock,
			reinterpret_cast<SOCKADDR*>(&server_info),
			sizeof(server_info)) != 0)
		{
			return std::unexpected(winsock::SocketError::Connection);
		}
		
		is_connected = true;
		
		return {};
	}

	auto Client::Disconnect() -> std::expected<void, winsock::SocketError>
	{
		if (!TryDisconnect())
		{
			return std::unexpected(winsock::SocketError::Disconnection);
		}
		return {};
	}

	auto Client::TryDisconnect() -> bool
	{
		if (shutdown(sock, SD_BOTH) != 0)
		{
			is_connected = false;
			sock = INVALID_SOCKET;
			return false;
		}
		return true;
	}


	auto Client::Send(non_owned_res<byte> data, u32 size) 
		-> std::expected<i32, winsock::SocketError>
	{
		i32 bytes_sent = send(sock, data, size, 0);
		
		// Possible disconnection
		if (bytes_sent == SOCKET_ERROR)
		{
			//TryDisconnect();
			return std::unexpected(winsock::SocketError::Send);
		}

		return bytes_sent;
	}


	auto Client::Recv(non_owned_res<byte> data, u32 size) 
		-> std::expected<i32, winsock::SocketError>
	{
		i32 num_of_bytes_recv = recv(sock, data, size, 0);
		if (num_of_bytes_recv == SOCKET_ERROR)
		{
			return std::unexpected(winsock::SocketError::Recv);
		}
		return num_of_bytes_recv;
	}
} // namespace murphy::tcp