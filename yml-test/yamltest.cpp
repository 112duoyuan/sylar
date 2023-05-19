#include <iostream>
#include "yaml-cpp/yaml.h"

using namespace std;
int main(int argc,char **argv)
{
    YAML::Node config;
    try{
        config = YAML::LoadFile("../test.yml");
    }catch(...){
        printf("error loading file,yaml file error or not exist.\n");
        return 0;
    }
    for(auto it = config.begin();it != config.end();++it){
        std::string key = it->first.as<std::string>();
        YAML::Node value = it->second;
        switch(value.Type()){
            case YAML::NodeType::Scalar:
            printf("key: %s scalar\n",key.c_str());
            break;
            case YAML::NodeType::Map:
            printf("key: %s Map\n",key.c_str());
            break;
            case YAML::NodeType::Null:
            printf("key: %s Null\n",key.c_str());
            break;
            case YAML::NodeType::Sequence:
            printf("key: %s Sequence\n",key.c_str());
            break;
            case YAML::NodeType::Undefined:
            printf("key: %s Undefined\n",key.c_str());
            break;
        }
    }

    cout << "version: " << config["version"].as<float>() << endl;
    cout << "version(str): " << config["version"].as<string>() << endl;
    cout << "need: " << config["need"].as<bool>() << endl;
    cout << "empty: " << config["empty"].as<string>() << endl;
    cout << "time: "<<config["time"].as<string>() << endl;

    for(auto it :config["fruit"])
        cout << "fruit: "<<it.as<string>() << endl;
    for(auto it : config["multi"]["sta"])
        cout << "multi sta: " <<it.as<string>() << endl;
    cout << "my name: "<<config["my"]["name"].as<string>() << endl;
    cout << "my name1: "<< config["my"]["name1"].as<string>() << endl;
    cout << "my age: " << config["my"]["age"].as<int>() << endl;
    return 0;
}