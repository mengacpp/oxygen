#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#define XGN_ANSI_RESET "\033[0m"
#define XGN_ANSI_DEBUG "\033[1;90m"
#define XGN_ANSI_INFO "\033[1;94m"
#define XGN_ANSI_WARNING "\033[1;93m"
#define XGN_ANSI_ERROR "\033[1;91m"
#define XGN_ANSI_FATAL "\033[1;95m"

namespace xgn
{
namespace log
{

class Message
{
public:
    virtual inline std::string get_output() const
    {
        return (enabled()) ? (get_text()) : "";
    }

    virtual bool enabled() const { return true; }

    template <typename T>
    std::enable_if_t<std::is_same<decltype(std::declval<std::ostream &>()
                                           << std::declval<T>()),
                                  std::ostream &>::value,
                     Message &>
    operator<<(T const &v)
    {
        std::ostringstream oss;
        oss << v;
        m_text += oss.str();
        return *this;
    }

protected:
    virtual std::string get_prefix() const { return ""; };

    virtual std::string get_suffix() const { return ""; };

    inline std::string get_text() const
    {
        return get_prefix() + m_text + get_suffix();
    }

private:
    std::string m_text;

public:
    Message &operator<<(const std::string &str)
    {
        if (enabled())
        {
            m_text += str;
        }
        return *this;
    }

    operator std::string() const { return get_output(); }
};

class Log : public Message
{
public:
    inline std::string get_output() const override
    {
        return (enabled()) ? (get_text() + XGN_ANSI_RESET "\n") : "";
    }
};

class Debug : public Log
{
protected:
    std::string get_prefix() const override { return XGN_ANSI_DEBUG; }

#ifndef XGN_DEBUG
    bool enabled() const override { return false; }
#endif
};

class Info : public Log
{
protected:
    std::string get_prefix() const override
    {
        return XGN_ANSI_INFO "[    INFO     ]: " XGN_ANSI_RESET;
    }
};

class Warning : public Log
{
protected:
    std::string get_prefix() const override
    {
        return XGN_ANSI_WARNING "[   WARNING   ]: " XGN_ANSI_RESET;
    }
};

class Error : public Log
{
protected:
    std::string get_prefix() const override
    {
        return XGN_ANSI_ERROR "[    ERROR    ]: " XGN_ANSI_RESET;
    }
};

class Fatal : public Log
{
protected:
    std::string get_prefix() const override
    {
        return XGN_ANSI_FATAL "[ FATAL ERROR ]: " XGN_ANSI_RESET;
    }
};
} // namespace log
} // namespace xgn

std::ostream &operator<<(std::ostream &os, const xgn::log::Message &msg);