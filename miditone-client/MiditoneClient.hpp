#pragma once

#include <string>
#include <memory>

namespace api_client {

    using char_type = char;
    using string_type = std::basic_string<char_type>;
    using int_type = int32_t;

    namespace http {
        class HTTPClient;
    }


    struct RequestBase {
    public:
        RequestBase(std::unique_ptr<http::HTTPClient> client);


    private:
        std::unique_ptr<http::HTTPClient> client_;
    };


    struct connection_dest_t {
        string_type host, port;
    };

    class MiditoneClient {
    public:
        /// <summary>
        /// コンストラクタ
        /// </summary>
        MiditoneClient();

        /// <summary>
        /// コンストラクタ
        /// 接続先情報を元に構築
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <param name="token">APIのトークン</param>
        MiditoneClient(const string_type& host, const string_type& port, const string_type& token);

        /// <summary>
        /// 接続先情報を登録する
        /// </summary>
        /// <param name="host">接続先のホスト名，またはアドレス</param>
        /// <param name="port">接続先のポート番号</param>
        /// <returns>
        /// *this
        /// </returns>
        MiditoneClient& destination(const string_type& host, const string_type& port);

        /// <summary>
        /// 接続先情報を取得する
        /// <returns>
        /// 接続先の情報
        /// </returns>
        connection_dest_t destination() const noexcept;

        /// <summary>
        /// APIのトークンを登録する
        /// </summary>
        /// <param name="token">APIのトークン</param>
        /// <returns>
        /// *this
        /// </returns>
        MiditoneClient& token(const string_type& token) noexcept;


        

    private:
        string_type host_;
        string_type port_;
        string_type token_;
    };
}
