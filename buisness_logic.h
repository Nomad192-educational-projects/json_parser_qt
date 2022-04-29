#ifndef BUISNESS_LOGIC_H
#define BUISNESS_LOGIC_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

using namespace std;

enum compilation{
   OPEN_FILE,
   CHECK,
};

bool interfaceBundle(enum compilation comp, string nameFile,  vector<string> &dataVector);

#endif // BUISNESS_LOGIC_H
