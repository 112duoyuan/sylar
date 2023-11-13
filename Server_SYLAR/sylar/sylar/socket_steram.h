#ifndef __SYLAR_SOCKET_STREAM_H__
#define __SYLAR_SOCKET_STREAM_H__

#include "stream.h"
#include "socket.h"

namespace sylar{
class SocketStream : public Stream{
public:

        /**
     * @brief 构造函数
     * @param[in] sock Socket类
     * @param[in] owner 是否完全控制
     */
    SocketStream(Socket::ptr sock,bool owner =true);
    ~SocketStream();
    typedef std::shared_ptr<SocketStream>ptr;

        /**
     * @brief 读取数据
     * @param[out] buffer 待接收数据的内存
     * @param[in] length 待接收数据的内存长度
     * @return
     *      @retval >0 返回实际接收到的数据长度
     *      @retval =0 socket被远端关闭
     *      @retval <0 socket错误
     */
    virtual int read(void* buffer,size_t length) override;

    virtual int read(ByteArray::ptr ba,size_t length) override;
    
    virtual int write(const void * buffer,size_t length)override;
    virtual int write(ByteArray::ptr ba,size_t length) override;
    virtual void close()override;

    Socket::ptr getSocket() const {return m_socket;}
    bool isConnected() const;
protected:

    Socket::ptr m_socket;
    /// 是否主控
    bool m_owner;

};
}




#endif