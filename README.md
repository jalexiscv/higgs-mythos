# Higgs Mythos

Higgs Mythos is a cross-platform C++ framework for desktop applications,
audio plug-ins and plug-in hosts.

## Modules

The framework lives in `modules/`. Each module is named `higgs_<area>`, its
public header is `<module>/<module>.h`, and the API is declared in the `higgs`
namespace.

## Building

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON
cmake --build cmake-build --target DemoRunner
```

## Licensing

Higgs Mythos is released under the AGPL-3.0-or-later. See `LICENSE.md`.
