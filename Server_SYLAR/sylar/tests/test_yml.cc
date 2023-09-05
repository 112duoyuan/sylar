#include "../sylar/sylar.h"
#include <list>
#include <sstream>
void getall(std::string prefix,const YAML::Node node
            ,std::list<std::pair<std::string,YAML::Node>>&output){
    if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos){
        std::cout << "invalid prefix !!!" << std::endl;
    }
    output.push_back(make_pair(prefix,node));
    if(node.IsMap()){
        for(auto it = node.begin();it != node.end();it++){
            std::cout << it->first.Scalar() << std::endl;
            getall(prefix.empty()?
                it->first.Scalar() : prefix+"."+it->first.Scalar(),
                    it->second,output);
        }
        return;
    }
    if(node.IsSequence()){
        for(size_t i = 0; i < node.size();i++){
            getall(prefix.empty()?
                node[i].Scalar():prefix + "." + node[i].Scalar(),
                    node[i],output);
        }
    }   
}
void loadfromyml(const YAML::Node root){
    std::list<std::pair<std::string,YAML::Node>> vec;
    getall("",root,vec);
    for(auto &i : vec){
        std::cout << i.first << " " << i.second.Scalar() << std::endl;
    }
}

int main(int argc,char ** agrv){
    YAML::Node root = YAML::LoadFile("/home/xu/Server_SYLAR/sylar/bin/conf/log.yml");
    std::stringstream ss;
    ss << root;
    //std::cout << ss.str() << std::endl;
    loadfromyml(root);
    return 0;
}