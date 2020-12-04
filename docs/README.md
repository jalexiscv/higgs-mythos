# Higgs Mythos Documentation

This directory documents the Higgs Mythos module format, the CMake API, and the
framework conventions.

- [Higgs Mythos Module Format.md](Higgs%20Mythos%20Module%20Format.md) — how a
  module is declared, what its header contains, and how dependencies work.
- [CMake API.md](CMake%20API.md) — the `higgs_add_*` functions, module targets
  and build options.
- [higgs-mythos.md](higgs-mythos.md) — framework identity: naming conventions,
  licence and provenance.
- [Accessibility.md](Accessibility.md), [ARA.md](ARA.md) — platform and plug-in
  integration notes.
- [Linux Dependencies.md](Linux%20Dependencies.md) — packages required to build
  on Linux.

The modules live in the `modules` subdirectory of the repository, and the CMake
example projects in `examples/CMake`.

The API is documented inline; HTML docs can be generated from the source with
`doxygen`.
