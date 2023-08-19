/*
* murphy/winsock/winsock_layer.cpp
*/


module;

#include <winsock2.h>
#include <winbase.h>

#include <murphy\log\logger_macro.hpp>

module murphy.winsock.winsock_layer;

import <string>;
import <string_view>;

import murphy.types;
import murphy.log;

namespace murphy::winsock
{
	auto WinsockManager::Startup() -> void
	{
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		{
			// error 
			MRP_CORE_DEBUG("Failed to start winsock.");
		}
	}

	auto WinsockManager::Shutdown() -> void
	{
		if (WSACleanup() == SOCKET_ERROR)
		{
			// error
			MRP_CORE_DEBUG("Failed to cleanup winsock.");
		}
	}

	// https://stackoverflow.com/questions/3400922/how-do-i-retrieve-an-error-string-from-wsagetlasterror
	auto WinsockManager::RegisterError() -> void
	{
		// Log the current error using WSAGETLASTERROR.
		i32 err;
		wchar_t msgbuf[256];   // for a message up to 255 bytes.
		msgbuf[0] = '\0';    // Microsoft doesn't guarantee this on man page.

		err = WSAGetLastError();

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
			nullptr,                // lpsource
			err,                 // message id
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
			msgbuf,              // output buffer
			sizeof(msgbuf),     // size of msgbuf, bytes
			nullptr);               // va_list of arguments


		// Temporary till logger support for wchar.
		// I'm aware of possible data loss, this is not intentended to be used as a good 
		// code.
		std::wstring w{ msgbuf };
		std::string s{ w.begin(), w.end() };

		MRP_CORE_DEBUG("Winsock error: {}", s);
	}


	auto WinsockManager::RegisterError(const SocketError e) -> void
	{
		// Log the current error using WSAGETLASTERROR.
		i32 err;
		wchar_t msgbuf[256];   // for a message up to 255 bytes.
		msgbuf[0] = '\0';    // Microsoft doesn't guarantee this on man page.

		err = WSAGetLastError();

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
			nullptr,                // lpsource
			err,                 // message id
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
			msgbuf,              // output buffer
			sizeof(msgbuf),     // size of msgbuf, bytes
			nullptr);               // va_list of arguments


		// Temporary till logger support for wchar.
		// I'm aware of possible data loss, this is not intentended to be used as a good 
		// code.
		std::wstring w{ msgbuf };
		std::string s{ w.begin(), w.end() };

		MRP_CORE_DEBUG("App error: {}\nWinsock error: {}", SocketErrorToString(e), s);	
	}
} // namespace murphy::winsock