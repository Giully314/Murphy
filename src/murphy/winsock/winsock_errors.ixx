/*
* murphy/tcp/tcp_exceptions.ixx
* 
* PURPOSE: custom errors for tcp classes.
* 
* CLASSES:
*	InvalidSocket: Failed to create a socket during construction.
*	SocketError: enum for socket operation errors.
* 
* DESCRIPTION:
*	Layer of abstraction to manage the winsock errors.
*/

export module murphy.winsock.winsock_errors;

import <exception>;
import <string>;
import <string_view>;
import <format>;

import murphy.types;

export 
namespace murphy::winsock
{
	class TCPException : public std::exception
	{
	public:
		TCPException() = default;

		TCPException(const std::string_view msg_) : msg(msg_)
		{

		}

		auto what() const noexcept -> const char* override
		{
			return msg.c_str();
		}

	protected:
		std::string msg;
	};


	class InvalidSocket : public TCPException
	{
	public:
		InvalidSocket() : TCPException("Invalid socket.")
		{

		}

		InvalidSocket(const std::string_view msg) : TCPException(msg)
		{

		}
	};


	//class ConnectionError : public TCPException
	//{
	//public:
	//	ConnectionError() : TCPException("Connection error.")
	//	{

	//	}

	//	ConnectionError(const std::string_view msg) : TCPException(msg)
	//	{

	//	}


	//	ConnectionError(const std::wstring_view ip, const u16 port)
	//	{
	//		// Temporary solution
	//		std::string s_ip{ ip.begin(), ip.end() };
	//		this->msg = std::format("Connection error with {} {}.", s_ip, port);
	//	}
	//};


	//class DisconnectionError : public TCPException
	//{
	//public:
	//	DisconnectionError() : TCPException("Disconnection error.")
	//	{

	//	}
	//};


	//class SendError : public TCPException
	//{
	//public:
	//	SendError() : TCPException("Send error.")
	//	{

	//	}
	//};

	//class RecvError : public TCPException
	//{
	//public:
	//	RecvError() : TCPException("Recv error.")
	//	{

	//	}
	//};

	//class BindError : public TCPException
	//{
	//public:
	//	BindError() : TCPException("Bind error.")
	//	{

	//	}
	//};


	enum class SocketError : u8
	{
		Bind,
		Listen,
		IPConversion,
		Connection,
		Disconnection,
		Send,
		Recv,
	};


	inline constexpr auto SocketErrorToString(const SocketError e) -> const std::string_view
	{
		switch (e)
		{
		case SocketError::Bind:
			return "Bind";
		
		case SocketError::Listen:
			return "Listen";
		
		case SocketError::IPConversion:
			return "IpConversion";
		
		case SocketError::Connection:
			return "Connection";

		case SocketError::Disconnection:
			return "Disconnection";

		case SocketError::Send:
			return "Send";

		case SocketError::Recv:
			return "Recv";
		}

		return "Unknown";
	}

} // namespace murphy::tcp