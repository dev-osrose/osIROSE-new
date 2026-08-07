# Unit tests

Unit tests are really useful to test each unit in isolation from each other.
Unfortunately, the unit tests here are not completely isolated because we chose to use singletons, which are really hard to decouple from the rest of the code.
You are welcome to add any tests you might see fit.

The end goal of the project would be to use dependency injection instead of singletons throughout the codebase to allow full coverage of the code.

## Running them

Tests are off by default. Both flags are required — `WITH_GTEST` is force-disabled unless `BUILD_TESTS` is also on.

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DWITH_GTEST=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Writing a new one

`src/tests/CMakeLists.txt` has two macros. Prefer the first:

- `compile_unit_test(<Name> <sources...>)` compiles the unit under test straight into the test binary and links only `Core`. Hermetic and fast.
- `compile_test(<Name> <source>)` links `osirose-lib`, the static library built from every login/char/map source except their `main()`s. Needed when the unit under test lives in a translation unit that reaches `EntitySystem`, which drags in Lua, curl and the database.

Each file gets its own binary, so each gets its own process — which is what makes the singletons (`Core::Config`, `Core::Random`, `ItemDatabase`) survivable. `gtest_discover_tests` then registers every `TEST` in it with CTest individually.

## Tests that need extra build flags

Two tests depend on a flag beyond `BUILD_TESTS`/`WITH_GTEST`. A third,
`test_lua_data.cpp`, changes shape with `CMAKE_BUILD_TYPE` rather than a flag —
see "Tests that document bugs" below.

### `ENABLE_SSL`

`test_ssl_config.cpp` covers `rose_ssl_config.h`, the bridge between the `[ssl]`
block in `server.json` and the TLS setters on the sockets. It has a body for
*both* build flavours rather than a skip: with `-DENABLE_SSL=ON` it asserts the
field mapping, the SNI precedence and the `ApplySsl*Config` return values;
without it, it asserts that the helpers really do compile down to no-ops that
never touch the socket. `TestConfig`'s `[ssl]` cases are flag-independent —
`Core::Config::Ssl` is a plain struct in every build.

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DWITH_GTEST=ON -DENABLE_SSL=ON
cmake --build build
ctest --test-dir build --output-on-failure -R 'Config|SslConfig'
```

CI's `linux_debug` job builds with `-DENABLE_SSL=ON`; `linux_release` stays
plaintext, so both sides of every `#ifdef USE_SSL` keep compiling.

The TLS implementation itself — `enable_ssl_server`/`enable_ssl_client`, the
handshake, the handshake timeout — lives in the upstream `packets` repo and is
tested there, against certificates minted at runtime. See `tests/` in that
repo; its CI runs the suite with `ENABLE_SSL=ON`.

### `WITH_CRASH_REPORTS`

Crashpad is only compiled in with
`-DWITH_CRASH_REPORTS=ON`; without it `Core::CrashReport` is an empty stub, so
two of the three tests in `test_crash_report.cpp` `GTEST_SKIP()` rather than
pass vacuously:

| Test | needs `WITH_CRASH_REPORTS=ON` |
| --- | --- |
| `MissingHandlerLeavesReporterInactive` | no — a bad handler path must fail closed either way |
| `StartsHandlerAndInitialisesDatabase` | yes |
| `CrashInChildProducesMinidump` | yes, and POSIX (it `fork()`s) |

The point of the gated pair is that the breakpad code this replaced spent years
compiling down to an empty stub because no CMakeLists ever defined
`ENABLE_CRASH_REPORTS`, and nothing caught it. To actually run them:

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DWITH_GTEST=ON -DWITH_CRASH_REPORTS=ON
cmake --build build
ctest --test-dir build --output-on-failure -R CrashReport
```

CI builds with `WITH_CRASH_REPORTS=OFF`, so on CI these two report as skipped.

## What is not covered

Deliberately, so the gaps are known rather than assumed:

- **Anything taking `EntitySystem&`** — including `Calculations::get_maxhp`/`get_maxmp` and the combat damage formulas. `EntitySystem`'s constructor needs `server.json`, a live MariaDB with a populated `item_db`, a running `NetworkThreadPool`, a Lua interpreter and the whole `scripts/` tree. That is integration-test territory.
- **The SQL call sites.** CI has no database service. The 15-case smoke program written during the sqlpp23 migration is the obvious seed if that changes.
- **Combat randomness** — `Core::Random` is a singleton with no `seed()` and lives in the upstream common repo, so nothing that draws from it is deterministic.
- **`Nearby`** — `grid` is private and the only observers take `EntitySystem&`, so the three mutators cannot be asserted. Its `get_grid_position` has a latent bug (`uint16_t gx = x / 1000.f` is UB for negative coordinates) that a test should pin but currently cannot reach.

## Tests that document bugs

Several tests pin *current* behavior rather than correct behavior, because fixing them is out of scope for a test pass. Each says so in a comment. If you fix one of these, the test is what tells you the old behavior is gone:

- `test_inventory_layout.cpp` — `getEquipped()` spans one slot too many and overlaps the first inventory slot.
- `test_id_manager.cpp` — `release_id` has no ownership check, and `_max_id` wraps silently at 65535.
- `test_lua_data.cpp` — `LuaData::get_data` is an unguarded `static_cast` whose behavior depends on the build type, because sol2 gates its type checks on `SOL_SAFE_GETTER`, which follows `NDEBUG`. In Debug it aborts the process on a missing key, a string, or a float-subtype value; in Release it silently reads a missing key as `0`, truncates floats and coerces strings. The file asserts both halves — `LuaDataDeathTest` in Debug, `LuaDataUncheckedGetter` in Release. The Release half is the more dangerous one: a script that misspells a stat produces a zeroed mob with no crash, no log line and nothing tying the result back to the script.
- `test_exp_curve.cpp` — `get_exp_to_level` clamps at the top but not the bottom.
- `test_escape_data.cpp` — `escapeData` does not escape backslashes and is not idempotent.
- `test_config.cpp` — `Config::getInstance` ignores the filename after the first call, and its accessors hand out mutable references to singleton state.
- `test_ssl_config.cpp` — `requireClientCert` is a server-side flag that `MakeSslClientConfig` also reads as "present our own certificate outbound"; the coupling is invisible at every call site.

Upstream, in `packets/tests/`:

- `test_ssl_config.cpp` — OpenSSL does not stat a `CApath`, so a typo'd `ssl.caPath` configures cleanly and then fails every handshake.
- `test_ssl_handshake.cpp` — `DrainNetwork()` exists because `CNetwork_Asio`'s asynchronous handlers capture a raw `this` with no tie to the object's lifetime. `shutdown(true)` only *schedules* the cancelled operations' handlers, and the accept loop's re-arm (`cnetwork_asio.cpp:635`) then makes a virtual call on a destroyed object. The servers survive the same teardown only because `main()` sleeps for a second first.
