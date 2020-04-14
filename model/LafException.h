#ifndef LAFEXCEPTION_H
#define LAFEXCEPTION_H
#include <exception>
#include <string>
class LafException : virtual public std::exception {

    const std::string message;

    public:
        LafException(const std::string& msg) :
            message(msg)
        {
        }

        virtual const char* what() const throw (){
            return message.c_str();
        }


};
#endif // LAFEXCEPTION_H
