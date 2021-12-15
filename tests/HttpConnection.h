#pragma once
#include <iostream>
#include <string>

#include <boost/beast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

constexpr auto HOST = "localhost";
constexpr auto PORT = "8081";

class HttpConnection
{
public:
	static const int http_version = 11;

private:
public:
	static std::string create_body()
	{
		boost::property_tree::ptree tree;
		/// pa1 / 1234 / pearl

		tree.put("id", "peaky");
		tree.put("passwd", "1234");
		tree.put("name", "pearl");

		std::basic_stringstream<char> jsonStream;
		boost::property_tree::json_parser::write_json(jsonStream, tree, false);
		return jsonStream.str();
	}

	/*
	
	RESTful API HTTP Request
	GET
	POST
	PUT

	*/
	static bool GetRequest(const char* uri, http::response<http::dynamic_body>& result)
	{
		try
		{
			asio::io_context context;

			tcp::resolver resolver{ context };
			tcp::socket socket{ context };

			const auto results = resolver.resolve(HOST, PORT);

			// 동기
			asio::connect(socket, results.begin(), results.end());

			// 비동기
			//asio::async_connect(socket, results.begin(), results.end(),
			//	[&]()->void { printf("connected "); });

			http::request<http::string_body> req{ http::verb::get, uri, http_version };
			req.set(http::field::host, HOST);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			// 동기
			//http::write(socket, req);

			// 비동기
			http::async_write(socket, req,
				[&](std::error_code& error, size_t bytes)->void
				{
					if (error)
					{
						std::cerr << error << "\n";
						return;
					}
					std::cout << "write " << bytes << "\n";
				});

			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;

			http::read(socket, buffer, res);

			//http::async_read(socket, buffer, res,
			//	[&](std::error_code& error, size_t bytes)->void
			//	{
			//		if (error)
			//		{
			//			printf("failed read\n");
			//			return;
			//		}
			//		printf("read .. %d", bytes);
			//	});
			result = res;
			std::cout << res << "\n";

			beast::error_code ec;
			socket.shutdown(tcp::socket::shutdown_both, ec);

			if (ec && ec != beast::errc::not_connected)
			{
				throw beast::system_error{ ec };
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << "\n";
			return false;
		}

		return true;
	}

	// 체인 형태로 감싸면 비동기로 다 할 수도 있음
	static bool PostRequest(
		const char* uri,
		http::response<http::dynamic_body>& result)
	{
		try
		{
			asio::io_context context;

			tcp::resolver resolver{ context };
			tcp::socket socket{ context };

			const auto results = resolver.resolve(HOST, PORT);

			// 동기
			asio::connect(socket, results.begin(), results.end());

			// 비동기
			//asio::async_connect(socket, results.begin(), results.end(),
			//	[&]()->void { printf("connected "); });

			http::request<http::string_body> req{ http::verb::post, uri, http_version };
			req.set(http::field::host, HOST);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		
			
			// 동기
			//http::write(socket, req);

			// 비동기
			http::async_write(socket, req,
				[&](std::error_code& error, size_t bytes)->void
				{
					if (error)
					{
						std::cerr << error << "\n";
						return;
					}
					std::cout << "write " << bytes << "\n";
				});

			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;

			http::read(socket, buffer, res);

			//http::async_read(socket, buffer, res,
			//	[&](std::error_code& error, size_t bytes)->void
			//	{
			//		if (error)
			//		{
			//			printf("failed read\n");
			//			return;
			//		}
			//		printf("read .. %d", bytes);
			//	});
			result = res;
			std::cout << res << "\n";

			beast::error_code ec;
			socket.shutdown(tcp::socket::shutdown_both, ec);

			if (ec && ec != beast::errc::not_connected)
			{
				throw beast::system_error{ ec };
			}

		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << "\n";
			return false;
		}
		return true;
	}
};

