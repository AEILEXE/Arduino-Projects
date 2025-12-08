######

# Arduino-Projects

## Overview

This repository contains a collection of Arduino-based projects demonstrating practical applications of microcontroller programming. The projects cover various topics including sensor usage, LCD displays, real-time clocks, input devices, and interactive systems. Each folder includes source code and required files for running the respective project on an Arduino board.

The repository serves as a reference for learning, experimentation, and future development of embedded systems.

---

## Included Projects

The repository contains multiple independent Arduino projects, such as:

* LCD interface and option selection systems
* Temperature sensing using thermistors and display modules
* Real-Time Clock (RTC) based clock systems
* Joystick-based interactive games
* Final or capstone Arduino builds
* Additional experiments and activity-based exercises

Each project is self-contained and includes its own `.ino` files and supporting resources.

---

## Technologies and Requirements

### Software

* Arduino IDE
* C/C++ for embedded programming
* Necessary Arduino libraries (some included in the `libraries/` folder)

### Hardware

Depending on the project, the following modules may be required:

* Arduino Uno or compatible board
* Temperature sensors (thermistors)
* LCD modules (16x2 or similar)
* RTC modules (e.g., DS3231)
* Joysticks or input controllers
* Basic electronic components (resistors, wires, breadboard)

---

## Repository Structure

```
Arduino-Projects/
│
├── RECTO_ACTIVITY4_LCDOPTIONS/
├── RECTO_ACTIVITY5/
├── RECTO_ACTIVITY6_TEMPERATURE_SENSOR_.../
├── RECTO_ACTIVITY7_TEMPERATURE_WITH_LCD/
├── RECTO_ACTIVITY8_ArduinoClockUsingRTCModule/
├── RECTO_ACTIVITY9_ArduinoClockUsingRTCModulewithLCD/
├── RECTO_Activity11ArduinoGameUsingJoystick/
├── RECTO_FINAL_PROJECT/
├── libraries/
└── README.md
```

Each folder represents a standalone project and contains the relevant `.ino` files and supporting assets.

---

## Usage and Setup

1. Clone the repository:

   ```
   git clone https://github.com/AEILEXE/Arduino-Projects.git
   ```
2. Open the project folder of your choice.
3. Launch the `.ino` file within the Arduino IDE.
4. Connect your Arduino board to your computer.
5. Ensure required hardware components for that project are properly connected.
6. Compile and upload the sketch to the board.
7. Follow project-specific instructions or monitor output via Serial Monitor.

---

## Recommendations

* Review the wiring and component requirements before uploading code.
* Install any necessary libraries included in the `libraries/` folder.
* For sensor and LCD projects, verify pin connections to avoid incorrect readings.
* Reset the Arduino device when switching between multiple projects.

---

## Contributing

Contributions to this repository are welcome. Suggested contribution workflow:

1. Fork the repository.
2. Create a feature branch:

   ```
   git checkout -b feature/new-project
   ```
3. Add your project folder or enhancements.
4. Commit your changes with a clear description:

   ```
   git commit -m "Added new Arduino project"
   ```
5. Push your branch and submit a Pull Request.

---
