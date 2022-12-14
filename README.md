# tardigrade

[![Build Status](https://github.com/karnkaul/tardigrade/actions/workflows/ci.yml/badge.svg)](https://github.com/karnkaul/tardigrade/actions/workflows/ci.yml)

## Tiny Game Engine Framework

`tardigrade` provides a backend-generic framework to quickly setup and run scenes as a game engine.

## Requirements

1. CMake 3.18+
1. C++20 compiler

## Setup

`tardigrade` is intended to be built from source and statically linked.

1. Make `tardigrade` available to CMake, using one of the following approaches:
    1. Add to build tree:
        1. Use CMake FetchContent to obtain `tardigrade`, or clone repository into local directory, or add it as a git submodule
        1. Add into existing CMake build tree via  `FetchContent_MakeAvailable()` / `add_subdirectory()`
    1. Install package:
        1. Build and install Debug and Release configurations via CMake
        1. Use `find_package(tardigrade CONFIG)` to import into build tree
    1. Manual:
        1. Copy `include/tardigrade` to `<project_include_path>`
        1. Add files in `src` to build or place tardigrade static library binary in `<project_library_paths>`
1. Link via `target_link_libraries(your_project PRIVATE tardigrade::tardigrade)` if using CMake
1. Use via `#include <tardigrade/tardigrade.hpp>` or any other headers

## Usage

Scene graph:

```
            Director
                |
              Scene
                |
            -----------------
            |       |       |
          Entity  Entity  ...
            |
      -------------------------
      |           |           |
  Attachment  Attachment     ... 
```

These classes form the basis of the framework:

### Entity

Represents a unique object in a scene, supports attaching / detaching / querying for `Attachment`s - one instance per unique/concrete type. Cannot be derived from (is marked `final`). Sets the `Entity::Id` of owned attachments to its own. Provides APIs to manipulate attachments, toggle activity, set destroyed (removed from owning `Scene` after tick), obtain a pointer to the owning `Scene` (if any), etc. Once an entity starts ticking, the list of its attachments is frozen. In other words, attachments attached in the midst of an entity's tick will not be ticked or rendered until the next frame.

### DeltaTime

Represents the real and time-scaled delta-time between two frames.

### Attachment

Abstract base class for an arbitrary component to attach to an `Entity`. Exposes `virtual void setup() = 0`, called by `Entity` once, right after construction. Provides a pointer to owning `Entity` (if any) through a stored `Entity::Id` (entity must be owned by a scene to find it via its ID).

#### TickAttachment

Abstract attachment that exposes `virtual void tick(DeltaTime) = 0`, called every frame by the owning `Entity`. Also exposes a customizable `int order` for the owning entity to sort attachments before ticking them.

#### RenderAttachment

Abstract tick attachment that exposes `virtual void render(RenderTarget const&) const = 0`, called every frame by the owning `Entity`. Also exposes a customizable `int layer` for the owning scene to sort attachments before drawing them. Sets `order = tick_order_v` (default `100`) in `setup()`: this enables all render attachments on an entity to be ticked after all its tick attachments, by default. (Don't forget to call `RenderAttachment::setup()` in overrides!)

### RenderTarget

Base class for custom render targets / frames. A const reference is passed to all active `RenderAttachment` instances.

### Scene

Pinned (non-movable, non-copiable) base class that represents a set of `Entity`s associated with their `Entity::Id`s. Sets the `Scene` pointer of owned entities to itself. Provides APIs to tick and render all of them, and a single `virtual void setup()`, called by `Director` on a scene being loaded. Once a scene starts ticking, the list of its entities is frozen. In other words, entities spawned in the midst of a scene's tick will not be ticked or rendered until the next frame.

### Director

Pinned (non-movable, non-copiable) class that drives an active `Scene`. Provides APIs to load another scene that replaces the active one on the next `tick`, to tick and render owned entities, and to modify the time scale (default `1.0f`).

### Services

Globally accessible typed non-owning service locator (stores one pointer per type); `Services::Scoped` clears all attached services on destruction. Manually attach/detach services via `Services::provide(Service)` and `Services::unprovide<Service>()`. Store an instance of `ServiceProvider<Type>` (pinned) for auto provision via RAII. Locate a service via `locate<Service>()` - returns a pointer, asserts that it is not null. Use `Services::find<Service>()` to bypass the assert, `Services::contains<Service>()` to check presence.

## Example

See [example.cpp](example/example.cpp) for a concrete example using [vulkify](https://github.com/vulkify/vulkify).

## Misc

[Original repository](https://github.com/karnkaul/tardigrade)

[LICENCE](LICENSE)
