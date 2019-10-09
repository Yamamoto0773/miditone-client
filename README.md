# miditone-client
[miditone-server](https://github.com/Yamamoto0773/miditone-server) client for C++

## Developed with
- C++17
- Boost/Asio
- Boost/Beast
- Boost/Property Tree

## Overview
- Simple HTTP Client
- Based on synchronous communication
- Can resolve name
- Can parse HTTP body

## Usage


```cpp
#include <iostream>
#include "MiditoneClient.hpp"

int main() {
    const auto token = "Bearer 01234abcd";
    const auto host = "localhost";
    const auto port = "3000";

    // create api client with configuration
    api_client::MiditoneClient client(host, port, token);

    // e.g.  GET /api/users/814152626436
    const auto& result = client.get_user("814152626436");

    // check request result
    if (result) {
        // get HTTP response
        const auto& response = result.success_value();
        std::cout << "     status: " << response.status() << "\n";
        std::cout << "status code: " << response.status_code() << "\n";
        std::cout << "       body: " << response.body() << "\n";
        std::cout << "  user.name: " << response.parsed_body().name << "\n";
        std::cout << "user.qrcode: " << response.parsed_body().qrcode << "\n";
    } else {
        // get error message
        const auto& error = result.failed_value();
        std::cout << "error      : " << error.body() << "\n";
    }
    
    return 0;
}

```

execution result.
```
     status: OK
status code: 200
       body: {"data":{"id":"5","type":"user","attributes":{"qrcode":"814152626436","name":"user_name"}}}
  user.name: user_name
user.qrcode: 814152626436
```


## Implemented

- unnamed space
  - `Result<S, F>`
- namespace `http` 
  - `Request`
  - `Response`
- namespace `api_client`
  - `MiditoneClient`
  - namespace `response`
    - `ResponseBase<T>`
    - namespace `parser`
      - methods for parsing HTTP body
  - namespace `request`
    - `RequestBase<T>`
    - `HealthCheck`
    - `User(s)`
