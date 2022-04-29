#include "buisness_logic.h"

static bool openFile(string nameFile,  vector<string> &dataVector);
static bool checkCorrect(vector<string> &dataVector);

bool interfaceBundle(enum compilation comp, string nameFile,  vector<string> &dataVector){
    bool result;
    switch (comp){
        case OPEN_FILE: result = openFile(nameFile, dataVector); break;
        case CHECK:     result = checkCorrect(dataVector); break;
    }
    return result;
}

static bool openFile(string nameFile, vector<string> &dataVector){
    fstream dataFile(nameFile, ios::in);
    bool result = false;
    if(dataFile.is_open()){
        result = true;
        for(string line; getline(dataFile, line); dataVector.push_back(line));
        dataFile.close();
    }
    return result;
}

enum Element_Type{
    STRING,
    NUMBER,
    OBJECT,
    ARRAY,
    OB_TRUE,
    OB_FALSE,
    OB_NULL,
};

class Object_field;
class Element;

class Element
{
private:
    Element_Type type;
    //void *value;
    unique_ptr<void, void(*)(void*)> value;

public:
    Element(Element_Type type, unique_ptr<void, void(*)(void*)> value) : type(type), value(move(value))
    {
        /*switch(type)
        {
            case STRING:    value = make_unique<string>();                  break;
            case NUMBER:    value = make_unique<double>();                  break;
            case OBJECT:    value = make_unique<vector<Object_field>>();    break;
            case ARRAY:     value = make_unique<vector<Element>>();         break;
            case OB_TRUE:   value = NULL;                                   break;
            case OB_FALSE:  value = NULL;                                   break;
            case OB_NULL:   value = NULL;                                   break;
        }*/
    }

    /*~Element()
    {
        switch(type)
        {
            case STRING:    delete (string*)(value);                break;
            case NUMBER:    delete (double*)(value);                break;
            case OBJECT:    delete (vector<unique_ptr<Object_field>> *) (value);  break;
            case ARRAY:     delete (vector<unique_ptr<Element>>*)(value);       break;
            case OB_TRUE:                                           break;
            case OB_FALSE:                                          break;
            case OB_NULL:                                           break;
        }
    }*/
};

class Object_field
{
private:
    string key;
    unique_ptr<Element> element;

public:
    Object_field(string key, unique_ptr<Element> element) : key(key), element(move(element)) {}
    /*~Object_field()
    {
        delete element;
    }*/
};

static unique_ptr<string> scanStr(stringstream &input)
{
    cout << "Scan str:" << endl;
    unique_ptr<string> result = make_unique<string>();
    char c;
    while(input.get(c))
    {
        if(c == '\"')
            break;
    }
    while(input.get(c))
    {
        if(c == '\"')
        {
            cout << "End scan str, res=\"" << *result << "\"" << endl;
            return move(result);
        }
        (*result) += c;
        cout << "Scan str char:\'" << c << "\'" << endl;
    }
    //delete result;
    return nullptr;
}

static unique_ptr<vector<unique_ptr<Object_field>>> scanObj(stringstream &input);

static unique_ptr<Element> scanEl(stringstream &input)
{
    cout << "Scan el" << endl;
    unique_ptr<Element> result;
    char c;
    while(input.get(c))
    {
        if(c != ' ' && c != '\n' && c != '\t')
            break;
    }
    /*if(!input->get(c))
    {
        return nullptr;
    }*/
    if (c >= '0' && c <= '9')
    {
        cout << "Number" << endl;

        input.unget();
        unique_ptr<double> value = make_unique<double>();
        input >> *value;
        cout << "Double val=" << *value << endl;
        unique_ptr<void, void(*)(void*)> new_val(value.release(), [](void* ptr)
            {
                delete (double*)ptr;
            });
        result = make_unique<Element>(Element_Type{NUMBER}, move(new_val));
    }
    else
    {
        switch(c)
        {
            case '\"':
            {
                cout << "String" << endl;
                input.unget();
                unique_ptr<string> value = make_unique<string>();
                //string *value;
                if((value = scanStr(input)).get() == nullptr)
                {
                    return nullptr;
                }
                unique_ptr<void, void(*)(void*)> new_val(value.release(), [](void* ptr)
                    {
                        delete (string*)ptr;
                    });
                result = make_unique<Element>(Element_Type{STRING}, move(new_val));
                break;
            }
            case '{':
            {
                cout << "Object" << endl;
                input.unget();
                unique_ptr<vector<unique_ptr<Object_field>>> value;
                if((value = scanObj(input)).get() == nullptr)
                {
                    return nullptr;
                }
                unique_ptr<void, void(*)(void*)> new_val(value.release(), [](void* ptr)
                    {
                        delete (vector<unique_ptr<Object_field>>*)ptr;
                    });
                result = make_unique<Element>(Element_Type{OBJECT}, move(new_val));
                break;
            }
            case '[':
            {
                cout << "Array" << endl;
                unique_ptr<vector<unique_ptr<Element>>> value = make_unique<vector<unique_ptr<Element>>>();
                input.unget();
                char ch;
                while(input.get(ch))
                {
                    if(ch == ']')
                        break;
                    else if(ch != ',' && ch != '[')
                    {
                        //delete value;
                        return nullptr;
                    }
                    unique_ptr<Element> buffer;
                    if((buffer = scanEl(input)).get() == nullptr)
                    {
                        //delete value;
                        return nullptr;
                    }
                    cout << "Array el ok" << endl;
                    value->push_back(move(buffer));
                    cout << "Array el push ok" << endl;
                }
                if(ch != ']')
                {
                    //delete value;
                    return nullptr;
                }
                cout << "Array ok" << endl;
                unique_ptr<void, void(*)(void*)> new_val(value.release(), [](void* ptr)
                    {
                        delete (vector<unique_ptr<Element>>*)ptr;
                    });
                result = make_unique<Element>(Element_Type{ARRAY}, move(new_val));
                cout << "Array result ok" << endl;
                break;
            }
            case 't':
            {
                cout << "True" << endl;
                input.unget();
                string value;
                input >> value;
                if(value == "true," || value == "true}")
                {
                    input.unget();
                }
                else if(value != "true")
                    return nullptr;
                unique_ptr<void, void(*)(void*)> new_val(new bool , [](void* ptr)
                    {
                        delete (bool*) ptr;
                    });
                result = make_unique<Element>(Element_Type{OB_TRUE}, move(new_val));
                break;
            }
            case 'f':
            {
                cout << "False" << endl;
                input.unget();
                string value;
                input >> value;
                if(value == "false," || value == "false}")
                {
                    input.unget();
                }
                else if(value != "false")
                    return nullptr;

                unique_ptr<void, void(*)(void*)> new_val(new bool, [](void* ptr)
                    {
                        delete (bool*) ptr;
                    });
                result = make_unique<Element>(Element_Type{OB_FALSE}, move(new_val));
                break;
            }
            case 'n':
            {
                cout << "Null" << endl;
                input.unget();
                string value;
                input >> value;
                if(value == "null," || value == "null}")
                {
                    input.unget();
                }
                else if(value != "null")
                    return nullptr;

                unique_ptr<void, void(*)(void*)> new_val(new bool, [](void* ptr)
                    {
                        delete (bool*) ptr;
                    });
                result = make_unique<Element>(Element_Type{OB_NULL}, move(new_val));
                break;
            }
            default:
            {
                cout << "Non" << endl;
                return nullptr;
            }
        }
    }

    return result;
}

static unique_ptr<vector<unique_ptr<Object_field>>> scanObj(stringstream &input)
{
    cout << "Scan obj" << endl;
    unique_ptr<vector <unique_ptr<Object_field>>> result = make_unique<vector<unique_ptr<Object_field>>>();

    char ch;
    while(input.get(ch))
    {
        if(ch != ' ' && ch != '\n' && ch != '\t')
        {
            input.unget();
            break;
        }
    }
    while(input.get(ch))
    {
        cout << "Char while = \"" << ch << "\"" << endl;
        input.unget();
        while(input.get(ch))
            if(ch != ' ' && ch != '\n' && ch != '\t')
                break;

        cout << "Char:\'" << ch << "\'" << endl;
        if(ch == '}')
            return move(result);
        else if(ch != ',' && ch != '{')
        {
            //delete result;
            return nullptr;
        }
        cout << "Continuation" << endl;
        unique_ptr<string> key;
        if((key = scanStr(input)).get() == nullptr)
        {
            //delete result;
            return nullptr;
        }
        cout << "Key:\"" << *key << "\"" << endl;
        char c;
        input >> c;
        if(c != ':')
        {
            //delete key;
            //delete result;
            return nullptr;
        }
        cout << "Char del:\'" << c << "\'" << endl;

        unique_ptr<Element> value;
        if((value = scanEl(input)).get() == nullptr)
        {
            //delete key;
            //delete result;
            return nullptr;
        }
        cout << "Value ok" << endl;
        //if(value->type == NUMBER)
            //cout << "Value double=" << *((double *)value->value) << endl;

        unique_ptr<Object_field> obj_f = make_unique<Object_field>(*key, move(value));
        result->push_back(move(obj_f));
        cout << "Push ok" << endl;
        //delete key;
        cout << "Next key + val" << endl;
    }

    //delete result;
    return nullptr;
}

static bool checkCorrect(vector<string> &dataVector)
{
    string full;
    for(string i : dataVector)
        full += i + '\n';

    stringstream json_stream(full);
    unique_ptr<Element> res;
    if((res = scanEl(json_stream)).get() == nullptr)
        return false;

    char ch;
    while(json_stream.get(ch))
        if(ch != ' ' && ch != '\n' && ch != '\t')
            return false;
    //delete res;
    return true;
}


