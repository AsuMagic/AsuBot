#ifndef IRCBOT_IRC_H
#define IRCBOT_IRC_H

#include <onidev/network.h>
#include <queue>
#include <string>
#include <functional>

class IRC
{
public:
    IRC();

    od::Socket::Status connect(std::string addr, int port);
    void disconnect();
    od::Socket::Status getStatus();
    bool isConnected();

    std::string receiveMessage();
    od::Socket::Status send(std::string str);

    od::IPAddress getRemoteAddress();
    int getRemotePort();

private:
    od::SocketTcp sock;
    od::Socket::Status stat;
    od::IPAddress addr;
    uint16_t port;

    // Used by the receiveMessage function, to handle uncomplete messages.
    std::queue<std::string> messages;
    std::string uncompleteMessage;
    char lastChar = 0;
};

#endif