# Gameplay Systems

---

---

---

# Pacman (Unreal Engine C++)


| | | |
|:-:|:-:|:-:|
| <img src="https://github.com/user-attachments/assets/355a6af1-1c72-4024-8885-eec857f2d564" width="300" /> | <img src="https://github.com/user-attachments/assets/12325bf2-9894-4c83-9d65-fe7178520231" width="300" /> | <img src="https://github.com/user-attachments/assets/f9cca475-aab5-4280-9146-e6084e7d3c48" width="300" /> |




*Arcade machine integration (left), in-game HUD and board state (center/right).*

A modular **Pacman gameplay system** built in **Unreal Engine (C++)**, designed with a  **clear ownership**, **data-driven rules**, and **event-based communication**.

---

## Overview

- A **central gameplay authority** owns global state (**score**, **lives**, **ghost modes**, **game flow**), while actors react through **delegates** instead of direct dependencies.
- Player and AI entities share a **common base**, with **grid-based movement** implemented as a reusable **gameplay component**.
- Ghost behavior follows classic Pacman rules (*Scatter / Chase / Frightened*), implemented via **timed state transitions** and **grid-based targeting** (not per-frame scripting).
- Level layout and collectibles are **data-driven**, separating **board definition** from **gameplay logic**.
- UI and world interaction are driven entirely by **gameplay events**, keeping **presentation** independent from core systems.

## What I Learned

- Designing gameplay systems with **clear responsibility boundaries** between **game state**, **actors**, and **components**.
- Implementing classic AI behavior in a maintainable way using **state machines**, **timers**, and **grid-based reasoning**.
- Choosing **delegates over direct references** to keep gameplay systems **flexible** and more **testable**.
- Understanding how Unreal’s gameplay lifecycle (**BeginPlay**, timers, restart/quit flow) affects system design.
- Favoring **clarity and correctness** in gameplay code over over-engineering.


---

---

---


# Inventory & Equipment System (Unreal Engine C++)


| | | |
|:-:|:-:|:-:|
| <img src="https://github.com/user-attachments/assets/0bd55781-4334-4958-bd8d-9648aef3b063" width="300" /> | <img src="https://github.com/user-attachments/assets/9e383b14-2201-4c58-bede-aa96bba944b7" width="300" /> | <img src="https://github.com/user-attachments/assets/68d0fc09-b906-40b3-a9c1-f3bf0f74d350" width="300" /> |



Gameplay logic lives entirely in **Actor Components**, while the UI layer acts as a **reactive client** that never mutates gameplay state directly.

---

## Key Design Highlights

- **InventoryComponent** acts as the single source of truth for item storage, stacking, weight limits, and consumption logic.
- **EquipmentComponent** owns equipped items, validates equipment rules, and applies visual and gameplay effects.
- Inventory ↔ Equipment interaction is handled through **explicit APIs and delegates**, avoiding circular dependencies.
- Items are **data-driven via DataTables**, supporting stack sizes, equip types, consumables, visuals, and attributes.
- World items use a shared **Interactable system**, keeping pickup logic decoupled from inventory internals.
- Complex interactions (drag & drop, splitting stacks, swapping, merging) are implemented as **stateful gameplay flows**, not UI hacks.

---

## UI & HUD Architecture

- The UI layer is fully **event-driven**:
  - Widgets listen to gameplay delegates instead of polling or owning data.
- A dedicated **Inventory Screen Orchestrator** coordinates:
  - inventory grid  
  - equipment slots  
  - context menus  
  - split dialogs  
  - drag visuals
- Drag & drop, right-click actions, and modals are handled **without direct gameplay mutation**.
- Equipment and consumable effects update HUD elements through **gameplay events**, not widget-to-widget coupling.
- Attributes, buffs, and timed effects are visualized via **reusable HUD widgets** driven by gameplay state.

  ## What I Learned

- How to integrate **UMG with C++ gameplay systems** in a way that preserves clear ownership and avoids UI-driven logic.
- How to use **delegates as the contract between gameplay and UI**, allowing widgets to react to state changes without directly modifying gameplay data.
- How to structure large UMG systems using **orchestrator widgets**, preventing tight coupling between individual UI elements.
- How to handle complex player interactions (drag & drop, split stacks, context menus) as **deterministic gameplay flows**, not ad-hoc UI behavior.
- How to balance feature-rich UI with **maintainable gameplay architecture** in Unreal Engine.
