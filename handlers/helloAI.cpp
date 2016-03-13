#include "handlers.h"

#include <iostream>

void helloAI(IRC& irc, const std::string& message)
{
    const std::vector<std::string> hellomatches = { "salut", "hello", "mille", "mill", "lut", "plop", "cc", "bonjour", "bjr", "hey", "coucou" };

    std::string str = toLower(irc.getMessage(message));

    if (!hasMatches(str, hellomatches) || !hasMatches(str, botnamematches))
        return;

    const std::vector<std::string> answers = { "Yop!", "Ça gaze?", "Salut!", "lut!", "Hey! Tu as besoin d'aide?", "hey!", "mille mille!", "Salut, ça va?", "Bonzour!", "Bonjourno", "o/" };
    irc.privmsg(irc.getChannel(message), answers[random(0, answers.size())]);
}