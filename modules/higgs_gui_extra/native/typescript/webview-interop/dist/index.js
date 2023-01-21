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
var __getOwnPropSymbols = Object.getOwnPropertySymbols;
var __hasOwnProp = Object.prototype.hasOwnProperty;
var __propIsEnum = Object.prototype.propertyIsEnumerable;
var __typeError = (msg) => {
  throw TypeError(msg);
};
var __objRest = (source, exclude) => {
  var target = {};
  for (var prop in source)
    if (__hasOwnProp.call(source, prop) && exclude.indexOf(prop) < 0)
      target[prop] = source[prop];
  if (source != null && __getOwnPropSymbols)
    for (var prop of __getOwnPropSymbols(source)) {
      if (exclude.indexOf(prop) < 0 && __propIsEnum.call(source, prop))
        target[prop] = source[prop];
    }
  return target;
};
var __accessCheck = (obj, member, msg) => member.has(obj) || __typeError("Cannot " + msg);
var __privateAdd = (obj, member, value) => member.has(obj) ? __typeError("Cannot add the same private member more than once") : member instanceof WeakSet ? member.add(obj) : member.set(obj, value);
var __privateMethod = (obj, member, method) => (__accessCheck(obj, member, "access private method"), method);

// src/check_native_interop.ts
var HiggsInitialisationData = class {
  constructor() {
    this.__higgs__platform = [];
    this.__higgs__functions = [];
    this.__higgs__registeredGlobalEventIds = [];
    this.__higgs__sliders = [];
    this.__higgs__toggles = [];
    this.__higgs__comboBoxes = [];
  }
};
var ListenerList = class {
  constructor() {
    this.listeners = /* @__PURE__ */ new Map();
    this.listenerId = 0;
  }
  addListener(fn) {
    const newListenerId = this.listenerId++;
    this.listeners.set(newListenerId, fn);
    return newListenerId;
  }
  removeListener(id) {
    if (this.listeners.has(id)) {
      this.listeners.delete(id);
    }
  }
  callListeners(payload) {
    for (const [, value] of this.listeners) {
      value(payload);
    }
  }
};
var EventListenerList = class {
  constructor() {
    this.eventListeners = /* @__PURE__ */ new Map();
  }
  addEventListener(eventId, fn) {
    let list = this.eventListeners.get(eventId);
    if (!list) {
      list = new ListenerList();
      this.eventListeners.set(eventId, list);
    }
    const id = list.addListener(fn);
    return [eventId, id];
  }
  removeEventListener([eventId, id]) {
    var _a;
    (_a = this.eventListeners.get(eventId)) == null ? void 0 : _a.removeListener(id);
  }
  emitEvent(eventId, object) {
    var _a;
    (_a = this.eventListeners.get(eventId)) == null ? void 0 : _a.callListeners(object);
  }
};
var Backend = class {
  constructor() {
    this.listeners = new EventListenerList();
  }
  /**
   * Registers a function to listen to events emitted by the backend. The backend can
   * emit such events by calling WebBrowserComponent::emitEventIfBrowserIsVisible.
   *
   * @param eventId The identifier of the emitted event.
   * @param fn      The function to be called when the event is received.
   */
  addEventListener(eventId, fn) {
    return this.listeners.addEventListener(eventId, fn);
  }
  removeEventListener(handle) {
    this.listeners.removeEventListener(handle);
  }
  /**
   * Sends an event from the frontend to the native backend.
   *
   * @param eventId The identifier of the event to emit.
   * @param object  The payload to send along with the event.
   */
  emitEvent(eventId, object) {
    window.__HIGGS__.postMessage(
      JSON.stringify({ eventId, payload: object })
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
  emitByBackend(eventId, object) {
    this.listeners.emitEvent(eventId, JSON.parse(object));
  }
};
if (typeof window.__HIGGS__ !== "undefined" && typeof window.__HIGGS__.getAndroidUserScripts !== "undefined" && typeof window.inAndroidUserScriptEval === "undefined") {
  window.inAndroidUserScriptEval = true;
  (0, eval)(window.__HIGGS__.getAndroidUserScripts());
  delete window.inAndroidUserScriptEval;
}
if (typeof window.__HIGGS__ === "undefined") {
  console.warn(
    "The 'window.__HIGGS__' object is undefined. Native integration features will not work. Defining a placeholder 'window.__HIGGS__' object."
  );
  window.__HIGGS__ = {};
}
if (typeof window.__HIGGS__.postMessage === "undefined") {
  window.__HIGGS__.postMessage = function() {
  };
}
if (typeof window.__HIGGS__.initialisationData === "undefined") {
  window.__HIGGS__.initialisationData = new HiggsInitialisationData();
}
if (typeof window.__HIGGS__.backend === "undefined") {
  window.__HIGGS__.backend = new Backend();
}

// src/index.ts
var PromiseHandler = class {
  constructor() {
    this.lastPromiseId = 0;
    this.promises = /* @__PURE__ */ new Map();
    window.__HIGGS__.backend.addEventListener("__higgs__complete", (payload) => {
      const { promiseId, result } = payload;
      const entry = this.promises.get(promiseId);
      if (entry) {
        entry.resolve(result);
        this.promises.delete(promiseId);
      }
    });
  }
  createPromise() {
    const promiseId = this.lastPromiseId++;
    const result = new Promise((resolve, reject) => {
      this.promises.set(promiseId, { resolve, reject });
    });
    return [promiseId, result];
  }
};
var promiseHandler = new PromiseHandler();
function getNativeFunction(name) {
  if (!window.__HIGGS__.initialisationData.__higgs__functions.includes(name))
    console.warn(
      `Creating native function binding for '${name}', which is unknown to the backend`
    );
  const f = (...args) => {
    const [promiseId, result] = promiseHandler.createPromise();
    window.__HIGGS__.backend.emitEvent("__higgs__invoke", {
      name,
      params: args,
      resultId: promiseId
    });
    return result;
  };
  return f;
}
var versionCheckFunctionName = "__higgs__reportHiggsFrameworkWebViewVersion";
if (window.__HIGGS__.initialisationData.__higgs__functions.includes(
  versionCheckFunctionName
)) {
  getNativeFunction(versionCheckFunctionName)(
    "1.0.0"
  ).then((backendVersion) => {
    if (backendVersion !== "1.0.0") {
      console.warn(
        `The backend expected a @jalexiscv/webview version of ${backendVersion}, but got ${"1.0.0"}. You should use matching backend and frontend versions.`
      );
    }
  });
}
var ListenerList2 = class {
  constructor() {
    this.listeners = /* @__PURE__ */ new Map();
    this.listenerId = 0;
  }
  addListener(fn) {
    const newListenerId = this.listenerId++;
    this.listeners.set(newListenerId, fn);
    return newListenerId;
  }
  removeListener(id) {
    if (this.listeners.has(id)) {
      this.listeners.delete(id);
    }
  }
  callListeners() {
    for (const [, value] of this.listeners) {
      value();
    }
  }
};
var BasicControl_valueChangedEventId = "valueChanged";
var BasicControl_propertiesChangedId = "propertiesChanged";
var SliderControl_sliderDragStartedEventId = "sliderDragStarted";
var SliderControl_sliderDragEndedEventId = "sliderDragEnded";
var SliderState = class {
  constructor(name) {
    this.scaledValue = 0;
    this.properties = {
      start: 0,
      end: 1,
      skew: 1,
      name: "",
      label: "",
      numSteps: 100,
      interval: 0,
      parameterIndex: -1
    };
    this.valueChangedEvent = new ListenerList2();
    this.propertiesChangedEvent = new ListenerList2();
    if (!window.__HIGGS__.initialisationData.__higgs__sliders.includes(name))
      console.warn(
        "Creating SliderState for '" + name + "', which is unknown to the backend"
      );
    this.name = name;
    this.identifier = "__higgs__slider" + this.name;
    window.__HIGGS__.backend.addEventListener(
      this.identifier,
      (event) => this.handleEvent(event)
    );
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: "requestInitialUpdate"
    });
  }
  /**
   * Sets the normalised value of the corresponding backend parameter. This value is always in the
   * [0, 1] range (inclusive).
   *
   * The meaning of this range is the same as in the case of
   * AudioProcessorParameter::getValue() (C++).
   *
   * @param {String} name
   */
  setNormalisedValue(newValue) {
    this.scaledValue = this.snapToLegalValue(
      this.normalisedToScaledValue(newValue)
    );
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.scaledValue
    });
  }
  /**
   * This function should be called first thing when the user starts interacting with the slider.
   */
  sliderDragStarted() {
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: SliderControl_sliderDragStartedEventId
    });
  }
  /**
   * This function should be called when the user finished the interaction with the slider.
   */
  sliderDragEnded() {
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: SliderControl_sliderDragEndedEventId
    });
  }
  /** Internal. */
  handleEvent(event) {
    const e = event;
    if (e.eventType === BasicControl_valueChangedEventId) {
      this.scaledValue = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType === BasicControl_propertiesChangedId) {
      let _a = e, { eventType: _ } = _a, rest = __objRest(_a, ["eventType"]);
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }
  /**
   * Returns the scaled value of the parameter. This corresponds to the return value of
   * NormalisableRange::convertFrom0to1() (C++). This value will differ from a linear
   * [0, 1] range if a non-default NormalisableRange was set for the parameter.
   */
  getScaledValue() {
    return this.scaledValue;
  }
  /**
   * Returns the normalised value of the corresponding backend parameter. This value is always in the
   * [0, 1] range (inclusive).
   *
   * The meaning of this range is the same as in the case of
   * AudioProcessorParameter::getValue() (C++).
   *
   * @param {String} name
   */
  getNormalisedValue() {
    return Math.pow(
      (this.scaledValue - this.properties.start) / (this.properties.end - this.properties.start),
      this.properties.skew
    );
  }
  /** Internal. */
  normalisedToScaledValue(normalisedValue) {
    return Math.pow(normalisedValue, 1 / this.properties.skew) * (this.properties.end - this.properties.start) + this.properties.start;
  }
  /** Internal. */
  snapToLegalValue(value) {
    const interval = this.properties.interval;
    if (interval == 0) return value;
    const start = this.properties.start;
    const clamp = (val, min = 0, max = 1) => Math.max(min, Math.min(max, val));
    return clamp(
      start + interval * Math.floor((value - start) / interval + 0.5),
      this.properties.start,
      this.properties.end
    );
  }
};
var sliderStates = /* @__PURE__ */ new Map();
for (const sliderName of window.__HIGGS__.initialisationData.__higgs__sliders)
  sliderStates.set(sliderName, new SliderState(sliderName));
function getSliderState(name) {
  let state = sliderStates.get(name);
  if (!state) {
    state = new SliderState(name);
    sliderStates.set(name, state);
  }
  return state;
}
var ToggleState = class {
  constructor(name) {
    this.value = false;
    this.properties = {
      name: "",
      parameterIndex: -1
    };
    this.valueChangedEvent = new ListenerList2();
    this.propertiesChangedEvent = new ListenerList2();
    if (!window.__HIGGS__.initialisationData.__higgs__toggles.includes(name))
      console.warn(
        "Creating ToggleState for '" + name + "', which is unknown to the backend"
      );
    this.name = name;
    this.identifier = "__higgs__toggle" + this.name;
    window.__HIGGS__.backend.addEventListener(
      this.identifier,
      (event) => this.handleEvent(event)
    );
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: "requestInitialUpdate"
    });
  }
  /** Returns the value corresponding to the associated WebToggleRelay's (C++) state. */
  getValue() {
    return this.value;
  }
  /** Informs the backend to change the associated WebToggleRelay's (C++) state. */
  setValue(newValue) {
    this.value = newValue;
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.value
    });
  }
  /** Internal. */
  handleEvent(event) {
    const e = event;
    if (e.eventType == BasicControl_valueChangedEventId) {
      this.value = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType == BasicControl_propertiesChangedId) {
      let _a = e, { eventType: _ } = _a, rest = __objRest(_a, ["eventType"]);
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }
};
var toggleStates = /* @__PURE__ */ new Map();
for (const name of window.__HIGGS__.initialisationData.__higgs__toggles)
  toggleStates.set(name, new ToggleState(name));
function getToggleState(name) {
  let state = toggleStates.get(name);
  if (!state) {
    state = new ToggleState(name);
    toggleStates.set(name, state);
  }
  return state;
}
var ComboBoxState = class {
  constructor(name) {
    this.value = 0;
    this.properties = {
      name: "",
      parameterIndex: -1,
      choices: []
    };
    this.valueChangedEvent = new ListenerList2();
    this.propertiesChangedEvent = new ListenerList2();
    if (!window.__HIGGS__.initialisationData.__higgs__comboBoxes.includes(name))
      console.warn(
        "Creating ComboBoxState for '" + name + "', which is unknown to the backend"
      );
    this.name = name;
    this.identifier = "__higgs__comboBox" + this.name;
    window.__HIGGS__.backend.addEventListener(
      this.identifier,
      (event) => this.handleEvent(event)
    );
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: "requestInitialUpdate"
    });
  }
  /**
   * Returns the value corresponding to the associated WebComboBoxRelay's (C++) state.
   *
   * This is an index identifying which element of the properties.choices array is currently
   * selected.
   */
  getChoiceIndex() {
    return Math.round(this.value * (this.properties.choices.length - 1));
  }
  /**
   * Informs the backend to change the associated WebComboBoxRelay's (C++) state.
   *
   * This should be called with the index identifying the selected element from the
   * properties.choices array.
   */
  setChoiceIndex(index) {
    const numItems = this.properties.choices.length;
    this.value = numItems > 1 ? index / (numItems - 1) : 0;
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.value
    });
  }
  /** Internal. */
  handleEvent(event) {
    const e = event;
    if (e.eventType == BasicControl_valueChangedEventId) {
      this.value = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType == BasicControl_propertiesChangedId) {
      let _a = e, { eventType: _ } = _a, rest = __objRest(_a, ["eventType"]);
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }
};
var comboBoxStates = /* @__PURE__ */ new Map();
for (const name of window.__HIGGS__.initialisationData.__higgs__comboBoxes)
  comboBoxStates.set(name, new ComboBoxState(name));
function getComboBoxState(name) {
  let state = comboBoxStates.get(name);
  if (!state) {
    state = new ComboBoxState(name);
    comboBoxStates.set(name, state);
  }
  return state;
}
function getBackendResourceAddress(path) {
  const platform = window.__HIGGS__.initialisationData.__higgs__platform.length > 0 ? window.__HIGGS__.initialisationData.__higgs__platform[0] : "";
  if (platform == "windows" || platform == "android")
    return "https://higgs.backend/" + path;
  if (platform == "macos" || platform == "ios" || platform == "linux")
    return "higgs://higgs.backend/" + path;
  console.warn(
    "getBackendResourceAddress() called, but no HIGGS native backend is detected."
  );
  return path;
}
var _ControlParameterIndexUpdater_instances, getControlParameterIndex_fn;
var ControlParameterIndexUpdater = class {
  constructor(controlParameterIndexAnnotation) {
    __privateAdd(this, _ControlParameterIndexUpdater_instances);
    this.lastElement = null;
    this.lastControlParameterIndex = -1;
    this.controlParameterIndexAnnotation = controlParameterIndexAnnotation;
  }
  handleMouseMove(event) {
    const currentElement = document.elementFromPoint(
      event.clientX,
      event.clientY
    );
    if (currentElement === this.lastElement) return;
    this.lastElement = currentElement;
    let controlParameterIndex = -1;
    if (currentElement !== null)
      controlParameterIndex = __privateMethod(this, _ControlParameterIndexUpdater_instances, getControlParameterIndex_fn).call(this, currentElement);
    if (controlParameterIndex === this.lastControlParameterIndex) return;
    this.lastControlParameterIndex = controlParameterIndex;
    window.__HIGGS__.backend.emitEvent(
      "__higgs__controlParameterIndexChanged",
      controlParameterIndex
    );
  }
};
_ControlParameterIndexUpdater_instances = new WeakSet();
//==============================================================================
getControlParameterIndex_fn = function(element) {
  const isValidNonRootElement = (e) => {
    return e !== null && e !== document.documentElement;
  };
  while (isValidNonRootElement(element)) {
    if (element.hasAttribute(this.controlParameterIndexAnnotation)) {
      const attribute = element.getAttribute(
        this.controlParameterIndexAnnotation
      );
      const attributeValue = Number(attribute);
      if (attribute === null || Number.isNaN(attributeValue)) {
        console.error(
          `Expected the "${this.controlParameterIndexAnnotation}" attribute to be a number,
             got "${attributeValue}". You should set this attribute to the parameterIndex member
             of the corresponding SliderState, ToggleState or ComboBoxState object's properties.`
        );
        return -1;
      }
      return attributeValue;
    }
    element = element.parentElement;
  }
  return -1;
};
export {
  ControlParameterIndexUpdater,
  getBackendResourceAddress,
  getComboBoxState,
  getNativeFunction,
  getSliderState,
  getToggleState
};
//# sourceMappingURL=index.js.map
