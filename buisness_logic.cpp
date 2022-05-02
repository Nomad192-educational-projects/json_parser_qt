#include "buisness_logic.h"
#include "json_lambda.h"
#include <fstream>

using namespace std;

static bool openFile(string nameFile,  vector<string> &dataVector);

bool interfaceBundle(enum compilation comp, string nameFile,  vector<string> &dataVector){
    bool result;
    switch (comp){
        case OPEN_FILE: result = openFile(nameFile, dataVector); break;
        case CHECK:     result = checkCorrect(dataVector); break;
    }
    return result;
}

static bool openFile(string nameFile, vector<string> &dataVector){
    fstream dataFile(nameFile);
    bool result = false;
    if(dataFile.is_open()){
        result = true;
        for(string line; getline(dataFile, line); dataVector.push_back(line));
        dataFile.close();
    }
    return result;
}
