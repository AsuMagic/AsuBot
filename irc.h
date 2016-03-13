#ifndef IRCBOT_IRC_H
#define IRCBOT_IRC_H

#include <onidev/network.h>
#include <queue>
#include <string>
#include <functional>

class IRC;

typedef std::function<void(IRC&, const std::string&)> ircEventHandler;

class IRC
{
public:
    IRC(const od::IPAddress& addr, const uint16_t& port);

    od::Socket::Status connect();
    od::Socket::Status getStatus();
    bool isConnected();

    std::string receiveMessage();
    od::Socket::Status send(std::string str);

    void triggerEvents(const std::string& message);
    void registerEventHandler(ircEventHandler handler);

private:
    od::SocketTcp sock;
    od::Socket::Status stat;
    od::IPAddress addr;
    uint16_t port;

    // Used by the receiveMessage function, to handle uncomplete messages.
    std::queue<std::string> messages;
    std::vector<ircEventHandler> handlers;
    std::string uncompleteMessage;
    char lastChar = 0;
};

void autoPing(IRC& irc, const std::string& message);

#endif