#pragma once

#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>

#include "HTTPClient.hpp"


namespace api_client {

    using char_type = char;
    using string_type = std::basic_string<char_type>;
    using int_type = int32_t;

    class MiditoneClient;

    namespace response {

        struct ResponseBase : public http::Response {
        public:
            ResponseBase(http::Response& response);

        protected:
            virtual void parse_body() = 0;
        };

        struct HealthCheck : public ResponseBase {
        public:
            HealthCheck(http::Response& response);
        protected:
            void parse_body() override;
        };

        struct User : public ResponseBase {
        public:
            User(http::Response& response);
        protected:
            void parse_body() override;
        };

    }


    namespace request {
        template<typename S>
        using result_type = Result<S, http::ConnectionError>;

        struct RequestBase {
        public:
            RequestBase(const MiditoneClient& client);

        protected:
            const MiditoneClient& client_;

            http::Request create_base_request() const noexcept;
        };

        struct HealthCheck : public RequestBase {
            HealthCheck(const MiditoneClient& client);
            result_type<response::HealthCheck> send() const noexcept;
        };

        struct User : public RequestBase {
            User(const MiditoneClient& client);

            /// <summary>
            /// 取得するユーザのQRコードを指定する
            /// </summary>
            /// <param name="qrcode">QRコードの文字列</param>
            /// <returns>
            /// *this
            /// </returns>
            User& set_qrcode(const string_type& qrcode) noexcept;
            result_type<response::User> send() const noexcept;
        private:
            string_type qrcode_;
        };
    }


    struct connection_dest_t {
        string_type host, port;
    };

    class MiditoneClient {
    public:
        static constexpr unsigned int http_version = http::version::_11;

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
        /// </summary>
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

        /// <summary>
        /// APIのトークンを取得する
        /// </summary>
        /// <returns>
        /// APIのトークン
        /// </returns>
        const string_type& token() const noexcept;

        /// <summary>
        /// ユーザに対するリクエストを取得する
        /// </summary>
        request::User users_request() const noexcept;

        request::HealthCheck health_check() const noexcept;

    private:
        string_type host_;
        string_type port_;
        string_type token_;
    };
}
