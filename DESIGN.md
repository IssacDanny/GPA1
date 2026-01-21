
<img width="865" height="627" alt="image" src="https://github.com/user-attachments/assets/303738f4-b850-4156-890c-ba50383c1e0f" />


### Design Philosophy & Architecture

The **Whack-a-Zombie** project is engineered as a **Modular Monolith**. This architecture consolidates the application into a single executable unit while enforcing strict boundaries between internal subsystems. The design prioritizes **Separation of Concerns** and **Data-Driven Design** to ensure maintainability and scalability.

The system is composed of five primary architectural components that interact through defined interfaces.

#### 1. The Application Root (`GameEngine`)
*   **Role:** The Central Controller and State Container.
*   **Function:** This class manages the application lifecycle. It owns the operating system window, the main game loop, and the global game state (Score, Misses).
*   **Interaction:** It acts as the hub for data exchange. It captures raw input from the hardware, calculates the frame delta time (`dt`), and orchestrates the execution order of the subsystems. It does not contain game logic; it delegates logic to specific managers.

#### 2. The Asset Repository (`ResourceManager`)
*   **Role:** Centralized Memory Management.
*   **Function:** Implements a variation of the **Flyweight Pattern**. It loads heavy resources (textures, audio buffers, fonts) from the disk into memory exactly once. It stores these assets in associative containers (`std::map`).
*   **Interaction:** Other components request assets via string keys (e.g., "zombie_head"). The `ResourceManager` returns a lightweight reference (pointer) to the existing asset. This prevents memory duplication and reduces GPU bandwidth usage.

#### 3. The Domain Controller (`ZombieManager`)
*   **Role:** Entity Lifecycle and Logic Management.
*   **Function:** This component manages a collection of game entities. It utilizes **Object Pooling** (initializing entities once and toggling their active state) rather than dynamic allocation/deallocation, which minimizes memory fragmentation and CPU overhead.
*   **Interaction:**
    *   **Spawning:** It monitors time accumulation and activates inactive entities at specific coordinates.
    *   **Collision:** It receives input coordinates from the `GameEngine`, iterates through active entities, and performs spatial queries (hitbox checks).
    *   **Aggregation:** It aggregates frame-specific data (e.g., number of missed zombies) and returns this to the `GameEngine` to update the global state.

#### 4. The Entity Model (`StandardZombie`)
*   **Role:** Encapsulated State Machine.
*   **Function:** Represents a single interactive object. It implements a **Finite State Machine (FSM)** with four distinct states: `Hidden`, `Spawning`, `Active`, and `Hit`.
*   **Interaction:** It is self-contained. It updates its own internal timers and animation interpolation based on the `deltaTime` provided by the Manager. It exposes a `TryHit(x, y)` method that performs a local bounding-box check against the provided coordinates.

#### 5. The Presentation Layer (`HUD`)
*   **Role:** Data Visualization.
*   **Function:** Renders the game state to the user interface.
*   **Interaction:** It adheres to the **Interface Segregation Principle**. It accesses the `GameEngine` solely through the `ITelemetry` interface, which grants read-only access to statistics (`GetScore`, `GetAccuracy`). This ensures the UI cannot inadvertently modify the game state.

### System Interaction: The Execution Flow

The "Whole" is formed by the sequential execution of these parts within the **Game Loop**.

**1. Initialization Phase (Dependency Injection)**
The `GameEngine` initializes the `ResourceManager` first. It then injects dependencies by passing pointers of the `ResourceManager` (as `IAssetSupplier`) to the `ZombieManager` and `HUD`. This ensures all components have access to shared resources without global variables.

**2. Input Phase (Event Propagation)**
*   The `GameEngine` polls the OS for input events.
*   When a mouse click occurs, the Engine converts the **Screen Coordinates** (pixels) to **World Coordinates** (game units) using `mapPixelToCoords`.
*   These normalized coordinates are passed to `ZombieManager::HandleClick`.
*   The Manager queries the `StandardZombie` entities. If a hit is confirmed, the entity changes its internal state to `Hit`, and the Manager reports a success boolean back to the Engine.

**3. Update Phase (State Evolution)**
*   The `GameEngine` calculates `deltaTime` (time elapsed since the last frame).
*   It calls `ZombieManager::Update(deltaTime)`.
*   The Manager propagates this time value to every active `StandardZombie`.
*   Each Zombie updates its animation and timers. If a Zombie's timer expires while `Active`, it resets itself and reports a "Miss" to the Manager.
*   The Manager sums these misses and returns the count to the Engine, which updates the global `m_misses` counter.

**4. Render Phase (Painter's Algorithm)**
*   The `GameEngine` clears the window.
*   It draws the background.
*   It calls `ZombieManager::Render`, which draws the graves (background layer) followed by the zombies (foreground layer).
*   It calls `HUD::Render`, which draws the text overlay on top of the scene.
*   The window displays the final composed frame.

### Summary

#### 1. The Central Nervous System (`GameEngine`)
*   **Role:** The Coordinator.
*   **Interaction:** It owns the window and the clock. It does not know *how* to draw a zombie or *how* to load a file. It simply delegates.
*   **The Loop:** Every frame, it shouts three commands:
    1.  "Process Inputs!" (Calls `ProcessEvents`)
    2.  "Update Logic!" (Calls `Update` on Managers)
    3.  "Draw Scene!" (Calls `Render` on Managers)

#### 2. The Supply Chain (`ResourceManager` & `IAssetSupplier`)
*   **Role:** The Assets Manager.
*   **The Problem:** If every Zombie loaded its own texture from the hard drive, the game would stutter and crash (memory bloat).
*   **The Interaction:**
    *   The `GameEngine` creates the `ResourceManager` **first**.
    *   It passes a pointer to this manager (disguised as the `IAssetSupplier` interface) to the `ZombieManager` and `HUD`.
    *   When a Zombie is born, it asks the Supplier: *"May I have the reference to the 'zombie_head' texture?"*
    *   The Supplier hands over a lightweight pointer. The heavy image stays in one place.

#### 3. The Workforce (`ZombieManager` & `IWhackable`)
*   **Role:** The Foreman and the Workers.
*   **The Interaction:**
    *   The `GameEngine` does not know what a `StandardZombie` is. It only talks to the `ZombieManager`.
    *   The `ZombieManager` holds a list of `IWhackable` pointers. This is the **Liskov Substitution Principle**.
    *   **Input Flow:** When you click:
        1.  `GameEngine` receives `(x, y)`.
        2.  Passes it to `ZombieManager::HandleClick`.
        3.  Manager iterates through the list and asks each worker: *"Did this hit you?"* (`TryHit`).
        4.  The Worker checks its own math and replies `true` or `false`.

#### 4. The Observer (`HUD` & `ITelemetry`)
*   **Role:** The Scribe.
*   **The Interaction:**
    *   The `HUD` needs to display the score, but we do not want the HUD to be able to *change* the score (e.g., `m_score = 9999`).
    *   **Interface Segregation:** The `GameEngine` hands the `HUD` a restricted pass called `ITelemetry`.
    *   Through this interface, the HUD can only ask `GetScore()`. It cannot touch the internal variables. This prevents accidental bugs where the UI breaks the game logic.

#### Gameflow loop:
The system forms a complete loop:
1. The player clicks the mouse.
2. The `GameEngine` detects the event. It translates the coordinates from "Screen Space" to "Stage Space."
3. The `GameEngine` passes the coordinates to The `ZombieManager`.
4. The `ZombieManager` asks each `StandardZombie` that is alive: "Are you standing at these coordinates?"
5. The `StandardZombie` checks Its Hitbox. It replies: "Yes!"
6. The `StandardZombie` immediately starts it Death Animation (changes state to Hit).
7. The `ZombieManager` reports back to the `GameEngine`: "Hit confirmed!"
8. The `GameEngine` adds +1 to the Score.
9. The `HUD` sees the score change and updates the display text.
