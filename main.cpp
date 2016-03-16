#include <cstdlib>
#include <onidev/network.h>
#include <random>

#include "irc/irc.h"
#include "scripting.h"

int main()
{
    IrcScripting().runScripting();
}