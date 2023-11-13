#ifndef __SYLAR_BYTEARRAY_H__
#define __SYLAR_BYTEARRAY_H__

#include <memory>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <vector>
#include <sys/socket.h>
/*
-------------------------------20231018 read done ---------------------------------
*/

namespace sylar{
    //二进制数组，提供基础序列化和反序列化
class ByteArray{
public:
    typedef std::shared_ptr<ByteArray>ptr;
    // bytearray存储节点
    struct Node{
        //内存块字节数 s
        Node(size_t s);

        Node();
        ~Node();//析构释放内存

        char * ptr;//内存块地址指针
        Node* next;//下一个内存块指针
        size_t size;//内存块大小
    };
    //base_size 内存块大小
    ByteArray(size_t base_size = 4096);//4k
    ~ByteArray();

    //write
    void writeFint8( int8_t value);
    void writeFuint8( uint8_t value);
    void writeFint16( int16_t value);
    void writeFuint16( uint16_t value);
    void writeFint32( int32_t value);
    void writeFuint32( uint32_t value);
    void writeFint64( int64_t value);
    void writeFuint64( uint64_t value);

    void writeInt32( int32_t value);
    void writeUint32( uint32_t value);
    void writeInt64( int64_t value);
    void writeUint64( uint64_t value);
   
    void writeFloat( float value);
    void writeDouble( double value);
    //length:int16,data
    void writeStringF16(const std::string& value);
    //length:int32,data
    void writeStringF32(const std::string& value);
    //length:int64,data
    void writeStringF64(const std::string& value);
    //length:varint,data
    void writeStringVint(const std::string& value);
    //data
    void writeStringWithoutLength(const std::string& value);

    //read
    int8_t  readFint8();
    uint8_t readFuint8();

    int16_t  readFint16();
    uint16_t readFuint16();

    int32_t  readFint32();
    uint32_t readFuint32();

    int64_t  readFint64();
    uint64_t readFuint64();

    int32_t readInt32();
    uint32_t readUint32();

    int64_t readInt64();
    uint64_t readUint64();

    float readfloat();
    double readdouble();

    //length:int16, data
    std::string readStringF16();
    //length:int32, data
    std::string readStringF32();
    //length:int64, data
    std::string readStringF64();
    //length:varint, data
    std::string readStringVint();

    //内部操作
    void clear();

    void write(const void*buf,size_t size);
    void read(void * buf,size_t size);
    void read(void * buf,size_t size,size_t position) const;

    size_t getPosition() const {return m_position;}
    void setPosition(size_t v);

    bool writeToFile(const std::string& name)const;
    bool readFromFile(const std::string& name);

    size_t getBaseSize() const {return m_baseSize;}
        /**
     * @brief 返回可读取数据大小
     */
    size_t getReadSize() const {return m_size - m_position;}
 
    bool isLittleEndian() const;
    void setLittleEndian(bool val);

    std::string toString() const;
    std::string toHexString() const;

    uint64_t getReadBuffers(std::vector<iovec>& buffers,uint64_t len = ~0ull)const;
    uint64_t getReadBuffers(std::vector<iovec>& buffers,uint64_t len, uint64_t position)const;
    //增加容量
    uint64_t getWriteBuffers(std::vector<iovec>& buffers,uint64_t len);

    size_t getSize()const {return m_size;}
private:
    
    void addCapacity(size_t size);
    //获取剩余容量
    size_t getCapacity() const {return m_capacity - m_position;}
   
private:
    //内存块大小
    size_t m_baseSize;
    //当前操作位置
    size_t m_position;
    //当前总容量
    size_t m_capacity;
    //当前数据的大小
    size_t m_size;
    //字节序 默认大端
    int8_t m_endian;
    //第一个内存块指针
    Node* m_root;
    //当前操作的内存块指针
    Node* m_cur;
};
}


#endif