#include "irc.h"

const unsigned int bufferSize = 4096;

IRC::IRC(const od::IPAddress& addr, const uint16_t& port) : addr(addr), port(port) {}

od::Socket::Status IRC::connect()
{
    printf("Connecting... ");
    stat = sock.connect(addr, port);
    if (stat == od::Socket::Done)
    {
        printf("Done.\nRemote IP : %s:%u\n", sock.remoteAddress().toString().c_str(), port);
    }
    else
    {
        printf("Failed.\nCould not connect to the server.\n");
    }

    return stat;
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
        triggerEvents(str);
        return str;
    }

    bool receiving = true;
    while (receiving)
    {
        char buffer[bufferSize];
        size_t received;
        stat = sock.receive(buffer, bufferSize, received);

        //printf("Received %lu bytes long packet\n", received);

        if (stat != od::Socket::Status::Done)
            return "";

        unsigned int lastMatch = 0;

        for (unsigned int i = 0; i < received; ++i)
        {
            if (buffer[i] == '\n' && lastChar == '\r')
            {
                //printf("IRC message found from %u to %u\n", lastMatch, i - lastMatch);

                messages.push(uncompleteMessage + std::string(buffer + lastMatch, i - lastMatch));

                lastMatch = i + 1;
                uncompleteMessage.clear();
                receiving = false;
            }

            lastChar = buffer[i];
        }

        if (lastMatch != received)
        {
            //printf("Uncomplete message received from the %lu bytes packet.\n", received);
            uncompleteMessage += std::string(buffer + lastMatch, received - lastMatch);
        }
        //printf("Current message fragment : '%s'\n", uncompleteMessage.c_str());
    }

    if (!messages.empty())
    {
        std::string str = messages.front();
        messages.pop();
        printf("[<] %s\n", str.c_str());
        triggerEvents(str);
        return str;
    }
    else
    {
        return "";
    }
}

void IRC::triggerEvents(const std::string& message)
{
    for (ircEventHandler& handler : handlers)
    {
        handler(*this, message);
    }
}

void IRC::registerEventHandler(ircEventHandler handler)
{
    handlers.push_back(handler);
}

std::string IRC::getChannel(const std::string& privmsg)
{
    unsigned long hashloc = privmsg.find('#');
    if (hashloc == std::string::npos) return "";

    unsigned long nextspaceloc = privmsg.find(' ', hashloc);
    if (nextspaceloc == std::string::npos) return "";

    return privmsg.substr(hashloc, nextspaceloc - hashloc);
}

std::string IRC::getMessage(const std::string& privmsg)
{
    unsigned long msgbegin = privmsg.find(':', 1);
    if (msgbegin == std::string::npos) return "[ No message ]";

    return privmsg.substr(msgbegin + 1, privmsg.size() - msgbegin + 1);
}

std::string IRC::getUsername(const std::string& privmsg)
{
    unsigned long warnpos = privmsg.find('!');
    if (warnpos == std::string::npos) return "personne";

    return privmsg.substr(1, warnpos - 1);
}

od::Socket::Status IRC::send(std::string str)
{
    printf("[>] %s\n", str.c_str());
    str += "\r\n";
    stat = sock.send(&str[0], str.size());
    return stat;
}

od::Socket::Status IRC::privmsg(const std::string& channel, const std::string& str)
{
    return send("PRIVMSG " + channel + " " + str);
}

void autoPing(IRC& irc, const std::string& message)
{
    if (message.substr(0, 4) == "PING")
    {
        irc.send("PONG " + message.substr(5, message.size() - 5));
    }
}