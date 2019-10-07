#pragma once

#include <optional>
#include <exception>
#include <string>
#include <variant>


template <typename S, typename F>
class Result {
public:
    using success_type = S;
    using failed_type = F;
    using variant_type = std::variant<success_type, failed_type>;
 
    class BadAccess : public std::exception {
    public:
        explicit BadAccess(const char* method_name) {
            using namespace std::literals::string_literals;

            msg_ = u8"Bad Access in `"s + method_name + u8"' method of `Result' class.\n";
        }

        const char* what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
    };

    Result() : is_success_(false), result_(std::nullopt) {}

    explicit Result(const success_type& s) : is_success_(true), result_(variant_type(s)) {}

    explicit Result(const failed_type& f) : is_success_(false), result_(variant_type(f)) {}

    Result& success(const success_type& s) {
        is_success_ = true;
        result_.emplace(variant_type(s));
    }

    Result& failed(const failed_type& f) {
        is_success_ = false;
        result_.emplace(variant_type(f));
    }

    void reset() {
        is_success_ = false;
        result_.reset();
    }

    bool is_success() const noexcept {
        return is_success_;
    }

    bool is_failed() const noexcept {
        return !is_success_;
    }

    bool has_value() const noexcept {
        return result_.has_value();
    }

    const success_type& success_value() const & {
        if (is_success_)
            return std::get<success_type>(result_.value());
        else
            throw BadAccess(u8"success_value");
    }

    const success_type&& success_value() const && {
        if (is_success_)
            return std::move(std::get<success_type>(result_.value()));
        else
            throw BadAccess(u8"success_value");
    }

    success_type& success_value() & {
        if (is_success_)
            return std::get<success_type>(result_.value());
        else
            throw BadAccess(u8"success_value");
    }

    success_type&& success_value() && {
        if (is_success_)
            return std::move(std::get<success_type>(result_.value()));
        else
            throw BadAccess(u8"success_value");
    }

    const failed_type& failed_value() const & {
        if (!is_success_ && result_.has_value())
            return std::get<failed_type>(result_.value());
        else
            throw BadAccess(u8"failed_value");
    }

    const failed_type&& failed_value() const && {
        if (!is_success_ && result_.has_value())
            return std::move(std::get<failed_type>(result_.value()));
        else
            throw BadAccess(u8"failed_value");
    }

    failed_type& failed_value() & {
        if (!is_success_ && result_.has_value())
            return std::get<failed_type>(result_.value());
        else
            throw BadAccess(u8"failed_value");
    }

    failed_type&& failed_value() && {
        if (!is_success_ && result_.has_value())
            return std::move(std::get<failed_type>(result_.value()));
        else
            throw BadAccess(u8"failed_value");
    }

    success_type success_value_or(success_type&& s) const & noexcept {
        try {
            return success_value();
        } catch (const std::exception& _) {
            return s;
        }
    }

    success_type success_value_or(success_type&& s) && noexcept {
        try {
            return success_value();
        } catch (const std::exception& _) {
            return s;
        }
    }

    failed_type failed_value_or(failed_type&& f) const & noexcept {
        try {
            return failed_value();
        } catch (const std::exception& _) {
            return f;
        }
    }

    failed_type failed_value_or(failed_type&& f) && noexcept {
        try {
            return failed_value();
        } catch (const std::exception& _) {
            return f;
        }
    }

    explicit operator bool() const noexcept {
        return is_success_;
    }

    bool operator!() const noexcept {
        return !is_success_;
    }

private:
    bool is_success_;
    std::optional<variant_type> result_;
};
