#pragma once

#include <string>
#include <stdexcept>

namespace titanium
{
    class RecoverableException : public std::runtime_error
    {
    public:
        RecoverableException(std::string msg) 
        : std::runtime_error(msg) {}
    private:
    };

    class FatalException : public std::runtime_error
    {
    public:
        FatalException(std::string msg) 
        : std::runtime_error(msg) {}
    private:
    };
}