#Unit tests
Unit tests are really useful to test each unit in isolation from each other.
Unfortunately, the unit tests here are not completely isolated because we chose to use singletons, which are really hard to decouple from the rest of the code.
The unit tests coverage is mostly centered around the [Core library](../core/core.md) and the [RoseCommon library](../rosecommon/rosecommon.md).
You are welcome to add any tests you might see fit.

There are a couple of mocks present. Mocks are fake classes that implement instances. It is mostly used with dependency injection, in order to decouple units as much as possible.
The end goal of the project would be to use dependency injection instead of singleton throughout the codebase to allow full coverage of the code.

