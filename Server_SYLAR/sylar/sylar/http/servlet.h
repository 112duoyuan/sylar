#ifndef __HTTP_SERVLET_H__
#define __HTTP_SERVLET_H__

#include <memory>
#include <functional>
#include <string>
#include "http_session.h"
#include "http.h"
#include <vector>
#include <unordered_map>
#include "../thread.h"

namespace sylar{
namespace http{

class Servlet{
public:
    typedef std::shared_ptr<Servlet>ptr;
    Servlet(const std::string& name)
        :m_name(name){}
    virtual ~Servlet(){}
    virtual int32_t handle(sylar::http::HttpRequest::ptr request
                    ,sylar::http::HttpResponse::ptr response
                    ,sylar::http::HttpSession::ptr session) = 0;
    const std::string& getName() const {return m_name;}
protected:
    //名称
    std::string m_name;
};

/**
 * @brief 函数式Servlet
 */
class FunctionServlet : public Servlet{
public:
    typedef std::shared_ptr<FunctionServlet>ptr;

    typedef std::function<int32_t (sylar::http::HttpRequest::ptr request
                    ,sylar::http::HttpResponse::ptr response
                    ,sylar::http::HttpSession::ptr session)>callback;


    /**
     * @brief 构造函数
     * @param[in] cb 回调函数
     */
    FunctionServlet(callback cb);
    virtual int32_t handle(sylar::http::HttpRequest::ptr request
                    ,sylar::http::HttpResponse::ptr response
                    ,sylar::http::HttpSession::ptr session)override;
private:
    // 回调函数
    callback m_cb;
};

/**
 * @brief Servlet分发器
 */
class ServletDispatch : public Servlet{
public:
    typedef std::shared_ptr<ServletDispatch>ptr;
    typedef RWMutex RWMutexType;

    ServletDispatch();
    virtual int32_t handle(sylar::http::HttpRequest::ptr request
                    ,sylar::http::HttpResponse::ptr response
                    ,sylar::http::HttpSession::ptr session)override;

    /**
     * @brief 添加servlet
     * @param[in] uri uri
     * @param[in] slt serlvet
     */
    void addServlet(const std::string& uri,Servlet::ptr slt);

        /**
     * @brief 添加servlet
     * @param[in] uri uri
     * @param[in] cb FunctionServlet回调函数
     */

    void addServlet(const std::string& uri,FunctionServlet::callback cb);

        /**
     * @brief 添加模糊匹配servlet
     * @param[in] uri uri 模糊匹配 /sylar_*
     * @param[in] slt servlet
     */
    void addGlobServlet(const std::string& uri,FunctionServlet::callback cb);
       
        /**
     * @brief 添加模糊匹配servlet
     * @param[in] uri uri 模糊匹配 /sylar_*
     * @param[in] slt FunctionServlet回调函数
     */
    void addGlobServlet(const std::string& uri,Servlet::ptr slt);


    void delServlet(const std::string& uri);
    void delGlobServlet(const std::string& uri);

    Servlet::ptr getDefault() const {return m_default;}
    void setDefault(Servlet::ptr v){m_default = v;}
    
    Servlet::ptr getServlet(const std::string& uri);
    Servlet::ptr getGlobServlet(const std::string& uri);

    Servlet::ptr getMatchedServlet(const std::string& uri);
private:

    RWMutexType m_mutex;
    // uri-> servlet 精确匹配
    std::unordered_map<std::string, Servlet::ptr>m_datas;
    //uri -> servlet 模糊匹配
    std::vector<std::pair<std::string,Servlet::ptr>>m_globs;
    //默认servlet,所有路径都没匹配到时使用
    Servlet::ptr m_default;
};

class NotFoundServlet : public Servlet{
public:
    typedef std::shared_ptr<NotFoundServlet>ptr;
    NotFoundServlet();
    virtual int32_t handle(sylar::http::HttpRequest::ptr request
                    ,sylar::http::HttpResponse::ptr response
                    ,sylar::http::HttpSession::ptr session)override;

};
}
} 

#endif