#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

// map to store json objects , key - value pairs as strings

class Jsonobject
{
private:
    map<string, string> mp;

public:
    void addData(string &key, string &val)
    {
        mp[key] = val;
    }

    void display()
    {
        for (auto u : mp)
        {
            cout << u.first << ":" << u.second << endl;
        }
    }
};

class Jsonparser
{
public:
    Jsonobject parse(string &json)
    {
        Jsonobject jsonObj;
        size_t start = 1;
        size_t end = json.find('}', start);

        string pairs = json.substr(start, end - start);
        vector<string> token = tokenizeComma(pairs);

        for (string &u : token)
        {
            auto keyVal = splitbyColon(u);
            string key = cleanString(keyVal.first);
            string value = cleanString(keyVal.second);
            // jsonObj.addData(key, value);

            if (isObject(value))
            {

                Jsonobject nested = parse(value);
            }
            else
            {

                jsonObj.addData(key, value);
            }
        }
        return jsonObj;
    }

private:
    vector<string> tokenizeComma(string &str)
    {
        vector<string> toks;
        size_t start = 0;
        size_t end = 0;

        while ((end = str.find(',', start)) != string::npos)
        {
            toks.push_back(str.substr(start, end - start));
            start = end + 1;
        }

        toks.push_back(str.substr(start));
        return toks;
    }

    pair<string, string> splitbyColon(string &str)
    {
        size_t pos = str.find(':');
        string key = str.substr(0, pos);
        string value = str.substr(pos + 1);
        return {key, value};
    }

    string cleanString(string &str)
    {
        string cleaned = str;
        cleaned.erase(remove_if(cleaned.begin(), cleaned.end(), ::isspace), cleaned.end());
        cleaned.erase(remove(cleaned.begin(), cleaned.end(), '"'), cleaned.end());
        return cleaned;
    }

    bool isObject(const string &str)
    {
        return str.front() == '{' && str.back() == '}';
    }
};

int main()
{

    ifstream jsonFile("data.json");

    stringstream buffer;
    buffer << jsonFile.rdbuf();
    string json_content = buffer.str();

    Jsonparser parser;
    Jsonobject obj = parser.parse(json_content);

    obj.display();

    return 0;
}