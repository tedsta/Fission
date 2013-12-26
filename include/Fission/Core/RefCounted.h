/*
Theodore DeRego
Copyright 2012
*/

#ifndef REFCOUNTED_H
#define REFCOUNTED_H

#include <iostream>
#include <Sqrat/sqrat.h>

/// \brief The abstract base class for all objects with reference counting.
class RefCounted
{
    public:
        RefCounted() : mRefs(1) {}
        virtual ~RefCounted(){}

        /// \brief Increment the reference counter.
        void addRef(){mRefs++;}

        /// \brief Decrement the reference counter and delete this object if the count reaches 0.
        void release()
        {
            mRefs--;
            if (mRefs == 0)
            {
                if (mSqInst.IsNull())
                    delete this;
                else
                {
                    HSQUIRRELVM vm = mSqInst.GetVM();
                    HSQOBJECT hobj = mSqInst.GetObject();
                    mSqInst.SetShouldRelease(false);
                    sq_release(vm, &hobj);
                }
            }
        }

        /// \brief Set the squirrel instance.
        void setSqInst(Sqrat::Object inst){mSqInst=inst;}

        /// \brief Get the squirrel instance, will be NULL this wasn't instantiated from a script
        Sqrat::Object getSqInst(){return mSqInst;}

    private:
        int mRefs;

        // The squirrel instance, if there is one. Useful for instantiating things
        // in squirrel and preventing them from being garbage collected
        Sqrat::Object mSqInst;
};

#endif // REFCOUNTED_H
