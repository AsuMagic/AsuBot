#include "handlers.h"

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