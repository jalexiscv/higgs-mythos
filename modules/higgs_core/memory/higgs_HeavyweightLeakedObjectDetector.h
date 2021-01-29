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
    This class is a useful way of tracking down hard to find memory leaks when the
    regular LeakedObjectDetector isn't enough.

    As well as firing when any instances of the OwnerClass type are leaked, it will
    print out a stack trace showing where the leaked object was created. This is obviously
    quite a heavyweight task so, unlike the LeakedObjectDetector which should be always
    be added to your classes, you should only use this object temporarily when you are
    debugging and remove it when finished.

    To use it, use the HIGGS_HEAVYWEIGHT_LEAK_DETECTOR macro as a simple way to put
    one in your class declaration.

    @tags{Core}
*/
template <class OwnerClass>
class HeavyweightLeakedObjectDetector
{
public:
    //==============================================================================
    HeavyweightLeakedObjectDetector() noexcept                                           { getBacktraceMap()[this] = SystemStats::getStackBacktrace(); }
    HeavyweightLeakedObjectDetector (const HeavyweightLeakedObjectDetector&) noexcept    { getBacktraceMap()[this] = SystemStats::getStackBacktrace(); }

    ~HeavyweightLeakedObjectDetector()                                                   { getBacktraceMap().erase (this); }

private:
    //==============================================================================
    typedef std::map<HeavyweightLeakedObjectDetector<OwnerClass>*, String> BacktraceMap;

    //==============================================================================
    struct BacktraceMapHolder
    {
        BacktraceMapHolder() = default;

        ~BacktraceMapHolder()
        {
            if (map.size() > 0)
            {
                DBG ("*** Leaked objects detected: " << map.size() << " instance(s) of class " << getLeakedObjectClassName());
                DBG (getFormattedBacktracesString());

                /** If you hit this, then you've leaked one or more objects of the type specified by
                    the 'OwnerClass' template parameter - the name and stack trace of its creation should
                    have been printed by the lines above.

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

        String getFormattedBacktracesString() const
        {
            String str;

            int counter = 1;
            for (auto& bt : map)
            {
                str << "\nBacktrace " << String (counter++)                                << "\n"
                    << "-----------------------------------------------------------------" << "\n"
                    << bt.second;
            }

            return str;
        }

        BacktraceMap map;
    };

    static BacktraceMap& getBacktraceMap()
    {
        static BacktraceMapHolder holder;
        return holder.map;
    }

    static const char* getLeakedObjectClassName()
    {
        return OwnerClass::getLeakedObjectClassName();
    }
};

//==============================================================================
#if DOXYGEN || ! defined (HIGGS_HEAVYWEIGHT_LEAK_DETECTOR)
 #if (DOXYGEN || HIGGS_CHECK_MEMORY_LEAKS)
  /** This macro lets you embed a heavyweight leak-detecting object inside a class.

      To use it, simply declare a HIGGS_HEAVYWEIGHT_LEAK_DETECTOR (YourClassName) inside a private section
      of the class declaration. E.g.

      @code
      class MyClass
      {
      public:
          MyClass();
          void blahBlah();

      private:
          HIGGS_HEAVYWEIGHT_LEAK_DETECTOR (MyClass)
      };
      @endcode

      NB: you should only use this when you really need to track down a tricky memory leak, and
      should never leave one of these inside a class!

      @see HeavyweightLeakedObjectDetector, HIGGS_LEAK_DETECTOR, LeakedObjectDetector
  */
  #define HIGGS_HEAVYWEIGHT_LEAK_DETECTOR(OwnerClass) \
        friend class higgs::HeavyweightLeakedObjectDetector<OwnerClass>; \
        static const char* getLeakedObjectClassName() noexcept { return #OwnerClass; } \
        higgs::HeavyweightLeakedObjectDetector<OwnerClass> HIGGS_JOIN_MACRO (leakDetector, __LINE__);
 #else
  #define HIGGS_HEAVYWEIGHT_LEAK_DETECTOR(OwnerClass)
 #endif
#endif

} // namespace higgs
