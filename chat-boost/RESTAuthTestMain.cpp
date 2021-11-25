#include "HttpConnection.h"

int main()
{
	http::response<http::dynamic_body> response;
	HttpConnection::GetRequest("/lobby/search/all", response);
	HttpConnection::PostRequest("/auth/signup/peaky1/1234/peaky1", response);


	return 0;
}