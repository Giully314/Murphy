export module murphy.test.http.test_http_transfer;

import <string_view>;
import <future>;

import murphy.types;
import murphy.http;
import murphy.tcp;
import murphy.test.test_base;
import murphy.winsock;
import murphy.ds.buffer;


export 
namespace murphy::test
{
	class TestHTTPTransfer final : public TestBase
	{
	public:
		auto RunTest() -> void
		{
			using namespace winsock;
			std::expected<void, SocketError> void_ret;
			std::expected<i32, SocketError> i32_ret;

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

			server.SetTimeout(2000);

			mconsole_log.Debug("Start client");
			auto f = std::async(std::launch::async, &TestHTTPTransfer::ClientTask, this);


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

			//std::string_view msg{ raw_buffer, static_cast<size_t>(*i32_ret)};
			http::Request<> request;
			request.Parse(ds::NonOwnedBuffer{ raw_buffer, static_cast<u64>(*i32_ret) });
			/*if (!request)
			{
				mconsole_log.Debug("Error with parse http message.");
				return;
			}*/
			mconsole_log.Debug("{}", request.ToString());
		}


	private:
		auto ClientTask() -> void
		{
			using namespace winsock;
			std::expected<void, SocketError> void_ret;
			std::expected<i32, SocketError> i32_ret;
			tcp::Client client;

			http::Request<> request;
			request.SetMethod(method);
			request.SetURL(url);
			request.SetVersion(version);
			request.Set("Content-length", "415");

			if (void_ret = client.Connect(ip, port); !void_ret)
			{
				WinsockManager::RegisterError(void_ret.error());
				mconsole_log.Debug("Error with client connect.");
				return;
			}

			auto data = request.Serialize();
			if (i32_ret = client.Send(data.Data(), data.Size()); !i32_ret)
			{
				WinsockManager::RegisterError(i32_ret.error());
				mconsole_log.Debug("Error with client send.");
				return;
			}
			mconsole_log.Debug("Client sent {} bytes.", i32_ret.value());
		}


		auto ToBytes(std::string_view s) -> ds::DynamicBuffer
		{
			constexpr u32 element_size = sizeof(char);
			ds::DynamicBuffer buffer;
			buffer.Append(reinterpret_cast<const byte*>(s.data()), s.size() * element_size);
			return buffer;
		}

	private:
		tcp::Client client;
		tcp::Server server;

		std::wstring_view ip = L"127.0.0.1";
		u16 port = 80;

		std::string method{ "GET" };
		std::string url{ "www.test.com" };
		std::string version{ "HTTP/1.1" };
		std::string message{ "GET www.test.com HTTP/1.1\r\nContent-length: 415\r\n\r\n"};
	};


} // namespace murphy::test