#include <cstdlib>
#include <onidev/network.h>
#include <random>

#include "irc.h"

std::random_device rd;
std::mt19937 gen(rd());

std::string toLower(std::string str)
{
    for (auto& i : str)
    {
        i = std::tolower(i);
    }
    return str;
}

bool hasMatches(const std::string& message, const std::vector<std::string>& toMatch)
{
    for (auto& i : toMatch)
    {
        if (message.find(i) != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

int random(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max - 1);
    return dis(gen);
}

const std::vector<std::string> botnamematches = { "asu", "asubot", "bobot", "le bot" };

void helloAI(IRC& irc, const std::string& message)
{
    const std::vector<std::string> hellomatches = { "salut", "hello", "mille", "mill", "lut", "plop", "cc", "bonjour", "bjr", "hey", "coucou" };

    std::string str = toLower(irc.getMessage(message));

    if (!hasMatches(str, hellomatches) || !hasMatches(str, botnamematches))
        return;

    const std::vector<std::string> answers = { "Salut!", "lut!", "Hey! Tu as besoin d'aide?", "hey!", "mille mille!", "Salut, ça va?", "Bonzour!", "Bonjourno", "o/" };
    irc.privmsg(irc.getChannel(message), answers[random(0, answers.size())]);
}

void ntmAI(IRC& irc, const std::string& message)
{
    const std::vector<std::string> ntmmatches = { "nique ta", "ntm", "tamer", "ta mère", "tamere", "tamère" };

    std::string str = toLower(irc.getMessage(message));

    if (!hasMatches(str, ntmmatches))
        return;

    std::vector<std::string> answers;
    if (hasMatches(str, botnamematches))
        answers = { "Et la tienne elle est mieux d'abord?", "Faudrait déjà que j'en aie une, du con.", "J'ai pas de mère, mon papa adoré c'est AsuMagic, alors tais-toi!", "Tu me parles pas comme ça!", "Oh! Tu te tais déjà! Tu sais pas où j'ai vécu moi!" };
    else
        answers = { "T'es pas obligé de lui parler comme ça...", "Il t'a fait quoi?", "Ouais, il t'a rien fait hein", "Et la tienne elle est mieux d'abord?" };

    irc.privmsg(irc.getChannel(message), answers[random(0, answers.size())]);
}

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

    irc.registerEventHandler(helloAI);
    irc.registerEventHandler(ntmAI);

    while (irc.isConnected())
    {
        std::string str = irc.receiveMessage();
    }

    printf("Connection closed.");
}