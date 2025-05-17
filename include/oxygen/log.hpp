#pragma once

#include <iostream>
#include <string>

#define OXY_ANSI_RESET      "\033[0m"
#define OXY_ANSI_DEBUG      "\033[1;90m"
#define OXY_ANSI_INFO       "\033[1;94m"
#define OXY_ANSI_WARNING    "\033[1;93m"
#define OXY_ANSI_ERROR      "\033[1;91m"
#define OXY_ANSI_FATAL      "\033[1;95m"

namespace oxygen
{
    class Message 
    {
    public:
        virtual inline std::string get_output() const 
        {
            return (enabled()) ? (get_text()) : ""; 
        }

        virtual bool enabled() const {return true; }
    protected:
        virtual std::string get_prefix() const {return ""; };

        virtual std::string get_suffix() const {return ""; };

        inline std::string get_text() const { return get_prefix() + m_text + get_suffix();}

    private:
        std::string m_text;


    public:
        Message& operator<<(const std::string& str) 
        {
            if(enabled())
            {
                m_text += str;
            }
            return *this;
        }

        operator std::string() const {
            return get_output();
        }
    };

    class Log : public Message
    {
    public:
        inline std::string get_output() const override
        {
            return (enabled()) ? (get_text() + OXY_ANSI_RESET "\n") : ""; 
        }
    };


    class LogDebug : public Log
    {
    protected:
        std::string get_prefix() const override 
        {return OXY_ANSI_DEBUG;}

        #ifndef OXY_DEBUG
        bool enabled() const override {return false; }
        #endif
    };

    class LogInfo : public Log
    {
    protected:
        std::string get_prefix() const override 
        {return OXY_ANSI_INFO "[    INFO     ]: " OXY_ANSI_RESET;}
    };

    class LogWarning : public Log
    {
    protected:
        std::string get_prefix() const override 
        {return OXY_ANSI_WARNING "[   WARNING   ]: " OXY_ANSI_RESET;}
    };

    class LogError : public Log
    {
    protected:
        std::string get_prefix() const override 
        {return OXY_ANSI_ERROR "[    ERROR    ]: " OXY_ANSI_RESET;}
    };

    class LogFatal : public Log
    {
    protected:
        std::string get_prefix() const override 
        {return OXY_ANSI_FATAL "[ FATAL ERROR ]: " OXY_ANSI_RESET;}
    };

}

std::ostream& operator<<(std::ostream& os, const oxygen::Message& msg);