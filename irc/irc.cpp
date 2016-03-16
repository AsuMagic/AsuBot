#include "irc.h"

const unsigned int bufferSize = 4096;

IRC::IRC() {}

od::Socket::Status IRC::connect(std::string addr, int port)
{
    this->addr = addr; this->port = static_cast<od::word>(port);
    stat = sock.connect(od::IPAddress(addr), port);
    return stat;
}

void IRC::disconnect()
{
    sock.close();
}

od::Socket::Status IRC::getStatus()
{
    return stat;
}

bool IRC::isConnected()
{
    return stat == od::Socket::Status::Done;
}

std::string IRC::receiveMessage()
{
    if (!messages.empty())
    {
        std::string str = messages.front();
        messages.pop();
        printf("[<] %s\n", str.c_str());
        return str;
    }

    bool receiving = true;
    while (receiving)
    {
        char buffer[bufferSize];
        size_t received;
        stat = sock.receive(buffer, bufferSize, received);

        if (stat != od::Socket::Status::Done)
            return "";

        unsigned int lastMatch = 0;

        for (unsigned int i = 0; i < received; ++i)
        {
            if (buffer[i] == '\n' && lastChar == '\r')
            {
                messages.push(uncompleteMessage + std::string(buffer + lastMatch, i - lastMatch));

                lastMatch = i + 1;
                uncompleteMessage.clear();
                receiving = false;
            }

            lastChar = buffer[i];
        }

        if (lastMatch != received)
        {
            uncompleteMessage += std::string(buffer + lastMatch, received - lastMatch);
        }
    }

    if (!messages.empty())
    {
        std::string str = messages.front();
        messages.pop();
        if (str.substr(0, 4) == "PING")
            send("PONG" + str.substr(4, str.size() - 3));

        printf("[<] %s\n", str.c_str());
        return str;
    }
    else
    {
        return "";
    }
}

od::Socket::Status IRC::send(std::string str)
{
    printf("[>] %s\n", str.c_str());
    str += "\r\n";
    stat = sock.send(&str[0], str.size());
    return stat;
}

od::IPAddress IRC::getRemoteAddress()
{
    return sock.remoteAddress();
}

int IRC::getRemotePort()
{
    return sock.remotePort();
}