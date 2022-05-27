#include "jsonError.h"

//конструктор по умолчанию!

jsonError::jsonError()
{
    message = "error.";
}

jsonError::jsonError(std::string str){
    message = str;
}

std::ostream& operator <<(std::ostream &os, jsonError &exc){
    os << "exception: " << exc.message << std::endl;
    return os;
}

std::string jsonError::toString(){
    return message;
}
