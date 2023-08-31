/*
* murphy/test/test_html_scanner.ixx
* 
*/

export module murphy.test.test_html_scanner;

import <string>;
import <string_view>;

import murphy.test.test_base;
import murphy.html;


namespace murphy::test
{
	export 
	class TestHTMLScanner : public TestBase
	{
	public:
		auto RunTest() -> void
		{

		}

	private:
		/*<!doctype html>
		<html lang = "en-US">
		<head>
		<meta charset = "utf-8" / >
		<title>My test page< / title>
		< / head>
		<body>
		<p>This is my page< / p>
		< / body>
		< / html>*/

		std::string_view doc = 
			"<!DOCTYPE>"
			"<html>"
			"</html>";
	};

} // namespace murphy::test