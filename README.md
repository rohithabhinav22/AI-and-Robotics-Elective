# Autonomous 2-Player Racing Simulation in Webots

## Overview

This project is a **2-player racing simulation** in Webots, where two manually controlled cars race on a track using real-time position data. One car is controlled by **WASD** keys, the other by **Arrow** keys. A supervisor (simulated as an e-puck or supervisor node) tracks crossing of the finish line, records times, and announces the winner. The system is designed for easy extension to support advanced racing rules, time penalties, and collision/safety logic.

---

## Final Simulation System

### Key Components

- **Manual Controls:**  
    - Player 1: WASD keys  
    - Player 2: Arrow keys  
- **GPS Position Tracking:**  
    - Each robot provides real-time position via onboard GPS or supervisor node lookups.
    - The simulation checks whether a robot has crossed a predefined finish line (`WHITE_LINE_X` coordinate) to high precision.
    - Crossing triggers time recording.
- **Supervisor Robot/Node:**  
    - An e-puck (or supervisor node) maintains race logic, tracks start/finish times, and prints results to the console.
    - Easily extendable to add advanced features (time penalties, safety events, etc).
- **Controller Language:**  
    - **C** is used for all robot controllers for highest compatibility with Webots’ automobile plugins and reliable, low-level performance.
    - C enables direct actuator access, robust sensor handling, and is less error-prone than Python for such integrations.

---

## Game Flow (How It Works)

1. Both players control their robots in real-time via keyboard (WASD vs Arrows).
2. Each robot's GPS position is tracked every simulation step.
3. When a robot crosses the finish line (specified X coordinate), the supervisor records the time and prints it.
4. When both robots finish, the supervisor announces individual times and the winner in the console.

---

## Supervisor’s Role & Future Extensions

- Monitors position of both cars.
- Tracks when cars start and finish the race.
- Handles all race timing.
- Prints start/finish notifications and winner summary.
- **Ready for future upgrades:** lap counters, collisions, safety car, penalties.

---

## Evolution of Project & Design Choices

### 1. Early Proposal: Automated Shopping Mall  
- Planned autonomous shopping carts with robotic arms.
- **Major challenges:** Required advanced manipulator control, item pickup, obstacle avoidance. Too ambitious for current time and skills.

### 2. Pivot: F1-Style Autonomous Racing
- Moved focus to autonomous cars competing for first position, with safety cars for collisions.
- **Challenges faced:** Developing robust pathfinding, overtaking logic, and collision management with Webots APIs. Multi-agent logic became overly complex.

### 3. Final Scope: 2-Player Manual Racing
- Practical, focused approach to finish reliably and demonstrate clear engineering.
- Essential infra (manual controls, supervisor, GPS, timing) built in C for reliability.
- Model allows easy swap between manual and autonomous logic.

---

## **Difficulties Faced & Lessons Learned**

### Controller File complocation: Python vs C

- **Problem:** Initial controller attempts used `.py` files , causing slow compilation and crashing.
- **Solution:** Use `.c` files for C code.
  
### Controller Not Found / Not Compiled

- **Problem:** Errors like "Could not find controller file: Expected wasd.exe..." meant Webots didn’t find compiled executables.
- **Solution:** Used Webots' “Build” system to compile all C controllers. Verified `.exe` existence in folders before simulation.

### Linker Errors (Undefined Reference)

- **Problem:** Build broke with errors like `undefined reference to 'wbu_driver_set_cruising_speed'`.
- **Cause:** Missing link to the Webots vehicle driver library.
- **Solution:** Edited Makefile to add `wbu_driver` to the target libraries:
WEBOTS_LIBRARIES = wbu_driver

then rebuilt the controllers.

### DLL/Plugin Not Found

- **Problem:** Errors loading `.dll` libraries (vehicle plugins, remote control plugins).
- **Solution:** Reinstalled Webots, verified correct architecture (64-bit), restored missing plugins.

### Supervisor "DEF Name" Error

- **Problem:** Supervisor script couldn't find cars by DEF name, leading to messages like "One or both car DEF names are incorrect or missing!"
- **Solution:** In Webots world editor, set the DEF name on each robot node to exactly match the supervisor code (`Rohith Car` and `Radha Krishna Car`).

---

## Lessons & Best Practices

- Match file extensions to language (no C in `.py` or vice versa).
- Always **build** (compile) C controllers before running simulation.
- Download all the required libraries, then itself the code works
- Link extra libraries required for car control.
- Name all nodes consistently and double-check controller selection in the world file.
- Use documentation, methodical debugging, and file structure discipline for large simulation projects.

---

## For Faculty

- This project demonstrates a functional simulation and careful step-by-step engineering, even through major pivots.
- The design allows future students to add more automation, players, or advanced race rules with minimal refactor.
- Documentation and troubleshooting logs are included for transparency and ongoing learning.
- Might have to install extra libraries to work it.

---

**Thank you for evaluating our work!**
