#pragma once

#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>

#include "Result.hpp"

namespace http {
    class Request;
    class Response;
}

namespace api_client {

    using char_type = char;
    using string_type = std::basic_string<char_type>;
    using int_type = int32_t;

    struct ResponseBase {
    public:
        ResponseBase();
        ResponseBase(const http::Response* response);

        const std::string& raw_body() const noexcept;
    private:
        unsigned int status_code_;
    };

    struct connection_configure_t {
        string_type host, port;
        string_type method;
        string_type uri;
    };

    struct RequestBase {
    public:
        RequestBase();

        std::unique_ptr<ResponseBase> send() const noexcept;

    private:
        std::unique_ptr<http::Request> request_;
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
