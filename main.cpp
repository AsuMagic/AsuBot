#include <cstdlib>
#include <onidev/network.h>

#include "irc.h"

int main()
{
    IRC irc(od::IPAddress("irc.mibbit.net"), 6667);
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

    while (irc.isConnected())
    {
        std::string str = irc.receiveMessage();
    }

    printf("Connection closed.");
}