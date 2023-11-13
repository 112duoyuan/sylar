#ifndef __SYLAR_SOCKET_H__
#define __SYLAR_SOCKET_H__

#include <memory>
#include "address.h"
#include "noncopyable.h"

namespace sylar{

class Socket: public std::enable_shared_from_this<Socket>, Noncopyable{
public:
    typedef std::shared_ptr<Socket>ptr;
    typedef std::weak_ptr<Socket> weak_ptr;

    enum Type{
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    /**
     * @brief Socket协议簇
     */
    enum Family{
        IPv4 = AF_INET,
        IPv6 = AF_INET6,
        UNIX = AF_UNIX
    };
    /**
     * @brief 创建TCP Socket(满足地址类型)
     * @param[in] address 地址
     */
    static Socket::ptr CreateTCP(sylar::Address::ptr address);
    static Socket::ptr CreateUDP(sylar::Address::ptr address);

    /**
     * @brief 创建UDP Socket(满足地址类型)
     * @param[in] address 地址
     */
    static Socket::ptr CreateTCPScoket();
    static Socket::ptr CreateUDPScoket();

    /**
     * @brief 创建IPv4的TCP Socket
     */
    static Socket::ptr CreateTCPScoket6();
    static Socket::ptr CreateUDPScoket6();

    static Socket::ptr CreateUnixTCPSocket();
    static Socket::ptr CreateUnixUDPSocket();

    Socket(int family, int type,int protocol = 0);
    ~Socket();

    /**
     * @brief 获取发送超时时间(毫秒)
     */
    int64_t getSendTimeout();

    void setSendTimeout(int64_t v);

    int64_t getRecvTimeout();

    void setRecvTimeout(int64_t v);

    /**
     * @brief 获取sockopt @see getsockopt
     */
    bool getOption(int level,int option,void* result,size_t * len);

    /**
     * @brief 获取sockopt模板 @see getsockopt
     */
    template<class T>
    bool getOption(int level,int option,T& result){
        size_t length = sizeof(T);
        return getOption(level,option,&result,&length);
    }

    bool setOption(int level,int option,const void * result,size_t len);
    template<class T>
    bool setOption(int level,int option,const T& value){
        return setOption(level,option,value);
    }

    /**
     * @brief 接收connect链接
     * @return 成功返回新连接的socket,失败返回nullptr
     * @pre Socket必须 bind , listen  成功
     */
    Socket::ptr accept();

    /**
     * @brief 绑定地址
     * @param[in] addr 地址
     * @return 是否绑定成功
     */
    bool  bind(const Address::ptr addr);

        /**
     * @brief 连接地址
     * @param[in] addr 目标地址
     * @param[in] timeout_ms 超时时间(毫秒)
     */
    bool connect(const Address::ptr addr,uint64_t timeout = -1);
    
        /**
     * @brief 监听socket
     * @param[in] backlog 未完成连接队列的最大长度
     * @result 返回监听是否成功
     * @pre 必须先 bind 成功
     */
    bool listen(int backlog = SOMAXCONN);

     /**
     * @brief 关闭socket
     */
    bool close();

    /**
     * @brief 发送数据
     * @param[in] buffer 待发送数据的内存
     * @param[in] length 待发送数据的长度
     * @param[in] flags 标志字
     * @return
     *      @retval >0 发送成功对应大小的数据
     *      @retval =0 socket被关闭
     *      @retval <0 socket出错
     */
    int send(const void* buffer,size_t length,int flags = 0);

        /**
     * @brief 发送数据
     * @param[in] buffers 待发送数据的内存(iovec数组)
     * @param[in] length 待发送数据的长度(iovec长度)
     * @param[in] flags 标志字
     * @return
     *      @retval >0 发送成功对应大小的数据
     *      @retval =0 socket被关闭
     *      @retval <0 socket出错
     */
    int send(const iovec* buffers,size_t length,int flags = 0);
    
        /**
     * @brief 发送数据
     * @param[in] buffer 待发送数据的内存
     * @param[in] length 待发送数据的长度
     * @param[in] to 发送的目标地址
     * @param[in] flags 标志字
     * @return
     *      @retval >0 发送成功对应大小的数据
     *      @retval =0 socket被关闭
     *      @retval <0 socket出错
     */
    int sendTo(const void* buffer,size_t length,const Address::ptr to,int flags = 0);
    int sendTo(const iovec* buffers,size_t length,const Address::ptr to,int flags = 0);

    int recv(void * buffer,size_t length,int flags= 0);
    int recv(iovec* buffers, size_t length,int flags = 0);
    int recvFrom(void* buffers, size_t length,Address::ptr from,int flags = 0);
    int recvFrom(iovec* buffers, size_t length,Address::ptr from ,int flags = 0);

        /**
     * @brief 获取远端地址
     */
    Address::ptr getRemoteAddress();
        /**
     * @brief 获取本地地址
     */
    Address::ptr getLocalAddress();

    int getFamily() const{ return m_family;}
    int getType() const {return m_type;}
    int getProtocol() const {return m_protocol;}

    bool isConnected() const {return m_isConnected;}
    bool isValid() const;

        /**
     * @brief 返回Socket错误
     */
    int getError();

    std::ostream& dump(std::ostream& os)const;
    int getSocket() const{return m_sock;}

    bool cancelRead();
    bool cancelWrite();
    bool cancelAccept();
    bool cancelAll();
private:
    void initSock();
    void newSock();
    bool init(int sock);
protected:

private:
    /// socket句柄
    int m_sock;
        /// 协议簇
    int m_family;
        /// 类型
    int m_type;
        /// 协议
    int m_protocol;
    /// 是否连接
    bool m_isConnected;

    Address::ptr m_localAddress;
    Address::ptr m_remoteAddress;    

};
/**
 * @brief 流式输出socket
 * @param[in, out] os 输出流
 * @param[in] sock Socket类
 */
std::ostream& operator<< (std::ostream& os,const Socket& addr);


}



#endif