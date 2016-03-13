#include "utils.h"

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