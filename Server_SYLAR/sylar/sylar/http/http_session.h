#ifndef __SYLAR_HTTP_SESSION_H__
#define __SYLAR_HTTP_SESSION_H__

#include "../socket_steram.h"
#include "http.h"

namespace sylar{
namespace http{
class HttpSession : public SocketStream{
public:
    typedef std::shared_ptr<HttpSession>ptr;
        /**
     * @brief 构造函数
     * @param[in] sock Socket类型
     * @param[in] owner 是否托管
     */
    HttpSession(Socket::ptr sock,bool owner = true);

        /**
     * @brief 接收HTTP请求
     */
    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr rsp);


};   
}

}    



#endif