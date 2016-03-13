#ifndef IRCBOT_UTILS_H
#define IRCBOT_UTILS_H

#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

std::string toLower(std::string str);

bool hasMatches(const std::string& message, const std::vector<std::string>& toMatch);

int random(int min, int max);

#endif
