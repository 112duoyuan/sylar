#include "http_connection.h"
#include "http_parser.h"
#include "sylar/log.h"
namespace sylar{
namespace http{

static sylar::Logger::ptr g_logger =  SYLAR_LOG_NAME("system");   

HttpConnection::HttpConnection(Socket::ptr sock,bool owner)
    :SocketStream(sock,owner){

}
HttpResponse::ptr HttpConnection::recvResponse(){
    HttpResponseParser::ptr parser(new HttpResponseParser);
    uint64_t buff_size = HttpRequestParser::GetHttpRequestBufferSize();
    
    //uint64_t buff_size =150;
    
    std::shared_ptr<char> buffer(new char[buff_size + 1],[](char * ptr){
        delete[] ptr;
    });
    char * data = buffer.get();
    int offset = 0;

    do{
        int len = read(data + offset,buff_size - offset);
        if(len <= 0){
            close();
            return nullptr;
        }
        len+= offset;
        data[len]='\0';
        size_t nparse =parser->execute(data,len,true);
        if(parser->hasError()){
            close();
            return nullptr;
        }
        offset = len -nparse;
        if(offset == (int)buff_size){
            close();
            return nullptr;
        }
        if(parser->isFinished()){
            break;     
        }
    }while(true);
    auto client_parser = parser->getParser();
    if(client_parser.chunked){
        std::string body;
        int len = offset;
        do{
            do{
                int rt = read(data + len,buff_size - len);
                if(rt <= 0){
                    close();
                    return nullptr;
                }
                len += rt;
                data[len]='\0';
                size_t nparse = parser->execute(data,len,true);
                if(parser->hasError()){
                    close();
                    return nullptr;
                }
                len -= nparse;
                if(len == (int)buff_size){
                    close();
                    return nullptr;
                }

            }while(!parser->isFinished());
            len -= 2;

            SYLAR_LOG_INFO(g_logger) << "content_len=" << client_parser.content_len;
            if(client_parser.content_len <= len){
                body.append(data,client_parser.content_len);
                memmove(data, data + client_parser.content_len
                        , len -client_parser.content_len);
                len -= client_parser.content_len;
            }else{
                body.append(data,len);
                len = 0;
                int left = client_parser.content_len - len;
                while(left > 0){
                    int rt = read(data,left > (int)buff_size ? (int)buff_size : left);
                    if(rt <= 0){
                        return nullptr;
                    }
                    body.append(data,rt);
                    left -= rt;
                }
                len = 0;
            }
        }while(!client_parser.chunks_done);
    }else{
        uint64_t length =parser->getContentLength();
        if(length > 0){
            std::string body;
            body.resize(length);
            int len = 0;
            if(length >=  offset){
                memcpy(&body[0],data,offset);
                len = offset;
            }else{
                memcpy(&body[0],data,offset);
                len = offset;
            }
            length -= offset;
            if(length > 0){
                if(readFixSize(&body[body.size()],length) <= 0){
                    close();
                    return nullptr;
                }
            }
            parser->getData()->setBody(body);
        }
    }   
    return parser->getData();

}
int HttpConnection::sendRequest(HttpRequest::ptr rsp){
    std::stringstream ss;
    ss << *rsp;
    std::string data = ss.str();
    writeFixSize(data.c_str(),data.size());
}

HttpResult::ptr HttpConnection::DoGet(const std::string& uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers 
            , const std::string& body){
    Uri::ptr uri = Uri::Create(uri);
    if(!uri){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::INVALID_URL
            , nullptr,"invalid url: "+ url);
    }
    return DoPoGet(uri,timeout_ms,headers,body);
}

HttpResult::ptr HttpConnection::DoGet(Uri::ptr uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers 
            , const std::string& body){
    return DoRequest(HttpMethod::GET,uri,timeout_ms,headers,body);
}   

HttpResult::ptr HttpConnection::DoPost(const std::string& uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers 
            , const std::string& body){
    Uri::ptr uri = Uri::Create(uri);
    if(!uri){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::INVALID_URL
            , nullptr,"invalid url: "+ url);
    }
    return DoPost(uri,timeout_ms,headers,body);
}

HttpResult::ptr HttpConnection::DoPost(Uri::ptr uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers 
            , const std::string& body){
    return  DoRequest(HttpMethod::POST,uri,timeout_ms,headers,body);
}

HttpResult::ptr HttpConnection::DoRequest(HttpMethod method
            , const std::string& uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers
            , const std::string& body){
    Uri::ptr uri = Uri::Create(uri);
    if(!uri){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::INVALID_URL
            , nullptr,"invalid url: "+ url);
    }
    return DoRequest(method,uri,timeout_ms,headers,body);
}

HttpResult::ptr HttpConnection::DoRequest(HttpMethod method
            , Uri::ptr uri
            , uint64_t timeout_ms
            , const std::map<std::string,std::string>&headers
            , const std::string& body){
    HttpRequest::ptr req = std::make_shared<HttpRequest>();
    req->setPath(uri->getPath());
    req->setMethod(method);
    bool has_host = false;
    for(auto& i : headers){
        if(strcasecmp(i.first.c_str(),"connection") == 0){
            if(strcasecmp(i.second().c_str(),"keep_alive") == 0){

            }
            continue;
        }

        if(!has_host && strcasecmp(i.first.c_str(),"host") == 0 ){
            has_host = !i.second.empty();
        }
        req->setHeader(i.first,i.second);
    }
    if(!has_host){
        req->setHeader("Host",uri->getHost());    
    }
    req->setBody(body);
    return DoRequest(req,uri,timeout_ms);
}

HttpResult::ptr HttpConnection::DoRequest(HttpRequest::ptr req
                , Uri::ptr uri
                , uint64_t timeout_ms){
    Address::ptr addr = uri->createAddress();
    if(!addr){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::INVALID_HOST
                , nullptr,"invalid host: " + uri->getHost());
    }
    Socket::ptr sock = Socket::CreateTCP(addr);
    if(!sock){
        return std::make_shared<HttpResult>(((int)HttpConnection::Error::CONNECT_FAIL
                , nullptr,"connect fail: " + addr->getHost()));
    }
    sock->setRecvTimeout(timeout_ms);
    HttpConnect::ptr conn = std::make_shared<HttpConnection>(sock);
    int rt = conn->sendRequest(rsq);
    if(rt == 0){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::SEND_CLOSE_BY_PEER
                , nullptr, "send request closed by peer: " + addr->toString());
    }
    if(rt < 0){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::SEND_SOCKET_ERROR
                , nullptr, "send request socket error erno=: " + std::to_string(errno)
                        + " errstr=" + std::string(strerror(errno)));
    }
    auto rsp = conn->recvResponse();
    if(!rsp){
        return std::make_shared<HttpResult>((int)HttpConnection::Error::TIMEOUT
                    , nullptr, "recv response timeout" + addr->toString() 
                    + " timeout_ms" +std::to_string(timeout_ms));
    }
    return std::make_shared<HttpResult>((int)HttpResult::Error::OK,rsp,"ok");

}


}

}