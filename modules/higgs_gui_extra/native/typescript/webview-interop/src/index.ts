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

import './check_native_interop';

interface HiggsCompleteEvent {
  promiseId: number;
  result: unknown;
}

class PromiseHandler {
  lastPromiseId: number = 0;
  promises: Map<
    number,
    { resolve: (value: unknown) => void; reject: (reason?: unknown) => void }
  > = new Map();

  constructor() {
    window.__HIGGS__.backend.addEventListener('__higgs__complete', (payload) => {
      const { promiseId, result } = payload as HiggsCompleteEvent;

      const entry = this.promises.get(promiseId);
      if (entry) {
        entry.resolve(result);
        this.promises.delete(promiseId);
      }
    });
  }

  createPromise(): [number, Promise<unknown>] {
    const promiseId = this.lastPromiseId++;
    const result = new Promise<unknown>((resolve, reject) => {
      this.promises.set(promiseId, { resolve: resolve, reject: reject });
    });
    return [promiseId, result];
  }
}

const promiseHandler = new PromiseHandler();

/**
 * Returns a function object that calls a function registered on the HIGGS backend and forwards all
 * parameters to it.
 *
 * The provided name should be the same as the name argument passed to
 * WebBrowserComponent::Options.withNativeFunction() on the backend.
 *
 * @param {String} name
 */
function getNativeFunction(name: string) {
  if (!window.__HIGGS__.initialisationData.__higgs__functions.includes(name))
    console.warn(
      `Creating native function binding for '${name}', which is unknown to the backend`,
    );

  const f = (...args: unknown[]): Promise<unknown> => {
    const [promiseId, result] = promiseHandler.createPromise();

    window.__HIGGS__.backend.emitEvent('__higgs__invoke', {
      name: name,
      params: args,
      resultId: promiseId,
    });

    return result;
  };

  return f;
}

//==============================================================================

declare const __HIGGS_FRAMEWORK_WEBVIEW_VERSION__: string;

const versionCheckFunctionName = '__higgs__reportHiggsFrameworkWebViewVersion';

if (
  window.__HIGGS__.initialisationData.__higgs__functions.includes(
    versionCheckFunctionName,
  )
) {
  getNativeFunction(versionCheckFunctionName)(
    __HIGGS_FRAMEWORK_WEBVIEW_VERSION__,
  ).then((backendVersion) => {
    if (backendVersion !== __HIGGS_FRAMEWORK_WEBVIEW_VERSION__) {
      console.warn(
        `The backend expected a @jalexiscv/webview version of ${backendVersion}, but got ${__HIGGS_FRAMEWORK_WEBVIEW_VERSION__}. You should use matching backend and frontend versions.`,
      );
    }
  });
}

//==============================================================================

class ListenerList {
  listeners: Map<number, () => void> = new Map();
  listenerId = 0;

  addListener(fn: () => void): number {
    const newListenerId = this.listenerId++;
    this.listeners.set(newListenerId, fn);
    return newListenerId;
  }

  removeListener(id: number): void {
    if (this.listeners.has(id)) {
      this.listeners.delete(id);
    }
  }

  callListeners(): void {
    for (const [, value] of this.listeners) {
      value();
    }
  }
}

const BasicControl_valueChangedEventId = 'valueChanged';
const BasicControl_propertiesChangedId = 'propertiesChanged';
const SliderControl_sliderDragStartedEventId = 'sliderDragStarted';
const SliderControl_sliderDragEndedEventId = 'sliderDragEnded';

type BasicControlEvent<TValue, TProperties> =
  | { eventType: typeof BasicControl_valueChangedEventId; value: TValue }
  | ({ eventType: typeof BasicControl_propertiesChangedId } & TProperties);

interface SliderProperties {
  start: number;
  end: number;
  skew: number;
  name: string;
  label: string;
  numSteps: number;
  interval: number;
  parameterIndex: number;
}

type SliderEvent = BasicControlEvent<number, SliderProperties>;

/**
 * SliderState encapsulates data and callbacks that are synchronised with a WebSliderRelay object
 * on the backend.
 *
 * Use getSliderState() to create a SliderState object. This object will be synchronised with the
 * WebSliderRelay backend object that was created using the same unique name.
 *
 * @param {String} name
 */
class SliderState {
  name: string;
  identifier: string;
  scaledValue = 0;
  properties: SliderProperties = {
    start: 0,
    end: 1,
    skew: 1,
    name: '',
    label: '',
    numSteps: 100,
    interval: 0,
    parameterIndex: -1,
  };
  valueChangedEvent = new ListenerList();
  propertiesChangedEvent = new ListenerList();

  constructor(name: string) {
    if (!window.__HIGGS__.initialisationData.__higgs__sliders.includes(name))
      console.warn(
        "Creating SliderState for '" +
          name +
          "', which is unknown to the backend",
      );

    this.name = name;
    this.identifier = '__higgs__slider' + this.name;

    window.__HIGGS__.backend.addEventListener(this.identifier, (event) =>
      this.handleEvent(event),
    );

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: 'requestInitialUpdate',
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
  setNormalisedValue(newValue: number): void {
    this.scaledValue = this.snapToLegalValue(
      this.normalisedToScaledValue(newValue),
    );

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.scaledValue,
    });
  }

  /**
   * This function should be called first thing when the user starts interacting with the slider.
   */
  sliderDragStarted(): void {
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: SliderControl_sliderDragStartedEventId,
    });
  }

  /**
   * This function should be called when the user finished the interaction with the slider.
   */
  sliderDragEnded(): void {
    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: SliderControl_sliderDragEndedEventId,
    });
  }

  /** Internal. */
  handleEvent(event: unknown): void {
    const e = event as SliderEvent;

    if (e.eventType === BasicControl_valueChangedEventId) {
      this.scaledValue = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType === BasicControl_propertiesChangedId) {
      // eslint-disable-next-line no-unused-vars
      let { eventType: _, ...rest } = e;
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }

  /**
   * Returns the scaled value of the parameter. This corresponds to the return value of
   * NormalisableRange::convertFrom0to1() (C++). This value will differ from a linear
   * [0, 1] range if a non-default NormalisableRange was set for the parameter.
   */
  getScaledValue(): number {
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
  getNormalisedValue(): number {
    return Math.pow(
      (this.scaledValue - this.properties.start) /
        (this.properties.end - this.properties.start),
      this.properties.skew,
    );
  }

  /** Internal. */
  normalisedToScaledValue(normalisedValue: number): number {
    return (
      Math.pow(normalisedValue, 1 / this.properties.skew) *
        (this.properties.end - this.properties.start) +
      this.properties.start
    );
  }

  /** Internal. */
  snapToLegalValue(value: number): number {
    const interval = this.properties.interval;

    if (interval == 0) return value;

    const start = this.properties.start;
    const clamp = (val: number, min = 0, max = 1) =>
      Math.max(min, Math.min(max, val));

    return clamp(
      start + interval * Math.floor((value - start) / interval + 0.5),
      this.properties.start,
      this.properties.end,
    );
  }
}

const sliderStates: Map<string, SliderState> = new Map();

for (const sliderName of window.__HIGGS__.initialisationData.__higgs__sliders)
  sliderStates.set(sliderName, new SliderState(sliderName));

/**
 * Returns a SliderState object that is connected to the backend WebSliderRelay object that was
 * created with the same name argument.
 *
 * To register a WebSliderRelay object create one with the right name and add it to the
 * WebBrowserComponent::Options struct using withOptionsFrom.
 *
 * @param {String} name
 */
function getSliderState(name: string): SliderState {
  let state = sliderStates.get(name);
  if (!state) {
    state = new SliderState(name);
    sliderStates.set(name, state);
  }

  return state;
}

interface ToggleProperties {
  name: string;
  parameterIndex: number;
}

type ToggleEvent = BasicControlEvent<boolean, ToggleProperties>;

/**
 * ToggleState encapsulates data and callbacks that are synchronised with a WebToggleRelay object
 * on the backend.
 *
 * Use getToggleState() to create a ToggleState object. This object will be synchronised with the
 * WebToggleRelay backend object that was created using the same unique name.
 *
 * @param {String} name
 */
class ToggleState {
  name: string;
  identifier: string;
  value: boolean = false;
  properties: ToggleProperties = {
    name: '',
    parameterIndex: -1,
  };
  valueChangedEvent: ListenerList = new ListenerList();
  propertiesChangedEvent: ListenerList = new ListenerList();

  constructor(name: string) {
    if (!window.__HIGGS__.initialisationData.__higgs__toggles.includes(name))
      console.warn(
        "Creating ToggleState for '" +
          name +
          "', which is unknown to the backend",
      );

    this.name = name;
    this.identifier = '__higgs__toggle' + this.name;

    window.__HIGGS__.backend.addEventListener(this.identifier, (event) =>
      this.handleEvent(event),
    );

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: 'requestInitialUpdate',
    });
  }

  /** Returns the value corresponding to the associated WebToggleRelay's (C++) state. */
  getValue(): boolean {
    return this.value;
  }

  /** Informs the backend to change the associated WebToggleRelay's (C++) state. */
  setValue(newValue: boolean): void {
    this.value = newValue;

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.value,
    });
  }

  /** Internal. */
  handleEvent(event: unknown): void {
    const e = event as ToggleEvent;

    if (e.eventType == BasicControl_valueChangedEventId) {
      this.value = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType == BasicControl_propertiesChangedId) {
      // eslint-disable-next-line no-unused-vars
      let { eventType: _, ...rest } = e;
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }
}

const toggleStates: Map<string, ToggleState> = new Map();

for (const name of window.__HIGGS__.initialisationData.__higgs__toggles)
  toggleStates.set(name, new ToggleState(name));

/**
 * Returns a ToggleState object that is connected to the backend WebToggleButtonRelay object that was
 * created with the same name argument.
 *
 * To register a WebToggleButtonRelay object create one with the right name and add it to the
 * WebBrowserComponent::Options struct using withOptionsFrom.
 *
 * @param {String} name
 */
function getToggleState(name: string): ToggleState {
  let state = toggleStates.get(name);
  if (!state) {
    state = new ToggleState(name);
    toggleStates.set(name, state);
  }

  return state;
}

interface ComboBoxProperties {
  name: string;
  parameterIndex: number;
  choices: string[];
}

type ComboBoxEvent = BasicControlEvent<number, ComboBoxProperties>;

/**
 * ComboBoxState encapsulates data and callbacks that are synchronised with a WebComboBoxRelay object
 * on the backend.
 *
 * Use getComboBoxState() to create a ComboBoxState object. This object will be synchronised with the
 * WebComboBoxRelay backend object that was created using the same unique name.
 *
 * @param {String} name
 */
class ComboBoxState {
  name: string;
  identifier: string;
  value = 0.0;
  properties: ComboBoxProperties = {
    name: '',
    parameterIndex: -1,
    choices: [],
  };
  valueChangedEvent: ListenerList = new ListenerList();
  propertiesChangedEvent: ListenerList = new ListenerList();

  constructor(name: string) {
    if (!window.__HIGGS__.initialisationData.__higgs__comboBoxes.includes(name))
      console.warn(
        "Creating ComboBoxState for '" +
          name +
          "', which is unknown to the backend",
      );

    this.name = name;
    this.identifier = '__higgs__comboBox' + this.name;

    window.__HIGGS__.backend.addEventListener(this.identifier, (event) =>
      this.handleEvent(event),
    );

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: 'requestInitialUpdate',
    });
  }

  /**
   * Returns the value corresponding to the associated WebComboBoxRelay's (C++) state.
   *
   * This is an index identifying which element of the properties.choices array is currently
   * selected.
   */
  getChoiceIndex(): number {
    return Math.round(this.value * (this.properties.choices.length - 1));
  }

  /**
   * Informs the backend to change the associated WebComboBoxRelay's (C++) state.
   *
   * This should be called with the index identifying the selected element from the
   * properties.choices array.
   */
  setChoiceIndex(index: number): void {
    const numItems = this.properties.choices.length;
    this.value = numItems > 1 ? index / (numItems - 1) : 0.0;

    window.__HIGGS__.backend.emitEvent(this.identifier, {
      eventType: BasicControl_valueChangedEventId,
      value: this.value,
    });
  }

  /** Internal. */
  handleEvent(event: unknown): void {
    const e = event as ComboBoxEvent;

    if (e.eventType == BasicControl_valueChangedEventId) {
      this.value = e.value;
      this.valueChangedEvent.callListeners();
    }
    if (e.eventType == BasicControl_propertiesChangedId) {
      // eslint-disable-next-line no-unused-vars
      let { eventType: _, ...rest } = e;
      this.properties = rest;
      this.propertiesChangedEvent.callListeners();
    }
  }
}

const comboBoxStates: Map<string, ComboBoxState> = new Map();

for (const name of window.__HIGGS__.initialisationData.__higgs__comboBoxes)
  comboBoxStates.set(name, new ComboBoxState(name));

/**
 * Returns a ComboBoxState object that is connected to the backend WebComboBoxRelay object that was
 * created with the same name argument.
 *
 * To register a WebComboBoxRelay object create one with the right name and add it to the
 * WebBrowserComponent::Options struct using withOptionsFrom.
 *
 * @param {String} name
 */
function getComboBoxState(name: string): ComboBoxState {
  let state = comboBoxStates.get(name);
  if (!state) {
    state = new ComboBoxState(name);
    comboBoxStates.set(name, state);
  }

  return state;
}

/**
 * Appends a platform-specific prefix to the path to ensure that a request sent to this address will
 * be received by the backend's ResourceProvider.
 * @param {String} path
 */
function getBackendResourceAddress(path: string): string {
  const platform =
    window.__HIGGS__.initialisationData.__higgs__platform.length > 0
      ? window.__HIGGS__.initialisationData.__higgs__platform[0]
      : '';

  if (platform == 'windows' || platform == 'android')
    return 'https://higgs.backend/' + path;

  if (platform == 'macos' || platform == 'ios' || platform == 'linux')
    return 'higgs://higgs.backend/' + path;

  console.warn(
    'getBackendResourceAddress() called, but no HIGGS native backend is detected.',
  );
  return path;
}

/**
 * This helper class is intended to aid the implementation of
 * AudioProcessorEditor::getControlParameterIndex() for editors using a WebView interface.
 *
 * Create an instance of this class and call its handleMouseMove() method in each mousemove event.
 *
 * This class can be used to continuously report the controlParameterIndexAnnotation attribute's
 * value related to the DOM element that is currently under the mouse pointer.
 *
 * This value is defined at all times as follows
 * * the annotation attribute's value for the DOM element directly under the mouse, if it has it,
 * * the annotation attribute's value for the first parent element, that has it,
 * * -1 otherwise.
 *
 * Whenever there is a change in this value, an event is emitted to the frontend with the new value.
 * You can use a ControlParameterIndexReceiver object on the backend to listen to these events.
 *
 * @param {String} controlParameterIndexAnnotation
 */
class ControlParameterIndexUpdater {
  controlParameterIndexAnnotation: string;
  lastElement: Element | null = null;
  lastControlParameterIndex = -1;

  constructor(controlParameterIndexAnnotation: string) {
    this.controlParameterIndexAnnotation = controlParameterIndexAnnotation;
  }

  handleMouseMove(event: MouseEvent): void {
    const currentElement = document.elementFromPoint(
      event.clientX,
      event.clientY,
    );

    if (currentElement === this.lastElement) return;
    this.lastElement = currentElement;

    let controlParameterIndex = -1;

    if (currentElement !== null)
      controlParameterIndex = this.#getControlParameterIndex(currentElement);

    if (controlParameterIndex === this.lastControlParameterIndex) return;
    this.lastControlParameterIndex = controlParameterIndex;

    window.__HIGGS__.backend.emitEvent(
      '__higgs__controlParameterIndexChanged',
      controlParameterIndex,
    );
  }

  //==============================================================================
  #getControlParameterIndex(element: Element | null): number {
    const isValidNonRootElement = (e: Element | null): e is Element => {
      return e !== null && e !== document.documentElement;
    };

    while (isValidNonRootElement(element)) {
      if (element.hasAttribute(this.controlParameterIndexAnnotation)) {
        const attribute = element.getAttribute(
          this.controlParameterIndexAnnotation,
        );

        const attributeValue = Number(attribute);

        if (attribute === null || Number.isNaN(attributeValue)) {
          console.error(
            `Expected the "${this.controlParameterIndexAnnotation}" attribute to be a number,
             got "${attributeValue}". You should set this attribute to the parameterIndex member
             of the corresponding SliderState, ToggleState or ComboBoxState object's properties.`,
          );

          return -1;
        }

        return attributeValue;
      }

      element = element.parentElement;
    }

    return -1;
  }
}

export {
  getNativeFunction,
  getSliderState,
  getToggleState,
  getComboBoxState,
  getBackendResourceAddress,
  ControlParameterIndexUpdater,
};
