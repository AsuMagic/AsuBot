#ifndef IRCBOT_HELLO_H
#define IRCBOT_HELLO_H

#include <string>

#include "../irc/irc.h"
#include "../irc/utils.h"

const std::vector<std::string> botnamematches = { "asu", "asubot", "bobot", "le bot" };

void helloAI(IRC& irc, const std::string& message);
void ntmAI(IRC& irc, const std::string& message);
void bombAI(IRC& irc, const std::string& message);

#endif