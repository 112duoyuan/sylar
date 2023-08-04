#ifndef __sylar_http_parser_h__
#define __sylar_http_parser_h__
#include "http11_parser.h"
#include "httpclient_parser.h"
#include "http.h"

namespace sylar{
namespace http{
class HttpRequestParser{
public:
    typedef std::shared_ptr<HttpRequestParser>ptr;
    HttpRequestParser();

    size_t execute(const char * data,size_t len,size_t off);
    int isFinished() const;
    int hasError() const;

    HttpRequest::ptr getData()const {return m_data;}
private:
    http_parser m_parser;
    HttpRequest::ptr m_data;
    int m_error;
};
class HttpResponseParser{
    typedef std::shared_ptr<HttpResponseParser>ptr;
    HttpResponseParser();
    size_t execute(const char * data,size_t len,size_t off);
    int isFinished() const;
    int hasError() const;
    HttpResponse::ptr getData()const {return m_data;}
private:
    httpclient_parser m_parser;
    HttpResponse::ptr m_data;
    int m_error;

};
}

}

#endif