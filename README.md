# miditone-client
[miditone-server](https://github.com/Yamamoto0773/miditone-server) client for C++

## Developed with
- C++17
- Boost/Asio
- Boost/Beast

## Overview
- Simple HTTP Client
- Based on synchronous communication


## Usage

⚠ This code is not using miditone-client.

```cpp
#include <iostream>
#include "HTTPClient.hpp"

int main() {
    const auto token = "Bearer 01234";
    const auto host = "localhost";
    const auto port = "3000";
    const std::string params = "{\"hoge\":{\"fuga\":\"bar\"}}";

    const auto response =
        http::Request(http::verb::post, "/path/to/resource", http::version::_11)
        .set(http::field::content_type, "application/json; charset=utf-8")
        .set(http::field::authorization, token)
        .set(http::field::content_length, std::to_string(params.length()))
        .set(http::field::connection, "close")
        .set_body(params)
        .send(host, port);

    if (response) {
        std::cout << response.success_value().status_code() << "\n";
        std::cout << response.success_value().body() << "\n";
    } else {
        std::cout << response.failed_value().body() << "\n";
    }

    return 0;
}
```


## Implemented Classes

- unnamed space
  - Result<S, F>
- namespace http 
  - Request
  - Response
- namespace client
  - MiditoneClient (unfinished)
  - Response (unfinished)
