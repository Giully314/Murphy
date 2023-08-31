
// must be included before windows.h
#include <winsock2.h>


import <iostream>;
import <thread>;
import <string>;
import <print>;

import murphy.tcp;
import murphy.winsock;
import murphy.test.http.test_http_transfer;

using namespace murphy;
using namespace std;

int main()
{

	winsock::WinsockManager::Startup();


	/*test::TestHTTPTransfer t;
	t.RunTest();*/


	winsock::WinsockManager::Shutdown();

	return 0;
}