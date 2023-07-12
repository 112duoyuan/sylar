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

#include "pthread.h"

namespace sylar{
/*
    ConfigVarBase基础功能：
        1、将名字转为小写
        2、获取名字、描述的功能
*/
class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name,const std::string& description = "")
        :m_name(name)
        ,m_description(description){
            std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);
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
    //重载 ()操作符
    T operator()(const F& v){
        // 模板lexical_cast 能把F类型转成T类型
        return boost::lexical_cast<T>(v);
    }
};

template<class T>
class LexicalCast<std::string,std::vector<T>>{
public:
    std::vector<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        //指出模板声明（或定义）中的非独立名称是类型名，而非变量名。
        typename std::vector<T>vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            //调用str()时会返回一个临时string对象；str("")用于清空内容
            ss.str("");
            ss << node[i];
            //LexicalCast转换ss string内容为类型T，在压入vector中
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



template<class T, class FromStr = LexicalCast<std::string,T>
            ,class ToStr = LexicalCast<T,std::string>>
class ConfigVar :public ConfigVarBase{
public:
    typedef RWMutex RWMutexType;
    //对象实例on_change_cb包装 函数指针、函数、模板函数、函数对象、模板对象函数
    typedef std::function<void (const T& old_value,const T& new_value)> on_change_cb;
    typedef std::shared_ptr<ConfigVar> ptr;
    
    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name,description)
        ,m_val(default_value){
        }
    //利用模板偏特化将传入m_val转为string数据
    std::string toString() override{
        try{
            //进行字符串与整数/浮点数之间的字面转换
            //boost::lexical_cast<std::string>(m_val);
            RWMutexType::ReadLock lock(m_mutex);
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

    const T getValue() {
        RWMutexType::ReadLock lock(m_mutex);    
        return m_val;
    } 
    void setValue(const T& v){
        {
            RWMutexType::ReadLock lock(m_mutex);
            if(v == m_val){
                return;
            }
            for(auto& i:m_cbs){
                i.second(m_val,v);
            }
        }
        RWMutexType::WriteLock lock(m_mutex);
        m_val = v;
    }

    std::string getTypeName() const override {return typeid(T).name();}

    uint64_t addListener(on_change_cb cb){
        static uint64_t s_fun_id = 0;
        RWMutexType::WriteLock lock(m_mutex);
        ++s_fun_id;
        m_cbs[s_fun_id] = cb;
        return s_fun_id;
    }

    void delListener(uint64_t key){
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.erase(key);
    }
    on_change_cb getListener(uint64_t key){
        RWMutexType::ReadLock lock(m_mutex);
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    void clearListener(){
        RWMutexType::ReadLock lock(m_mutex);
        m_cbs.clear();
    }
private:
    RWMutexType m_mutex;
    T m_val;
    //变更回调数组，uint64_t key 要求唯一，一般可以用hash
    std::map<uint64_t,on_change_cb> m_cbs;
};

class Config{
public:
    typedef std::map<std::string, ConfigVarBase::ptr>ConfigVarMap;
    typedef RWMutex RWMutexType;
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
            const T& default_value, const std::string& description = ""){
        RWMutexType::WriteLock lock(GetMutex());
        auto it = GetDatas().find(name);
        if(it != GetDatas().end()){
            //类型强制转换 智能指针转换
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
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") 
            != std::string::npos) {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
                throw std::invalid_argument(name);
            }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
        GetDatas()[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name){
        RWMutexType::ReadLock lock(GetMutex());
        auto it = GetDatas().find(name);
        if(it == GetDatas().end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);

    static void Visit(std::function<void(ConfigVarBase::ptr)> cb);
private:
    static ConfigVarMap& GetDatas(){
        static ConfigVarMap s_datas;
        return s_datas;
    }
    static RWMutexType& GetMutex(){
        static RWMutexType s_mutex;
        return s_mutex;
    }
};


}



#endif
