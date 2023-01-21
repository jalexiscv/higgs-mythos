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

class HiggsInitialisationData {
  __higgs__platform: string[] = [];
  __higgs__functions: string[] = [];
  __higgs__registeredGlobalEventIds: string[] = [];
  __higgs__sliders: string[] = [];
  __higgs__toggles: string[] = [];
  __higgs__comboBoxes: string[] = [];
}

type Listener = (_: unknown) => void;

class ListenerList {
  listeners: Map<number, Listener> = new Map();
  listenerId: number = 0;

  addListener(fn: Listener): number {
    const newListenerId = this.listenerId++;
    this.listeners.set(newListenerId, fn);
    return newListenerId;
  }

  removeListener(id: number): void {
    if (this.listeners.has(id)) {
      this.listeners.delete(id);
    }
  }

  callListeners(payload: unknown): void {
    for (const [, value] of this.listeners) {
      value(payload);
    }
  }
}

type ListenerHandle = [eventId: string, id: number];

class EventListenerList {
  eventListeners: Map<string, ListenerList> = new Map();

  addEventListener(
    eventId: string,
    fn: (payload: unknown) => void,
  ): ListenerHandle {
    let list = this.eventListeners.get(eventId);
    if (!list) {
      list = new ListenerList();
      this.eventListeners.set(eventId, list);
    }

    const id = list.addListener(fn);

    return [eventId, id];
  }

  removeEventListener([eventId, id]: ListenerHandle): void {
    this.eventListeners.get(eventId)?.removeListener(id);
  }

  emitEvent(eventId: string, object: unknown): void {
    this.eventListeners.get(eventId)?.callListeners(object);
  }
}

class Backend {
  listeners: EventListenerList = new EventListenerList();

  /**
   * Registers a function to listen to events emitted by the backend. The backend can
   * emit such events by calling WebBrowserComponent::emitEventIfBrowserIsVisible.
   *
   * @param eventId The identifier of the emitted event.
   * @param fn      The function to be called when the event is received.
   */
  addEventListener(
    eventId: string,
    fn: (payload: unknown) => void,
  ): ListenerHandle {
    return this.listeners.addEventListener(eventId, fn);
  }

  removeEventListener(handle: ListenerHandle): void {
    this.listeners.removeEventListener(handle);
  }

  /**
   * Sends an event from the frontend to the native backend.
   *
   * @param eventId The identifier of the event to emit.
   * @param object  The payload to send along with the event.
   */
  emitEvent(eventId: string, object: unknown): void {
    window.__HIGGS__.postMessage(
      JSON.stringify({ eventId: eventId, payload: object }),
    );
  }

  /**
   * Internal function called by the HIGGS backend implementation.
   *
   * User Javascript code running inside the WebView should not call this
   * function.
   *
   * @param eventId The identifier of the event to emit.
   * @param object  The JSON-encoded payload to send along with the event.
   */
  emitByBackend(eventId: string, object: string): void {
    this.listeners.emitEvent(eventId, JSON.parse(object));
  }
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

if (
  typeof window.__HIGGS__ !== 'undefined' &&
  typeof window.__HIGGS__.getAndroidUserScripts !== 'undefined' &&
  typeof window.inAndroidUserScriptEval === 'undefined'
) {
  window.inAndroidUserScriptEval = true;
  (0, eval)(window.__HIGGS__.getAndroidUserScripts());
  delete window.inAndroidUserScriptEval;
}

if (typeof window.__HIGGS__ === 'undefined') {
  console.warn(
    "The 'window.__HIGGS__' object is undefined." +
      ' Native integration features will not work.' +
      " Defining a placeholder 'window.__HIGGS__' object.",
  );

  window.__HIGGS__ = {} as HiggsGlobal;
}

if (typeof window.__HIGGS__.postMessage === 'undefined') {
  window.__HIGGS__.postMessage = function () {};
}

if (typeof window.__HIGGS__.initialisationData === 'undefined') {
  window.__HIGGS__.initialisationData = new HiggsInitialisationData();
}

if (typeof window.__HIGGS__.backend === 'undefined') {
  window.__HIGGS__.backend = new Backend();
}
