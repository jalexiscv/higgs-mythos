/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

//==============================================================================
/**
    Embedding an instance of this class inside another class can be used as a low-overhead
    way of detecting leaked instances.

    This class keeps an internal static count of the number of instances that are
    active, so that when the app is shutdown and the static destructors are called,
    it can check whether there are any left-over instances that may have been leaked.

    To use it, use the HIGGS_LEAK_DETECTOR macro as a simple way to put one in your
    class declaration. Have a look through the higgs codebase for examples, it's used
    in most of the classes.

    @tags{Core}
*/
template <class OwnerClass>
class LeakedObjectDetector
{
public:
    //==============================================================================
    LeakedObjectDetector() noexcept                                 { ++(getCounter().numObjects); }
    LeakedObjectDetector (const LeakedObjectDetector&) noexcept     { ++(getCounter().numObjects); }

    LeakedObjectDetector& operator= (const LeakedObjectDetector&) noexcept = default;

    ~LeakedObjectDetector()
    {
        if (--(getCounter().numObjects) < 0)
        {
            DBG ("*** Dangling pointer deletion! Class: " << getLeakedObjectClassName());

            /** If you hit this, then you've managed to delete more instances of this class than you've
                created. That indicates that you're deleting some dangling pointers.

                Note that although this assertion will have been triggered during a destructor, it might
                not be this particular deletion that's at fault - the incorrect one may have happened
                at an earlier point in the program, and simply not been detected until now.

                Many errors like this are caused by using old-fashioned, non-RAII techniques for
                object management. If you're seeing this, it's a good idea to double-check
                that you're using std::unique_ptrs, OwnedArrays,
                ReferenceCountedObjects, etc, and avoiding manual calls to the 'delete'
                operator at all costs!

                You may also see this error if HIGGS's shutdown code is called while an object
                containing a leak detector is still alive (perhaps declared as a namespace-scope
                static).
            */
            jassertfalse;
        }
    }

private:
    //==============================================================================
    class LeakCounter
    {
    public:
        LeakCounter() = default;

        ~LeakCounter()
        {
            if (numObjects.value > 0)
            {
                DBG ("*** Leaked objects detected: " << numObjects.value << " instance(s) of class " << getLeakedObjectClassName());

                /** If you hit this, then you've leaked one or more objects of the type specified by
                    the 'OwnerClass' template parameter - the name should have been printed by the line above.

                    Many errors like this are caused by using old-fashioned, non-RAII techniques for
                    object management. If you're seeing this, it's a good idea to double-check
                    that you're using std::unique_ptrs, OwnedArrays,
                    ReferenceCountedObjects, etc, and avoiding manual calls to the 'delete'
                    operator at all costs!

                    You may also see this error if HIGGS's shutdown code is called while an object
                    containing a leak detector is still alive (perhaps declared as a namespace-scope
                    static).
                */
                jassertfalse;
            }
        }

        Atomic<int> numObjects;
    };

    static const char* getLeakedObjectClassName()
    {
        return OwnerClass::getLeakedObjectClassName();
    }

    static LeakCounter& getCounter() noexcept
    {
        static LeakCounter counter;
        return counter;
    }
};

//==============================================================================
#if DOXYGEN || ! defined (HIGGS_LEAK_DETECTOR)
 #if (DOXYGEN || HIGGS_CHECK_MEMORY_LEAKS)
  /** This macro lets you embed a leak-detecting object inside a class.

      To use it, simply declare a HIGGS_LEAK_DETECTOR (YourClassName) inside a private section
      of the class declaration. E.g.

      @code
      class MyClass
      {
      public:
          MyClass();
          void blahBlah();

      private:
          HIGGS_LEAK_DETECTOR (MyClass)
      };
      @endcode

      @see HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR, LeakedObjectDetector
  */
  #define HIGGS_LEAK_DETECTOR(OwnerClass) \
        friend class higgs::LeakedObjectDetector<OwnerClass>; \
        static const char* getLeakedObjectClassName() noexcept { return #OwnerClass; } \
        higgs::LeakedObjectDetector<OwnerClass> HIGGS_JOIN_MACRO (leakDetector, __LINE__);
 #else
  #define HIGGS_LEAK_DETECTOR(OwnerClass)
 #endif
#endif

} // namespace higgs
