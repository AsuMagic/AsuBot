#include <cstdlib>
#include <onidev/network.h>
#include <random>

#include "irc/irc.h"
#include "handlers/handlers.h"

int main()
{
    IRC irc(od::IPAddress("irc.klat00.org"), 6667);
    if (irc.connect() != od::Socket::Status::Done)
        return EXIT_FAILURE;

    irc.registerEventHandler(autoPing);

    irc.receiveMessage(); // Skip the first message
    if (!irc.isConnected())
    {
        printf("Connection aborted before logging...\n");
        return EXIT_FAILURE;
    }

    irc.send("NICK AsuBot");
    irc.send("USER AsuBot AsuBot AsuBot AsuBot");

    bool joinedChannels = false;
    while (irc.isConnected() && !joinedChannels)
    {
        std::string str = irc.receiveMessage();
        joinedChannels = str.find("MODE") != std::string::npos;
    }

    std::vector<std::string> channels = { "himself" };

    for (auto& i : channels)
    {
        irc.send("JOIN #" + i);
        irc.send("PRIVMSG #" + i + " Bonjour! Je suis un bot IRC.");
    }

    for (ircEventHandler i : { helloAI, ntmAI, bombAI })
        irc.registerEventHandler(i);

    while (irc.isConnected())
    {
        std::string str = irc.receiveMessage();
    }

    printf("Connection closed.");
}