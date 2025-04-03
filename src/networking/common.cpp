#include "common.hpp"
#include <sys/socket.h>
#include <vector>

std::string readMessage(int sckt) {
    uint32_t len_net;
    ssize_t received = recv(sckt, &len_net, sizeof(len_net), MSG_WAITALL);
    if (received != sizeof(len_net)) {
        return "";
    }

    uint32_t len = ntohl(len_net);

    std::vector<char> buffer(len);

    received = recv(sckt, buffer.data(), len, MSG_WAITALL);
    if (received != static_cast<ssize_t>(len)) return "";
    
    return std::string(buffer.begin(), buffer.end());
}

bool sendMessage(int sckt, const std::string& message) {
    uint32_t len = message.size();
    uint32_t len_net = htonl(len);
    if ( send(sckt, &len_net, sizeof(len_net), 0) != sizeof(len_net) ) return false;
    return send(sckt, message.c_str(), len, 0) == static_cast<ssize_t>(len);
}
