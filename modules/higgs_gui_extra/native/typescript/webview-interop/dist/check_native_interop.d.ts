/*!
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
declare class HiggsInitialisationData {
    __higgs__platform: string[];
    __higgs__functions: string[];
    __higgs__registeredGlobalEventIds: string[];
    __higgs__sliders: string[];
    __higgs__toggles: string[];
    __higgs__comboBoxes: string[];
}
type Listener = (_: unknown) => void;
declare class ListenerList {
    listeners: Map<number, Listener>;
    listenerId: number;
    addListener(fn: Listener): number;
    removeListener(id: number): void;
    callListeners(payload: unknown): void;
}
type ListenerHandle = [eventId: string, id: number];
declare class EventListenerList {
    eventListeners: Map<string, ListenerList>;
    addEventListener(eventId: string, fn: (payload: unknown) => void): ListenerHandle;
    removeEventListener([eventId, id]: ListenerHandle): void;
    emitEvent(eventId: string, object: unknown): void;
}
declare class Backend {
    listeners: EventListenerList;
    /**
     * Registers a function to listen to events emitted by the backend. The backend can
     * emit such events by calling WebBrowserComponent::emitEventIfBrowserIsVisible.
     *
     * @param eventId The identifier of the emitted event.
     * @param fn      The function to be called when the event is received.
     */
    addEventListener(eventId: string, fn: (payload: unknown) => void): ListenerHandle;
    removeEventListener(handle: ListenerHandle): void;
    /**
     * Sends an event from the frontend to the native backend.
     *
     * @param eventId The identifier of the event to emit.
     * @param object  The payload to send along with the event.
     */
    emitEvent(eventId: string, object: unknown): void;
    /**
     * Internal function called by the HIGGS backend implementation.
     *
     * User Javascript code running inside the WebView should not call this
     * function.
     *
     * @param eventId The identifier of the event to emit.
     * @param object  The JSON-encoded payload to send along with the event.
     */
    emitByBackend(eventId: string, object: string): void;
}
interface HiggsGlobal {
    postMessage: (message: string) => void;
    initialisationData: HiggsInitialisationData;
    backend: Backend;
    getAndroidUserScripts?: () => string;
}
declare global {
    interface Window {
        __HIGGS__: HiggsGlobal;
        inAndroidUserScriptEval?: true;
    }
}
export {};
//# sourceMappingURL=check_native_interop.d.ts.map