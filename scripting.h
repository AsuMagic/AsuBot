#ifndef IRCBOT_SCRIPTING_H
#define IRCBOT_SCRIPTING_H

#include "irc/irc.h"

#include <angelscript.h>

const char* getTypePrefix(asEMsgType type);

void addressConstructor(void* memory, std::string name);

class IrcScripting
{
public:
    IrcScripting();

    static void asMessageCallback(const asSMessageInfo* msg);
    static void print(const std::string& text);

    template<typename T>
    static void constructObject(void* memory)
    {
        new(memory) T();
    }

    template<typename T>
    static void destructObject(void* memory)
    {
        static_cast<T*>(memory)->~T();
    }

    static void rebuildf();
    void rebuild();

    template<typename T>
    void registerAsObject(asIScriptEngine* engine, const char* name)
    {
        engine->RegisterObjectType(name, sizeof(T), asOBJ_VALUE);
        engine->RegisterObjectBehaviour(name, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructObject<T>), asCALL_CDECL_OBJLAST);
        engine->RegisterObjectBehaviour(name, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructObject<T>), asCALL_CDECL_OBJLAST);
    }

    template<typename T>
    void registerMethod(const char* className, const char* asDeclaration, const asSFuncPtr method, const asDWORD& callConvention = asCALL_THISCALL)
    {
        engine->RegisterObjectMethod(className, asDeclaration, method, callConvention);
    }

    void registerFunction(const char* asDeclaration, const asSFuncPtr function, const asDWORD& callConvention = asCALL_CDECL);
    void registerProperty(const char* property, void* ptr);

    void runScripting();

private:
    asIScriptEngine* engine;
    asIScriptContext* context;

    bool rebuilding = false;

    void registerScripting();
};

#endif
