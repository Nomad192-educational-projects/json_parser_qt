#ifndef BUISNESS_LOGIC_H
#define BUISNESS_LOGIC_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

enum compilation{
   OPEN_FILE,
   CHECK,
};

bool interfaceBundle(enum compilation comp, void *req, void *answer,  vector<string> *dataVector);

#endif // BUISNESS_LOGIC_H
