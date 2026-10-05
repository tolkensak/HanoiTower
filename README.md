# <img src="icon.png" alt="" width="28"> HanoiTower

> **Classic Tower of Hanoi puzzle — visualized in C and C++.**

The Tower of Hanoi is a classic mathematical puzzle. This project implements it in two versions:
- **MFC (C++)** — 2001
- **WinAPI (C)** — 2005

**Tech Stack:** C, C++, WinAPI, MFC

**Created:** 2001 (MFC), 2005 (C)

<br />

## Implementations

### MFC Version (C++)

Built with **C++ and MFC**. Created in 2001.

![Screenshot: HanoiTower (MFC)](screenshots/MFC-version.png "HanoiTower (MFC)")

### C Version (WinAPI)

Built with **C and Windows API**. Created in 2005.

![Screenshot: HanoiTower (C)](screenshots/C-version.png "HanoiTower (C)")

<br />

## About the Puzzle
The Tower of Hanoi consists of three pegs and a number of disks of
different sizes. The goal is to move the entire stack to another peg,
following these rules:
1. Only one disk can be moved at a time.
2. Each move consists of taking the upper disk from one stack and placing it on another stack.
3. No disk may be placed on top of a smaller disk.

<br />

## Overview

This project demonstrates two different approaches to Windows GUI programming:
- **WinAPI** — low-level Windows programming in pure C
- **MFC** — Microsoft Foundation Classes in C++

Both versions solve the same mathematical puzzle and render solutions graphically.

<br />

## Project Structure
```
/
├── HanoiTower (C)/     — C + WinAPI implementation
├── HanoiTower (MFC)/   — C++ + MFC implementation
└── screenshots/    — Application screenshots
```

<br />

## Requirements

- **Windows**
- **Microsoft Visual Studio**
- **MFC** (for MFC version)

<br />

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.