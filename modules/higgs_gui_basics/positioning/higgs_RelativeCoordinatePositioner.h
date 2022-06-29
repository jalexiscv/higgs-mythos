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
    Base class for Component::Positioners that are based upon relative coordinates.

    @tags{GUI}
*/
class HIGGS_API  RelativeCoordinatePositionerBase  : public Component::Positioner,
                                                    public ComponentListener,
                                                    public MarkerList::Listener
{
public:
    RelativeCoordinatePositionerBase (Component&);
    ~RelativeCoordinatePositionerBase() override;

    void componentMovedOrResized (Component&, bool, bool) override;
    void componentParentHierarchyChanged (Component&) override;
    void componentChildrenChanged (Component&) override;
    void componentBeingDeleted (Component&) override;
    void markersChanged (MarkerList*) override;
    void markerListBeingDeleted (MarkerList*) override;

    void apply();

    bool addCoordinate (const RelativeCoordinate&);
    bool addPoint (const RelativePoint&);

    //==============================================================================
    /** Used for resolving a RelativeCoordinate expression in the context of a component. */
    class ComponentScope  : public Expression::Scope
    {
    public:
        ComponentScope (Component&);

        Expression getSymbolValue (const String& symbol) const override;
        void visitRelativeScope (const String& scopeName, Visitor&) const override;
        String getScopeUID() const override;

    protected:
        Component& component;

        Component* findSiblingComponent (const String& componentID) const;
    };

protected:
    virtual bool registerCoordinates() = 0;
    virtual void applyToComponentBounds() = 0;

private:
    class DependencyFinderScope;
    friend class DependencyFinderScope;
    Array<Component*> sourceComponents;
    Array<MarkerList*> sourceMarkerLists;
    bool registeredOk;

    void registerComponentListener (Component&);
    void registerMarkerListListener (MarkerList*);
    void unregisterListeners();

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RelativeCoordinatePositionerBase)
};

} // namespace higgs
