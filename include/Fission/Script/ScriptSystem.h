#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <Sqrat/sqrat.h>

#include <Fission/Core/System.h>

namespace fsn
{
    class Engine;
    class Component;

    /// \brief a little template function to help you cast components in squirrel
    template<typename T>
    T* componentCast(Component* data){return reinterpret_cast<T*>(data);}

    class ScriptSystem : public System
    {
        public:
            ScriptSystem(IEventManager* eventManager, float lockStep, Engine *engine);
            virtual ~ScriptSystem();

            /// \brief Creates a new script
            HSQUIRRELVM createScript(const std::string& fileName);

            /// \brief Adds a new binder function
            void addBinder(void (*binder)(HSQUIRRELVM)){mBinders.push_back(binder);binder(mVM);}

            // Accessors

            /// \brief Get the Squirrel VM
            HSQUIRRELVM getVM(){return mVM;}

        protected:
            void processEntity(EntityRef* entity, const float dt);

        private:
            HSQUIRRELVM mVM;

            Engine *mEngine;

            std::vector<void (*)(HSQUIRRELVM)> mBinders; // Binding functions
    };
}


#endif // SCRIPTSYSTEM_H
