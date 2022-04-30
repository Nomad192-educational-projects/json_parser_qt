#ifndef BUISNESS_LOGIC_H
#define BUISNESS_LOGIC_H

#include <string>
#include <vector>

enum compilation{
   OPEN_FILE,
   CHECK,
};

bool interfaceBundle(enum compilation comp, std::string nameFile,  std::vector<std::string> &dataVector);

#endif // BUISNESS_LOGIC_H
