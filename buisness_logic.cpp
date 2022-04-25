#include "buisness_logic.h"

static bool openFile(string *nameFile,  vector<string> *dataVector);
static bool checkCorrect(vector<string> *dataVector);

bool interfaceBundle(enum compilation comp, void *req, void *answer,  vector<string> *dataVector){
    bool result;
    switch (comp){
        case OPEN_FILE: result = openFile((string*)req, dataVector); break;
        case CHECK:     result = checkCorrect(dataVector); break;
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

enum Element_Type{
    STRING,
    NUMBER,
    OBJECT,
    ARRAY,
    OB_TRUE,
    OB_FALSE,
    OB_NULL,
};

struct Object_field;

class Element
{
private:
    Element_Type type;
    void *value;

public:
    Element(Element_Type type, void *value) : type(type), value(value)
    {
        /*switch(type)
        {
            case STRING:    value = new string;                 break;
            case NUMBER:    value = new double;                 break;
            case OBJECT:    value = new vector<Object_field>;   break;
            case ARRAY:     value = new vector<Element>;        break;
            case OB_TRUE:   value = NULL;                       break;
            case OB_FALSE:  value = NULL;                       break;
            case OB_NULL:   value = NULL;                       break;
        }*/
    }

    ~Element()
    {
        switch(type)
        {
            case STRING:    delete (string*)(value);                break;
            case NUMBER:    delete (double*)(value);                break;
            case OBJECT:    delete (vector<Object_field *>*)(value);  break;
            case ARRAY:     delete (vector<Element *>*)(value);       break;
            case OB_TRUE:                                           break;
            case OB_FALSE:                                          break;
            case OB_NULL:                                           break;
        }
    }
};

class Object_field
{
private:
    string key;
    Element *element;

public:
    Object_field(string key, Element *element)
    {
        this->key = key;
        this->element = element;
    }
    ~Object_field()
    {
        delete element;
    }
};

static string *scanStr(stringstream *input)
{
    cout << "Scan str:" << endl;
    string *result = new string;
    char c;
    while(input->get(c))
    {
        if(c == '\"')
            break;
    }
    while(input->get(c))
    {
        if(c == '\"')
        {
            cout << "End scan str, res=\"" << *result << "\"" << endl;
            return result;
        }
        (*result) += c;
        cout << "Scan str char:\'" << c << "\'" << endl;
    }
    delete result;
    return NULL;
}

static vector<Object_field *> *scanObj(stringstream *input);

static Element *scanEl(stringstream *input)
{
    cout << "Scan el" << endl;
    Element *result;
    char c;
    while(input->get(c))
    {
        if(c != ' ' && c != '\n' && c != '\t')
            break;
    }
    /*if(!input->get(c))
    {
        return NULL;
    }*/
    if (c >= '0' && c <= '9')
    {
        cout << "Number" << endl;

        input->unget();
        double *value = new double;
        (*input) >> (*value);
        cout << "Double val=" << *value << endl;
        result = new Element(Element_Type{NUMBER}, value);
    }
    else
    {
        switch(c)
        {
            case '\"':
            {
                cout << "String" << endl;
                input->unget();
                string *value;
                if((value = scanStr(input)) == NULL)
                {
                    return NULL;
                }
                result = new Element(Element_Type{STRING}, value);
                break;
            }
            case '{':
            {
                cout << "Object" << endl;
                input->unget();
                vector<Object_field *> *value;
                if((value = scanObj(input)) == NULL)
                {
                    return NULL;
                }
                result = new Element(Element_Type{OBJECT}, value);
                break;
            }
            case '[':
            {
                cout << "Array" << endl;
                vector <Element *> *value = new vector<Element *>;
                input->unget();
                char ch;
                while(input->get(ch))
                {
                    if(ch == ']')
                        break;
                    else if(ch != ',' && ch != '[')
                    {
                        delete value;
                        return NULL;
                    }
                    Element *buffer;
                    if((buffer = scanEl(input)) == NULL)
                    {
                        delete value;
                        return NULL;
                    }
                    cout << "Array el ok" << endl;
                    value->push_back(buffer);
                    cout << "Array el push ok" << endl;
                }
                if(ch != ']')
                {
                    delete value;
                    return NULL;
                }
                cout << "Array ok" << endl;
                result = new Element(Element_Type{ARRAY}, value);
                cout << "Array result ok" << endl;
                break;
            }
            case 't':
            {
                cout << "True" << endl;
                input->unget();
                string value;
                (*input) >> value;
                if(value == "true," || value == "true}")
                {
                    input->unget();
                }
                else if(value != "true")
                    return NULL;

                result = new Element(Element_Type{OB_TRUE}, NULL);
                break;
            }
            case 'f':
            {
                cout << "False" << endl;
                input->unget();
                string value;
                (*input) >> value;
                if(value == "false," || value == "false}")
                {
                    input->unget();
                }
                else if(value != "false")
                    return NULL;

                result = new Element(Element_Type{OB_FALSE}, NULL);
                break;
            }
            case 'n':
            {
                cout << "Null" << endl;
                input->unget();
                string value;
                (*input) >> value;
                if(value == "null," || value == "null}")
                {
                    input->unget();
                }
                else if(value != "null")
                    return NULL;

                result = new Element(Element_Type{OB_NULL}, NULL);
                break;
            }
            default:
            {
                cout << "Non" << endl;
                return NULL;
            }
        }
    }

    return result;
}

static vector<Object_field *> *scanObj(stringstream *input)
{
    cout << "Scan obj" << endl;
    vector <Object_field *> *result = new vector<Object_field *>;

    char ch;
    while(input->get(ch))
    {
        if(ch != ' ' && ch != '\n' && ch != '\t')
        {
            input->unget();
            break;
        }
    }
    while(input->get(ch))
    {
        input->unget();
        while(input->get(ch))
        {
            if(ch != ' ' && ch != '\n' && ch != '\t')
            {
                input->unget();
                break;
            }
        }

        cout << "Char:\'" << ch << "\'" << endl;
        if(ch == '}')
            return result;
        else if(ch != ',' && ch != '{')
        {
            delete result;
            return NULL;
        }
        cout << "Continuation" << endl;
        string *key;
        if((key = scanStr(input)) == NULL)
        {
            delete result;
            return NULL;
        }
        cout << "Key:\"" << *key << "\"" << endl;
        char c;
        (*input) >> c;
        if(c != ':')
        {
            delete key;
            delete result;
            return NULL;
        }
        cout << "Char del:\'" << c << "\'" << endl;

        Element *value;
        if((value = scanEl(input)) == NULL)
        {
            delete key;
            delete result;
            return NULL;
        }
        cout << "Value ok" << endl;
        //if(value->type == NUMBER)
            //cout << "Value double=" << *((double *)value->value) << endl;
        Object_field *obj_f = new Object_field(*key, value);
        result->push_back(obj_f);
        cout << "Push ok" << endl;
        delete key;
        cout << "Next key + val" << endl;
    }

    delete result;
    return NULL;
}

static bool checkCorrect(vector<string> *dataVector)
{
    string full;
    for(string i : *dataVector)
        full += i + '\n';

    stringstream json_stream(full);
    Element *res;
    if((res = scanEl(&json_stream)) == NULL)
        return false;
    delete res;
    return true;
}


