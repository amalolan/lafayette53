#ifndef BACKENDEXCEPTION_H
#define BACKENDEXCEPTION_H
#include <iostream>
#include <exception>

class BackendException : public std::exception
{
    std::string _msg;
public:
    BackendException(const std::string msg) : _msg(msg){}

    virtual const char * what () const noexcept override
    {
        return ("Backend Exception" + _msg).c_str();
    }
};

#endif // BACKENDEXCEPTION_H
