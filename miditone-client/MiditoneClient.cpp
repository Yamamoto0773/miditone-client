#include <sstream>

#include "MiditoneClient.hpp"


namespace api_client {
    namespace response {
        namespace parser {
            health_check_t health_check_parser(const ptree_type& ptree) {
                health_check_t parsed;
                parsed.title     = ptree.get<string_type>("data.attributes.title", "");

                return parsed;
            }

            user_t user_parser(const ptree_type& ptree) {
                user_t parsed;
                parsed.name     = ptree.get<string_type>("data.attributes.name", "");
                parsed.qrcode   = ptree.get<string_type>("data.attributes.qrcode", "");

                return parsed;
            }

            std::vector<user_t> users_parser(const ptree_type& ptree) {
                ptree_type empty_tree;
                std::vector<user_t> parsed;

                for (const auto& child_tree : ptree.get_child("data", empty_tree)) {
                    const auto& user_attributes = child_tree.second.get_child("attributes");
                    parsed.push_back(user_parser(user_attributes));
                }

                return parsed;
            }
        }
    }

    namespace request {

        http::Request create_base_request(unsigned int http_version, const string_type& token) {
            const auto base_req =
                http::Request()
                .set(http_version)
                .set(http::field::content_type, "application/json; charset=utf-8")
                .set(http::field::authorization, token)
                .set(http::field::connection, "close");

            return base_req;
        }

        HealthCheck::HealthCheck(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::HealthCheck> HealthCheck::send() const noexcept {
            const auto result =
                create_base_request(client_.http_version, client_.token())
                .set(method_, "/api/health_check")
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::HealthCheck>(response::HealthCheck(result.success_value(), response::parser::health_check_parser));
            else
                return result_type<response::HealthCheck>(result.failed_value());
        }


        User::User(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

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
                create_base_request(client_.http_version, client_.token())
                .set(http::verb::get, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::User>(response::User(result.success_value(), response::parser::user_parser));
            else
                return result_type<response::User>(result.failed_value());
        }

        Users::Users(const MiditoneClient& client, http::verb method)
            : RequestBase(client, method) {}

        result_type<response::Users> Users::send() const noexcept {
            string_type uri = "/api/users";

            auto result =
                create_base_request(client_.http_version, client_.token())
                .set(http::verb::get, uri)
                .send(client_.destination().host, client_.destination().port);

            if (result)
                return result_type<response::Users>(response::Users(result.success_value(), response::parser::users_parser));
            else
                return result_type<response::Users>(result.failed_value());
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

    request::HealthCheck MiditoneClient::health_check_request() const noexcept {
        return request::HealthCheck(*this, http::verb::get);
    }

    request::User MiditoneClient::get_user_request(const string_type& qrcode) const noexcept {
        return request::User(*this, http::verb::get).set_qrcode(qrcode);
    }

    request::Users MiditoneClient::get_users_request() const noexcept {
        return request::Users(*this, http::verb::get);
    }

}

