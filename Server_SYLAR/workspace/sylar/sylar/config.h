//P09 11:34
#ifndef __SYLAR_CONFIG_H__
#define __SYLAR_CONFIG_H__
#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "sylar/log.h"
#include <yaml-cpp/yaml.h>


namespace sylar{

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
    T oeprator()(const F& v){
        return boost::lexical_cast<T>(v);
    }
};

template<class T>
class LexicalCast<std::string,std::vector<T>>{
public:
    std::vector<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
    }
};

template<class T, class FromStr = LexicalCast<std::string,T>
            ,class ToStr = LexicalCast<T,std::string>>
class ConfigVar :public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name,description)
        ,m_val(default_value){
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
                    << e.what() << "convert: string to " << typeid(m_val).name();
        }
        return false;
            
    }

    const T getValue() const {return m_val;} 
    void setValue(const T& v){m_val = v;}
private:
    T m_val;

};

class Config{
public:
    typedef std::map<std::string, ConfigVarBase::ptr>ConfigVarMap;

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
            const T& default_value, const std::string& description = ""){
        auto tmp = Lookup(name);
        if(tmp){
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name= " << name << "exists";
            return tmp;
        }
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") 
            != std::string::npos) {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
                throw std::invalid_argument(name);
            }

            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
            s_datas[name] = v;
            return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name){
        auto it = s_datas.find(name);
        if(it == s_datas.end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase LookupBase(const std::string& name);
private:
    static ConfigVarMap m_datas;
};


}



#endif