#include "MiditoneClient.hpp"

namespace api_client {

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
    }


    namespace response {
        HealthCheck::HealthCheck(http::Response& response) : ResponseBase(response) {}

        void HealthCheck::parse_body() {}

        ResponseBase::ResponseBase(http::Response& response) : http::Response(response) {
        }
    }


}

