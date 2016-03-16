#include "scripting.h"
#include "irc/irc.h"

#include <angelscript.h>
#include <add_on/scriptstdstring/scriptstdstring.h>
#include <add_on/scriptbuilder/scriptbuilder.h>
#include <cstdio>

IrcScripting* scripter;

namespace prefixes
{
    const char *info = "[i]",
            *warn = "[!]",
            *err  = "[X]",
            *def  = "[*]";
}

const char* getTypePrefix(asEMsgType type)
{
    switch(type)
    {
        case asMSGTYPE_INFORMATION:
            return prefixes::info;
        case asMSGTYPE_WARNING:
            return prefixes::warn;
        case asMSGTYPE_ERROR:
            return prefixes::err;
        default:
            return prefixes::def;
    }
}

void addressConstructor(void* memory, std::string name)
{
    new(memory) od::IPAddress(name);
}

IrcScripting::IrcScripting()
{
    scripter = this;
}

void IrcScripting::asMessageCallback(const asSMessageInfo* msg)
{
    const char* type = getTypePrefix(msg->type);
    if (strlen(msg->section) == 0)
    {
        printf("%s %s\n", type, msg->message);
    }
    else
    {
        printf("%s At line (%d, %d) in %s : %s\n", type, msg->row, msg->col, msg->section, msg->message);
    }
}

void IrcScripting::print(const std::string& text) { printf("%s \n", text.c_str()); }

void IrcScripting::rebuildf()
{
    scripter->rebuild();
}

void IrcScripting::rebuild()
{
    rebuilding = true;
    context->Abort();
}

void IrcScripting::registerFunction(const char* asDeclaration, const asSFuncPtr function, const asDWORD& callConvention)
{
    engine->RegisterGlobalFunction(asDeclaration, function, callConvention);
}

void IrcScripting::registerProperty(const char* property, void* ptr)
{
    engine->RegisterGlobalProperty(property, ptr);
}

void IrcScripting::registerScripting()
{
    // Register the IPAddress class
    const char ipaddrType[] = "IPAddress";
    typedef od::IPAddress addr;
    registerAsObject<addr>(engine, ipaddrType);
    registerMethod<addr>(ipaddrType, "string toString()", asMETHOD(addr, toString));
    registerMethod<addr>(ipaddrType, "int toInteger()", asMETHOD(addr, toInteger));
    registerMethod<addr>(ipaddrType, "bool valid()", asMETHOD(addr, valid));
    registerMethod<addr>(ipaddrType, "bool opEquals(IPAddress &in ip) const", asMETHODPR(addr, operator==, (const addr&) const, bool));

    // Register the IRC class
    const char* ircType = "IRCClient";
    registerAsObject<IRC>(engine, ircType);
    registerMethod<IRC>(ircType, "int connect(string &in ip, int port)", asMETHOD(IRC, connect));
    registerMethod<IRC>(ircType, "int getStatus()", asMETHOD(IRC, getStatus));
    registerMethod<IRC>(ircType, "bool isConnected()", asMETHOD(IRC, isConnected));
    registerMethod<IRC>(ircType, "void disconnect()", asMETHOD(IRC, disconnect));

    registerMethod<IRC>(ircType, "IPAddress& getRemoteAddress()", asMETHOD(IRC, getRemoteAddress));
    registerMethod<IRC>(ircType, "int getRemotePort()", asMETHOD(IRC, getRemotePort));

    registerMethod<IRC>(ircType, "string receiveMessage()", asMETHOD(IRC, receiveMessage));
    registerMethod<IRC>(ircType, "int send(string str)", asMETHOD(IRC, send));

    // Register other functions
    registerFunction("void print(string &in str)", asFUNCTION(print));
    registerFunction("void rebuild()", asFUNCTION(rebuildf));
}

void IrcScripting::runScripting()
{
    printf("%s Loading main.as...\n", prefixes::info);

    engine = asCreateScriptEngine();
    engine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL);

    RegisterStdString(engine);
    registerScripting();

    do
    {
        CScriptBuilder builder;
        builder.StartNewModule(engine, "main");
        builder.AddSectionFromFile("scripts/main.as");
        builder.BuildModule();

        asIScriptModule* module = engine->GetModule("main");
        const char* entryName = (!rebuilding ? "void onInit()" : "void onRebuild()");
        asIScriptFunction* entry = module->GetFunctionByDecl(entryName);
        if (!entry)
        {
            printf("%s main.as entry point '%s' not found!\n", prefixes::err, entryName);
            return;
        }

        context = engine->CreateContext();
        context->Prepare(entry);
        int retcode = context->Execute();
        if (!rebuilding)
        {
            if (retcode != asEXECUTION_FINISHED)
            {
                printf("%s Script execution aborted!\n", prefixes::def);
                if (retcode == asEXECUTION_EXCEPTION)
                {
                    printf("%s Exception '%s' occurred. Exiting.\n", prefixes::err, context->GetExceptionString());
                }
            }
            else
            {
                printf("%s Script ended properly.\n", prefixes::info);
            }
        }

        context->Release();
    } while (rebuilding);

    engine->ShutDownAndRelease();
}