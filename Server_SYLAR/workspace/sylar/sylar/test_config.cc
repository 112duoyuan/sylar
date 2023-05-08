#include "sylar/config.h"
#include "sylar/log.h"
#include <yaml-cpp/yaml.h>

sylar::ConfigVar<int>::ptr g_int_value_config = 
    sylar::Config::Lokkup("system.port",(int)8080,"system port");
sylar::ConfigVar<int>::ptr g_float_value_config = 
    sylar::Config::Lokkup("system.port",(int)8080,"system port");

void test_yaml(){
    YAML::Node root = YAML::LoadFile()
}

int main(int argc,char ** argv){
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << g_int_value_config->toString();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << g_float_value_config->getValue();
    return 0;
}