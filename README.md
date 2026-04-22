# Terminal Text Editor (ncurses)

A lightweight, terminal-based text editor built in C. This project utilizes a **doubly linked list** for efficient buffer management and the **ncurses** library for a responsive terminal-based user interface.

## 📺 Project Demo
*If viewing on GitHub, the video below demonstrates mode-switching, live typing, and file persistence.*

![Project Demo](demo.mp4)

---

## 🎮 Command Interface
The editor uses a dual-mode system to prevent accidental text entry during navigation.

### **Command Mode (Default)**
| Key | Action |
| :--- | :--- |
| `i` | **Enter Insert Mode** (Start typing) |
| `d` | **Delete** the currently selected line |
| `s` | **Save** the current buffer to a file |
| `l` | **Load** an existing file into the editor |
| `h` | **Search & Highlight** specific text |
| `↑ / ↓` | Navigate through lines |
| `q` | **Quit** the application |

### **Insert Mode**
| Key | Action |
| :--- | :--- |
| **`ESC`** | **Exit Insert Mode** (Return to Menu) |
| **`Enter`** | Commit the buffer and start a new line |
| **`Backspace`** | Delete characters within the active line |

---

## 🛠 Prerequisites & Installation

To ensure the project compiles correctly on a native Linux or macOS system, the `ncurses` development headers must be installed.

### **For Linux (Ubuntu/Debian/Linux Mint)**
```bash
sudo apt update
sudo apt install libncurses-dev libncursesw5-dev
```
### **For macOS**
```bash
brew install ncurses
```
## 🏗 Build & Run Instructions

A Makefile is included to simplify the compilation and linking process.

### **Compile the program and Execute the editor:
```bash
make run
```
### **Clean build files:
```bash
make clean
```
## 🚀 Technical Highlights

Doubly Linked List: Each line of text is stored as a node in a doubly linked list, allowing for $O(1)$ line insertions and removals regardless of file size.
Optimized Mode Switching: Implemented set_escdelay(15) and advanced input handling to ensure the ESC key provides an instant, lag-free transition from Insert to Command mode.
Custom UI Windows: Uses newwin() for a dedicated status and menu bar, ensuring the text buffer remains visually distinct from the editor controls.
Efficient Redrawing: Utilizes wnoutrefresh() and doupdate() to minimize terminal flicker and improve performance on native Linux environments.
Terminal Restoration: Robust handling of endwin() to ensure the user's terminal environment is perfectly restored upon exiting the program.

## NOTE:

Currently, moving cursor up and down works only after using ESC and exitting out of the other modes

# Developed by P Prethusha
