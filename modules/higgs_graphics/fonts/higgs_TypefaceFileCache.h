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

/** @cond */
namespace higgs
{

struct TypefaceFileAndIndex
{
    File file;
    int index{};

    auto tie() const { return std::tuple (file, index); }

    bool operator< (const TypefaceFileAndIndex& other) const { return tie() < other.tie(); }
};

class TypefaceFileCache : public DeletedAtShutdown
{
public:
    ~TypefaceFileCache() override
    {
        clearSingletonInstance();
    }

    template <typename Fn>
    Typeface::Ptr get (const TypefaceFileAndIndex& key, Fn&& getTypeface)
    {
        {
            std::scoped_lock lock{mutex};

            if (auto result = cache[key])
                return result;
        }

        if (const auto loaded = getTypeface (key))
        {
            std::scoped_lock lock{mutex};
            return cache[key] = loaded;
        }

        return nullptr;
    }

    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (TypefaceFileCache)

private:
    std::mutex mutex;
    std::map<TypefaceFileAndIndex, Typeface::Ptr> cache;
};

} // namespace higgs
/** @endcond */
