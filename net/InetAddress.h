#ifndef NET_INETADDRESS_H
#define NET_INETADDRESS_H

#include <string>
#include <netinet/in.h>

namespace net
{

class InetAddress
{
    public:
        
        using SA = struct sockaddr;

        explicit InetAddress(uint16_t port);
        
        static const SA* sockaddr_cast(const struct sockaddr_in* addr)
        {
          return reinterpret_cast<const SA*>(addr);
        }

        static SA* sockaddr_cast(struct sockaddr_in* addr)
        {
          return reinterpret_cast<SA*>(addr);
        }

        InetAddress(const std::string& ip, uint16_t port);

        InetAddress(const struct sockaddr_in& addr): addr_(addr){ }

        std::string toHostPort() const;

        const struct sockaddr_in& getSockAddrInet() const { return addr_; }
        void setSockAddrInet(const struct sockaddr_in& addr) { addr_ = addr; }

    private:
        struct sockaddr_in addr_;
};

}

#endif  // NET_INETADDRESS_H
