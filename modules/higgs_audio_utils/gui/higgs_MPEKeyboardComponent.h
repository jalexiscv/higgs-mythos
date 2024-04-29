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
    A component that displays an MPE-compatible keyboard, whose notes can be clicked on.

    This component will mimic a physical MPE-compatible keyboard, showing the current state
    of an MPEInstrument object. When the on-screen keys are clicked on, it will play these
    notes by calling the noteOn() and noteOff() methods of its MPEInstrument object. Moving
    the mouse will update the pitchbend and timbre dimensions of the MPEInstrument.

    @see MPEInstrument

    @tags{Audio}
*/
class HIGGS_API  MPEKeyboardComponent  : public KeyboardComponentBase,
                                        private MPEInstrument::Listener,
                                        private Timer
{
public:
    //==============================================================================
    /** Creates an MPEKeyboardComponent.

        @param instrument   the MPEInstrument that this component represents
        @param orientation  whether the keyboard is horizontal or vertical
    */
    MPEKeyboardComponent (MPEInstrument& instrument, Orientation orientation);

    /** Destructor. */
    virtual ~MPEKeyboardComponent() override;

    //==============================================================================
    /** Sets the note-on velocity, or "strike", value that will be used when triggering new notes. */
    void setVelocity (float newVelocity)                                 { velocity = jlimit (0.0f, 1.0f, newVelocity); }

    /** Sets the pressure value that will be used for new notes. */
    void setPressure (float newPressure)                                 { pressure = jlimit (0.0f, 1.0f, newPressure); }

    /** Sets the note-off velocity, or "lift", value that will be used when notes are released. */
    void setLift (float newLift)                                         { lift = jlimit (0.0f, 1.0f, newLift); }

    /** Use this to enable the mouse source pressure to be used for the initial note-on
        velocity, or "strike", value if the mouse source supports it.
    */
    void setUseMouseSourcePressureForStrike (bool usePressureForStrike)  { useMouseSourcePressureForStrike = usePressureForStrike; }

    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the keyboard.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        whiteNoteColourId         = 0x1006000,
        blackNoteColourId         = 0x1006001,
        textLabelColourId         = 0x1006002,
        noteCircleFillColourId    = 0x1006003,
        noteCircleOutlineColourId = 0x1006004
    };

    //==============================================================================
    /** @internal */
    void mouseDrag (const MouseEvent&) override;
    /** @internal */
    void mouseDown (const MouseEvent&) override;
    /** @internal */
    void mouseUp (const MouseEvent&) override;
    /** @internal */
    void focusLost (FocusChangeType) override;
    /** @internal */
    void colourChanged() override;

private:
    //==============================================================================
    struct MPENoteComponent;

    //==============================================================================
    void drawKeyboardBackground (Graphics& g, Rectangle<float> area) override;
    void drawWhiteKey (int midiNoteNumber, Graphics& g, Rectangle<float> area) override;
    void drawBlackKey (int midiNoteNumber, Graphics& g, Rectangle<float> area) override;

    void updateNoteData (MPENote&);

    void noteAdded (MPENote) override;
    void notePressureChanged (MPENote) override;
    void notePitchbendChanged (MPENote) override;
    void noteTimbreChanged (MPENote) override;
    void noteReleased (MPENote) override;
    void zoneLayoutChanged() override;

    void timerCallback() override;

    //==============================================================================
    MPEValue mousePositionToPitchbend (int, Point<float>);
    MPEValue mousePositionToTimbre (Point<float>);

    void addNewNote (MPENote);
    void removeNote (MPENote);

    void handleNoteOns  (std::set<MPENote>&);
    void handleNoteOffs (std::set<MPENote>&);
    void updateNoteComponentBounds (const MPENote&, MPENoteComponent&);
    void updateNoteComponents();

    void updateZoneLayout();

    //==============================================================================
    MPEInstrument& instrument;
    std::unique_ptr<MPEChannelAssigner> channelAssigner;

    CriticalSection activeNotesLock;
    std::vector<std::pair<MPENote, bool>> activeNotes;
    std::vector<std::unique_ptr<MPENoteComponent>> noteComponents;
    std::map<int, uint16> sourceIDMap;

    float velocity = 0.7f, pressure = 1.0f, lift = 0.0f;
    bool useMouseSourcePressureForStrike = false;
    int perNotePitchbendRange = 48;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MPEKeyboardComponent)
};

} // namespace higgs
