/*
Theodore DeRego
Copyright 2012
*/

#ifndef REFCOUNTED_H
#define REFCOUNTED_H

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
                delete this;
        }

    private:
        int mRefs;
};

#endif // REFCOUNTED_H
