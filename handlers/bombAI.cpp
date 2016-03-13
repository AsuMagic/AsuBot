#include "handlers.h"

bool badtime = false;

void bombAI(IRC& irc, const std::string& message)
{
    std::string str = toLower(irc.getMessage(message));

    if (str.substr(0, 16) == "!timebomb asubot")
    {
        const std::vector<std::string> blame = { "T'es sérieux là, ", "Pourquoi tu me fais ça, ", "Qu'ai-je fait pour mériter ça, " };
        const std::vector<std::string> uhoh = { "Bon, ben, euh...", "Ça va mal finir ce genre de choses...", "Beh... #yolo?", "Ça va chier des paquerettes, là!", "Allez, ça va chier!", "Quand faut y aller, faut y aller!" };
        irc.privmsg(irc.getChannel(message), blame[random(0, blame.size())] + irc.getUsername(message) + "?");
        irc.privmsg(irc.getChannel(message), uhoh[random(0, blame.size())]);
        badtime = true;
    }
    else if (irc.getUsername(message) == "Dartagnan" && badtime)
    {
        unsigned long endfils = str.find("s : ");
        if (endfils == std::string::npos)
            return;

        unsigned long tokenBegin = endfils + 4, nextToken;
        std::vector<std::string> wires;
        while(badtime) // reused variable
        {
            unsigned long tokenEnd = str.find(',', tokenBegin);
            if (tokenEnd == std::string::npos)
            {
                tokenEnd = str.find(" et", tokenBegin);
                if (tokenEnd == std::string::npos) // line end
                {
                    tokenEnd = str.size() - 2; // size off by one + dot ignore
                    badtime = false;
                }

                nextToken = tokenEnd + 4;
            }
            else
            {
                nextToken = tokenEnd + 2;
            }

            wires.push_back(str.substr(tokenBegin, tokenEnd - tokenBegin));

            tokenBegin = nextToken;
        }

        irc.privmsg(irc.getChannel(message), "!coupe " + wires[random(0, wires.size())]);
    }
}