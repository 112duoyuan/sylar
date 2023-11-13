#ifndef __SYLAR_STREAM_H__
#define __SYLAR_STREAM_H__

#include <memory>
#include "bytearray.h"

namespace sylar{

class Stream{
public:
    typedef std::shared_ptr<Stream> ptr;
    virtual ~Stream(){}

    virtual int read(void* buffer,size_t length) =0;
    virtual int read(ByteArray::ptr ba,size_t length) = 0;

    /**
     * @brief 读固定长度的数据
     * @param[out] buffer 接收数据的内存
     * @param[in] length 接收数据的内存大小
     * @return
     *      @retval >0 返回接收到的数据的实际大小
     *      @retval =0 被关闭
     *      @retval <0 出现流错误
     */
    virtual int readFixSize(void* buffer,size_t length);
    virtual int readFixSize(ByteArray::ptr ba,size_t length);
 

    virtual int write(const void * buffer,size_t length) =0;
    virtual int write(ByteArray::ptr ba,size_t length) = 0;
    virtual int writeFixSize(const void * buffer,size_t length);
    virtual int writeFixSize(ByteArray::ptr ba,size_t length);
    virtual void close() = 0;
};
}




#endif