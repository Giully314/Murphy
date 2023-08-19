
// must be included before windows.h
#include <winsock2.h>


import <iostream>;
import <thread>;
import <string>;

import murphy.tcp;
import murphy.winsock;
import murphy.test.test_client_server;

using namespace murphy;
using namespace std;

int main()
{

	winsock::WinsockManager::Startup();

	test::TestClientServer t;
	t.RunTest();

	winsock::WinsockManager::Shutdown();

	return 0;
}