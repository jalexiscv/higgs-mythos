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

class SharedResourcePointerTest final : public UnitTest
{
public:
    SharedResourcePointerTest()
        : UnitTest ("SharedResourcePointer", UnitTestCategories::memory) {}

    void runTest() final
    {
        beginTest ("Only one instance is created");
        {
            static int count = 0;
            struct CountIncrementer { CountIncrementer() { ++count; } };
            expect (count == 0);

            const SharedResourcePointer<CountIncrementer> instance1;
            expect (count == 1);

            const SharedResourcePointer<CountIncrementer> instance2;
            expect (count == 1);

            expect (&instance1.get() == &instance2.get());
        }

        beginTest ("The shared object is destroyed when the reference count reaches 0");
        {
            static int count = 0;
            struct ReferenceCounter
            {
                ReferenceCounter() { ++count; }
                ~ReferenceCounter() { --count; }
            };

            expect (count == 0);

            {
                const SharedResourcePointer<ReferenceCounter> instance1;
                const SharedResourcePointer<ReferenceCounter> instance2;
                expect (count == 1);
            }

            expect (count == 0);
        }

        beginTest ("getInstanceWithoutCreating()");
        {
            struct Object{};

            expect (SharedResourcePointer<Object>::getSharedObjectWithoutCreating() == std::nullopt);

            {
                const SharedResourcePointer<Object> instance;
                expect (&SharedResourcePointer<Object>::getSharedObjectWithoutCreating()->get() == &instance.get());
            }

            expect (SharedResourcePointer<Object>::getSharedObjectWithoutCreating() == std::nullopt);
        }

        beginTest ("Create objects with private constructors");
        {
            class ObjectWithPrivateConstructor
            {
            private:
                ObjectWithPrivateConstructor() = default;
                friend SharedResourcePointer<ObjectWithPrivateConstructor>;
            };

            SharedResourcePointer<ObjectWithPrivateConstructor> instance;
        }
    }
};

static SharedResourcePointerTest sharedResourcePointerTest;

} // namespace higgs
