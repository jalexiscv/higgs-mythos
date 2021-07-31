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
    A drawable object which renders a filled or outlined shape.

    For details on how to change the fill and stroke, see the DrawableShape class.

    @see Drawable, DrawableShape

    @tags{GUI}
*/
class HIGGS_API  DrawablePath  : public DrawableShape
{
public:
    //==============================================================================
    /** Creates a DrawablePath. */
    DrawablePath() = default;
    DrawablePath (const DrawablePath&) = default;

    //==============================================================================
    /** Changes the path that will be drawn.
        @see setFill, setStrokeType
    */
    void setPath (const Path& newPath);

    /** Changes the path that will be drawn.
        @see setFill, setStrokeType
    */
    void setPath (Path&& newPath);

    /** Returns the current path. */
    const Path& getPath() const;

    /** Returns the current path for the outline. */
    const Path& getStrokePath() const;

    //==============================================================================
    /** @internal */
    std::unique_ptr<Drawable> createCopy() const override;

private:
    //==============================================================================
    HIGGS_LEAK_DETECTOR (DrawablePath)
};

} // namespace higgs
