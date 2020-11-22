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
    Implements some basic array storage allocation functions.

    This class isn't really for public use - it used to be part of the
    container classes but has since been superseded by ArrayBase. Eventually
    it will be removed from the API.

    @tags{Core}
*/
template <class ElementType, class TypeOfCriticalSectionToUse>
class ArrayAllocationBase  : public TypeOfCriticalSectionToUse
{
public:
    //==============================================================================
    /** Creates an empty array. */
    ArrayAllocationBase() = default;

    /** Destructor. */
    ~ArrayAllocationBase() = default;

    ArrayAllocationBase (ArrayAllocationBase&& other) noexcept
        : elements (std::move (other.elements)),
          numAllocated (other.numAllocated)
    {
    }

    ArrayAllocationBase& operator= (ArrayAllocationBase&& other) noexcept
    {
        elements = std::move (other.elements);
        numAllocated = other.numAllocated;
        return *this;
    }

    //==============================================================================
    /** Changes the amount of storage allocated.

        This will retain any data currently held in the array, and either add or
        remove extra space at the end.

        @param numElements  the number of elements that are needed
    */
    void setAllocatedSize (int numElements)
    {
        if (numAllocated != numElements)
        {
            if (numElements > 0)
                elements.realloc ((size_t) numElements);
            else
                elements.free();

            numAllocated = numElements;
        }
    }

    /** Increases the amount of storage allocated if it is less than a given amount.

        This will retain any data currently held in the array, but will add
        extra space at the end to make sure there it's at least as big as the size
        passed in. If it's already bigger, no action is taken.

        @param minNumElements  the minimum number of elements that are needed
    */
    void ensureAllocatedSize (int minNumElements)
    {
        if (minNumElements > numAllocated)
            setAllocatedSize ((minNumElements + minNumElements / 2 + 8) & ~7);

        jassert (numAllocated <= 0 || elements != nullptr);
    }

    /** Minimises the amount of storage allocated so that it's no more than
        the given number of elements.
    */
    void shrinkToNoMoreThan (int maxNumElements)
    {
        if (maxNumElements < numAllocated)
            setAllocatedSize (maxNumElements);
    }

    /** Swap the contents of two objects. */
    void swapWith (ArrayAllocationBase& other) noexcept
    {
        elements.swapWith (other.elements);
        std::swap (numAllocated, other.numAllocated);
    }

    //==============================================================================
    HeapBlock<ElementType> elements;
    int numAllocated = 0;

private:
    HIGGS_DECLARE_NON_COPYABLE (ArrayAllocationBase)
};

} // namespace higgs
