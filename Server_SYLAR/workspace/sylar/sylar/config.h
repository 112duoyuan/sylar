//P09 11:34
#ifndef __SYLAR_CONFIG_H__
#define __SYLAR_CONFIG_H__
#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "log.h"
#include <yaml-cpp/yaml.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <iostream>


namespace sylar{

class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name,const std::string& description = "")
        :m_name(name)
        ,m_description(description){
            std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);
            std::cout << "configvarbase" << std::endl;
        }
        virtual ~ConfigVarBase(){}
        const std::string& getName() const {return m_name;}
        const std::string& getDescription() const {return m_description;}

        virtual std::string toString() = 0;
        virtual bool fromString(const std::string& val) = 0;
        virtual std::string getTypeName() const =  0;
protected:
    std::string m_name;
    std::string m_description;
};
//FromStr T operator()(const std::string&)
//ToStr std::string operator() (const T&)
//from_type to_type
template<class F ,class T>
class LexicalCast{
public:
    T operator()(const F& v){
        //用于字面值转换
        // integer -> string
        // string- > integer
        // integer -> char
        // float- > string
        return boost::lexical_cast<T>(v);
    }
};

template<class T>
class LexicalCast<std::string,std::vector<T>>{
public:
    std::vector<T> operator()(const std::string& v){
        //读取yaml配置文件 load函数将数据转为列表或字典
        YAML::Node node = YAML::Load(v);
        typename std::vector<T>vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::vector<T>,std::string>{
public:
    std::string operator()(const std::vector<T>& v){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string,std::list<T>>{
public:
    std::list<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::list<T>vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::list<T>,std::string>{
public:
    std::string operator()(const std::list<T>& v){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string,std::set<T>>{
public:
    std::set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::set<T>vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::set<T>,std::string>{
public:
    std::string operator()(const std::set<T>& v){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string,std::unordered_set<T>>{
public:
    std::unordered_set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T>vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::unordered_set<T>,std::string>{
public:
    std::string operator()(const std::unordered_set<T>& v){
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v){
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string,std::map<std::string , T>>{
public:
    std::map<std::string ,T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T>vec;
        std::stringstream ss;
        for(auto it = node.begin();
                it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                LexicalCast<std::string ,T>()(ss.str())));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::map<std::string ,T>,std::string>{
public:
    std::string operator()(const std::map<std::string ,T>&v){
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v){
            node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string,std::unordered_map<std::string , T>>{
public:
    std::unordered_map<std::string ,T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T>vec;
        std::stringstream ss;
        for(auto it = node.begin();
                it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                LexicalCast<std::string ,T>()(ss.str())));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::unordered_map<std::string ,T>,std::string>{
public:
    std::string operator()(const std::unordered_map<std::string ,T>&v){
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v){
            node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


//Config ConfigVarBase
template<class T, class FromStr = LexicalCast<std::string,T>
            ,class ToStr = LexicalCast<T,std::string>>
class ConfigVar :public ConfigVarBase{
public:
    //回调函数
    typedef std::function<void (const T& old_value,const T& new_value)> on_change_cb;
    typedef std::shared_ptr<ConfigVar> ptr;
    
    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name,description)
        ,m_val(default_value){
            std::cout << "configVar" << std::endl;
        }
    std::string toString() override{
        try{
            //进行字符串与整数/浮点数之间的字面转换
            //boost::lexical_cast<std::string>(m_val);
            return ToStr()(m_val);
        }catch(std::exception& e){
            //typeid 获取类型信息
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) <<  "ConfigVar::toString exception"
                << e.what() << "convert: " << typeid(m_val).name() << "to string";
        }
        return "";
    }
    bool fromString(const std::string& val) override{
        try{
            //m_val = boost::
            setValue(FromStr()(val));
        }catch (std::exception& e){
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception"    
                    << e.what() << "convert: string to " << typeid(m_val).name()
                    << " - " <<val;
        }
        return false;
            
    }

    const T getValue() const {return m_val;} 
    void setValue(const T& v){
        if(v == m_val){
            return;
        }
        for(auto& i:m_cbs){
            i.second(m_val,v);
        }
        m_val = v;
    }

    std::string getTypeName() const override {return typeid(T).name();}

    void addListener(uint64_t key,on_change_cb cb){
        m_cbs[key] = cb;
    }

    void deleteListener(uint64_t key){
        m_cbs.erase.erase(key);
    }
    on_change_cb getListener(uint64_t key){
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    void clearListener(){
        m_cbs.clear();
    }
private:
    T m_val;
    //变更回调数组，uint64_t key 要求唯一，一般可以用hash
    std::map<uint64_t,on_change_cb> m_cbs;
};

class Config{
public:
    typedef std::map<std::string, ConfigVarBase::ptr>ConfigVarMap;

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
            const T& default_value, const std::string& description = ""){

        std::cout << "name: " << name << std::endl;

        std::cout << "type is "<<typeid(default_value).name() << std::endl;
        std::cout << "description: "<<description << std::endl;

        auto it = m_datas.find(name);

        if(it != m_datas.end()){
            //类型强制转换
            std::cout << "find name!! " <<  std::endl;
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
            if(tmp){
                SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name= " << name << "exists";
                return tmp;
            } else{
                std::cout << "name " << name << std::endl;
                std::cout << typeid(T).name() << std::endl;
                std::cout << it->second->getTypeName() << std::endl;

                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name= " << name << "exists but type not"
                        << typeid(T).name() << " real_type = " << it->second->getTypeName()
                            <<" " << it->second->toString();
                return nullptr;
            }
        }
        std::cout << "can't find name!! " <<  std::endl;
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") 
            != std::string::npos) {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
                throw std::invalid_argument(name);
            }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
        m_datas[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name){
        auto it = m_datas.find(name);
        if(it == m_datas.end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);
private:
    static ConfigVarMap m_datas;
};


}



#endif