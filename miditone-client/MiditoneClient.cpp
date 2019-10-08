#include "MiditoneClient.hpp"

namespace api_client {

    namespace response {
        ResponseBase::ResponseBase(http::Response& response) : http::Response(response) {}

        HealthCheck::HealthCheck(http::Response& response) : ResponseBase(response) {}
        void HealthCheck::parse_body() {}

        User::User(http::Response& response) : ResponseBase(response) {}
        void User::parse_body() {}
    }

    namespace request {
        RequestBase::RequestBase(const MiditoneClient& client)
            : client_(client) {}

        http::Request RequestBase::create_base_request() const noexcept {
            const auto base_req =
                http::Request()
                .set(client_.http_version)
                .set(http::field::content_type, "application/json; charset=utf-8")
                .set(http::field::authorization, client_.token())
                .set(http::field::connection, "close");

            return base_req;
        }


        HealthCheck::HealthCheck(const MiditoneClient& client)
            : RequestBase(client) {}

        result_type<response::HealthCheck> HealthCheck::send() const noexcept {
            auto result =
                create_base_request()
                .set(http::verb::get, "/api/health_check")
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::HealthCheck>(result.success_value());
            else
                return result_type<response::HealthCheck>(result.failed_value());
        }


        User::User(const MiditoneClient& client)
            : RequestBase(client) {}

        User& User::set_qrcode(const string_type& qrcode) noexcept {
            qrcode_ = qrcode;

            return *this;
        }

        result_type<response::User> User::send() const noexcept {
            string_type uri = "/api/users";
            if (!qrcode_.empty()) {
                uri += "/" + qrcode_;
            }

            auto result =
                create_base_request()
                .set(http::verb::get, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::User>(result.success_value());
            else
                return result_type<response::User>(result.failed_value());
        }
    }


    MiditoneClient::MiditoneClient() {}

    MiditoneClient::MiditoneClient(const string_type& host, const string_type& port, const string_type& token) :
        host_(host), port_(port), token_(token) {}

    MiditoneClient& MiditoneClient::destination(const string_type& host, const string_type& port) {
        host_ = host;
        port_ = port;

        return *this;
    }

    connection_dest_t MiditoneClient::destination() const noexcept {
        return { host_, port_ };
    }

    MiditoneClient& MiditoneClient::token(const string_type& token) noexcept {
        token_ = token;

        return *this;
    }

    const string_type& MiditoneClient::token() const noexcept {
        return token_;
    }

    request::HealthCheck MiditoneClient::health_check() const noexcept {
        return request::HealthCheck(*this);
    }

    request::User MiditoneClient::users_request() const noexcept {
        return request::User(*this);
    }

}

