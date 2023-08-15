#ifndef __SYLAR_HTTP_CONNECTION_H__
#define __SYLAR_HTTP_CONNECTION_H__

#include "sylar/socket_steram.h"
#include "http.h"
#include "sylar/uri.h"

namespace sylar{
namespace http{

struct HttpResult{
    HttpResult(int _result
                ,HttpResponse::ptr _response
                ,const std::string& _error)
            :result(_result)
            ,response(_response)
            ,error(_error){}
    int result;
    HttpResponse::ptr response;
    std::string error;
};
class HttpConnection : public SocketStream{
public:
    typedef std::shared_ptr<HttpConnection>ptr;
    enum class Error{
        OK = 0,
        INVALID_URL = 1,
        INVALID_HOST =2,
        CONNECT_FAIL = 3,
        SEND_CLOSE_BY_PEER = 4,
        SEND_SOCKET_ERROR = 5,
        TIMEOUT = 6
    };
    static HttpResult::ptr DoGet(const std::string& uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");

    static HttpResult::ptr DoGet(Uri::ptr uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");    

    static HttpResult::ptr DoPost(const std::string& uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");
    
    static HttpResult::ptr DoPost(Uri::ptr uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");
    
    static HttpResult::ptr DoRequest(HttpMethod method
                        , const std::string& uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");

    static HttpResult::ptr DoRequest(HttpMethod method
                        , Uri::ptr uri
                        , uint64_t timeout_ms
                        , const std::map<std::string,std::string>&headers ={}
                        , const std::string& body = "");

    static HttpResult::ptr DoRequest(HttpRequest::ptr req
                        , Uri::ptr uri
                        , uint64_t timeout_ms);


    HttpConnection(Socket::ptr sock,bool owner = true);
    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr req);


};   
}

}    



#endif