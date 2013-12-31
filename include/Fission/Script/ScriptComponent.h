#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <Sqrat/sqrat.h>

#include <Fission/Core/Component.h>

class ScriptComponent : public Component
{
    public:
        ScriptComponent(HSQUIRRELVM vm = NULL);
        virtual ~ScriptComponent();

        /// \brief Execute the update function
        void executeUpdate(const float dt);

        static ComponentType Type;
        ComponentType getType() const {return Type;}

    private:
        Sqrat::Function mUpdateFunction;
};

#endif // SCRIPTCOMPONENT_H
