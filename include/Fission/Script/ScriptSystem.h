#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <Sqrat/sqrat.h>

#include <Fission/Core/System.h>

class Engine;

void bindSquirrel(HSQUIRRELVM vm, Engine *_engine);

class ScriptSystem : public System
{
    public:
        ScriptSystem(EventManager *eventManager, Engine *engine);
        virtual ~ScriptSystem();

        /// \brief Creates a new script
        HSQUIRRELVM createScript(const std::string& fileName);

        /// \brief Adds a new binder function
        void addBinder(void (*binder)(HSQUIRRELVM)){mBinders.push_back(binder);binder(mVM);}

        // Accessors

        /// \brief Get the Squirrel VM
        HSQUIRRELVM getVM(){return mVM;}

    protected:
        void processEntity(Entity *entity, const float dt);

    private:
        HSQUIRRELVM mVM;

        Engine *mEngine;

        std::vector<void (*)(HSQUIRRELVM)> mBinders; // Binding functions
};

#endif // SCRIPTSYSTEM_H
