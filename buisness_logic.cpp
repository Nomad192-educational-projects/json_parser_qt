#include "buisness_logic.h"

static bool openFile(string *nameFile,  vector<string> *dataVector);

bool interfaceBundle(enum compilation comp, void *req, void *answer,  vector<string> *dataVector){
    bool result;
    switch (comp){
        case OPEN_FILE: result = openFile((string*)req, dataVector); break;
    }
    return result;
}

static bool openFile(string *nameFile, vector<string> *dataVector){
    fstream dataFile(*nameFile, ios::in);
    bool result = false;
    if(dataFile.is_open()){
        result = true;
        for(string line; getline(dataFile, line); dataVector->push_back(line));
        dataFile.close();
    }
    return result;
}
