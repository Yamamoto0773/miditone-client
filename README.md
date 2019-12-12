# miditone-client
C++17 HTTP Client for [miditone-server](https://github.com/Yamamoto0773/miditone-server). 
This Repository is part of miditone#, Koreisai 2019 5J Project.

## Related Repositories
Koreisai2019 5J Project, miditone#, constructed by follwing repositories.
- Main Game   
  [miditone](https://github.com/shirry0129/miditone)
- Game Server  
  [miditone-server](https://github.com/Yamamoto0773/miditone-server)
- C++ HTTP Client for miditone-server  
  [miditone-client (here)](https://github.com/Yamamoto0773/miditone-client)
- Use Wii Balance Board as input device  
  [balanceboard-controller](https://github.com/Yamamoto0773/balanceboard-controller)

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
        std::cout << "  user.name: " << response.parsed_body().user.name << "\n";
        std::cout << "user.qrcode: " << response.parsed_body().user.qrcode << "\n";
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
  user.name: user_name
user.qrcode: 814152626436
```

## Supported API Routes
|URI|VERB|DESCRIPTION|Available|
|--|--|--|--|
|`/api/users`                                       |`GET`    |get users|✅|
|`/api/users         `                              |`POST`   |create a user||
|`/api/users/{qrcode}`                              |`GET`    |get a user specified by QR code|✅|
|`/api/users/{qrcode}`                              |`PUT`    |update a user specified by QR code||
|`/api/users/{qrcode}`                              |`DELETE` |destroy a user specified by QR code||
|`/api/users/{qrcode}/{platform}/preference`        |`GET`    |get user's preference|✅|
|`/api/users/{qrcode}/{platform}/preference`        |`PUT`    |update user's preference|✅|
|`/api/users/{qrcode}/{platform}/scores`            |`GET`    |get user's scores|✅
|`/api/users/{qrcode}/{platform}/scores/{id}`       |`GET`    |get a score specified by record id||
|`/api/users/{qrcode}/{platform}/scores/new_record` |`PUT`    |create or update user's score|✅|
|`/api/users/{qrcode}/{platform}/scores/{id}`       |`DELETE` |destroy a score specified by record id||
|`/api/musics`                                      |`GET`    |get musics|✅|
|`/api/musics`                                      |`POST`   |create a music|
|`/api/musics/{id}`                                 |`GET`    |get a music specified by music ID|✅|
|`/api/musics/{id}`                                 |`PUT`    |update a music specified by music ID||
|`/api/musics/{id}`                                 |`DELETE` |destroy a music specified by music ID||
|`/api/musics/{id}/{platform}/ranking`              |`GET`    |get all user's scores of specified music|✅|
|`/api/musics/{platform}/played_times`              |`GET`    |get played times of each musics|✅|
|`/api/musics/{id}/{platform}/played_times`         |`GET`    |get played times of specified music|✅|
|`/api/health_check`                                |`GET`    |for checking server|✅|

## License
This software is published under the **MIT License**.
Please read [License](/LICENSE) before use.
