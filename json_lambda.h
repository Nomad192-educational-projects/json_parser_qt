#if -DMETHOD == -DJSON_LAMBDA

#ifndef JSON_LAMBDA_H
#define JSON_LAMBDA_H

#include <sstream>
#include <string>
#include <memory>
#include <vector>

class Object_field;
class Element;

std::unique_ptr<std::string> scanStr(std::stringstream &input);

std::unique_ptr<Element> scanEl(std::stringstream &input);

std::unique_ptr<std::vector<std::unique_ptr<Object_field>>> scanObj(std::stringstream &input);

bool checkCorrect(std::vector<std::string> &dataVector);

#endif // JSON_LAMBDA_H

#endif
