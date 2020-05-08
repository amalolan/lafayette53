#ifndef MODELEXCEPTION_H
#define MODELEXCEPTION_H
#include <iostream>
#include <exception>

/**
 * @brief The ModelException class is a custom Exception object for the ModelClass class
 */
class ModelException : public std::exception
{
    std::string _msg;
public:
    ModelException(const std::string msg) : _msg(msg){}

    virtual const char * what () const noexcept override
    {
        return _msg.c_str();
    }
};

#endif // MODELEXCEPTION_H
