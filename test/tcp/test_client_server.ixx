/*
* test/tcp/test_client_server.ixx
* 
* PURPOSE: basic test for tcp client server.
*/
module;

#include <WinSock2.h>

export module murphy.test.test_client_server;

import <future>;
import <string_view>;
import <iostream>;
import <expected>;

import murphy.test.test_base;

import murphy.tcp;
import murphy.winsock;
import murphy.ds.buffer;


namespace murphy::test
{
	export
	class TestClientServer final : public TestBase
	{
	public:
		TestClientServer()
		{
			buffer = ToBytes(L"Hello world!");
		}

		auto RunTest() -> void
		{
			using namespace winsock;

			mconsole_log.Debug("Start run test");
			try
			{
				tcp::Server server;

				std::expected<void, winsock::SocketError> void_ret;
				std::expected<i32, winsock::SocketError> i32_ret;

				mconsole_log.Debug("Bind server to ip");
				
				if (void_ret = server.Bind(ip, port); !void_ret)
				{
					WinsockManager::RegisterError(void_ret.error());
					mconsole_log.Debug("Error with bind");
					return;
				}


				if (void_ret = server.Listen(); !void_ret)
				{
					WinsockManager::RegisterError(void_ret.error());
					mconsole_log.Debug("Error with listen.");
					return;
				}

				
				mconsole_log.Debug("Set server timeout");
				DWORD recvTimeoutMs = 2000;
				setsockopt(server.Socket(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&recvTimeoutMs, sizeof(recvTimeoutMs));
				DWORD sendTimeoutMs = 3000;
				setsockopt(server.Socket(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&sendTimeoutMs, sizeof(sendTimeoutMs));
				

				mconsole_log.Debug("Start client");
				auto f = std::async(std::launch::async, &TestClientServer::ClientTask, this);
				
				if (void_ret = server.AcceptConnection(); !void_ret)
				{
					WinsockManager::RegisterError(void_ret.error());
					mconsole_log.Debug("Error with accept connection.");
					return;
				}
				mconsole_log.Debug("Connection accepted");

				constexpr u32 raw_buffer_size = 1024;
				byte raw_buffer[raw_buffer_size];
				if (i32_ret = server.Recv(raw_buffer, raw_buffer_size); !i32_ret)
				{
					WinsockManager::RegisterError(i32_ret.error());
					mconsole_log.Debug("Error with server recv.");
					return;
				}
				mconsole_log.Debug("Server received {} bytes.", *i32_ret);
				
				ds::NonOwnedBuffer recv_buffer{ raw_buffer, static_cast<u32>(*i32_ret) };
				if (recv_buffer != buffer)
				{
					mconsole_log.Debug("Messages are not equal");
				}
				else
				{
					mconsole_log.Debug("Messages are equal!");
				}
			}
			catch (winsock::TCPException& e)
			{
				mconsole_log.Debug("{}", e.what());
			}
		}

	private:
		auto ClientTask() -> void
		{
			using namespace winsock;
			try
			{
				std::expected<void, winsock::SocketError> void_ret;
				std::expected<i32, winsock::SocketError> i32_ret;
				tcp::Client client;

				if (void_ret = client.Connect(ip, port); !void_ret)
				{
					WinsockManager::RegisterError(void_ret.error());
					mconsole_log.Debug("Error with client connect.");
					return;
				}
				
				if (i32_ret = client.Send(buffer.Data(), buffer.Size()); !i32_ret)
				{
					WinsockManager::RegisterError(i32_ret.error());
					mconsole_log.Debug("Error with client send.");
					return;
				}
				mconsole_log.Debug("Client sent {} bytes.", i32_ret.value());
			}
			catch (winsock::TCPException& e)
			{
				mconsole_log.Debug("{}", e.what());
			}
		}

		auto ToBytes(std::wstring_view s) -> ds::DynamicBuffer
		{
			constexpr u32 element_size = sizeof(wchar_t);
			ds::DynamicBuffer buffer;
			buffer.Append(reinterpret_cast<const byte const*>(s.data()), s.size() * element_size);
			return buffer;
		}


	private:
		ds::DynamicBuffer buffer;
		std::wstring_view ip = L"127.0.0.1";
		u16 port = 5150;
	};
} // namespace murphy::test