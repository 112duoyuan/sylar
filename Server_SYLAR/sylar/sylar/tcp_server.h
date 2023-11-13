#ifndef __SYLAR_TCP_SERVER_H__
#define __SYLAR_TCP_SERVER_H__

#include <memory>
#include <functional>
#include "iomanager.h"
#include "socket.h"
#include "address.h"
#include "noncopyable.h"

namespace sylar{

class TcpServer : public std::enable_shared_from_this<TcpServer>
            ,Noncopyable{
public:
    typedef std::shared_ptr<TcpServer> ptr;

    /**
     * @brief 构造函数
     * @param[in] worker socket客户端工作的协程调度器
     * @param[in] accept_worker 服务器socket执行接收socket连接的协程调度器
     */
    TcpServer(sylar::IOManager * worker = sylar::IOManager::GetThis(),
                sylar::IOManager * accept_worker = sylar::IOManager::GetThis());
    virtual ~TcpServer();

    /**
     * @brief 绑定地址
     * @return 返回是否绑定成功
     */
    virtual bool bind(sylar::Address::ptr addr);

        /**
     * @brief 绑定地址数组
     * @param[in] addrs 需要绑定的地址数组
     * @param[out] fails 绑定失败的地址
     * @return 是否绑定成功
     */
    virtual bool bind(const std::vector<Address::ptr>& addrs
        ,std::vector<Address::ptr>& fails);
    
    virtual bool start();
    virtual void stop();

    uint64_t getRecvTimeout() const {return m_recvTimeout;}
    std::string getName() const {return m_name;}
    void setRecvTimeout(uint64_t v){m_recvTimeout = v;}
    void setName(const std::string& v){m_name = v;}
    bool isStop()const {return m_isStop;}
protected:

    /**
     * @brief 处理新连接的Socket类
     */
    virtual void handleClient(Socket::ptr client);
    /**
     * @brief 开始接受连接
     */
    virtual void startAccept(Socket::ptr sock);
private:
        /// 监听Socket数组
    std::vector<Socket::ptr>m_socks;
    /// 新连接的Socket工作的调度器
    IOManager* m_worker;

    /// 服务器Socket接收连接的调度器
    IOManager* m_acceptWorker;
    /// 接收超时时间(毫秒)
    uint64_t m_recvTimeout;
    /// 服务器名称
    std::string m_name;
    /// 服务是否停止
    bool m_isStop;
};
}


#endif