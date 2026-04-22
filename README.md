# Terminal Text Editor (ncurses)

A lightweight, terminal-based text editor built in C. This project utilizes a **doubly linked list** for efficient buffer management and the **ncurses** library for a responsive terminal-based user interface.

## 🎮 Command Interface
The editor uses a dual-mode system to prevent accidental text entry during navigation.

### **Command Mode (Default)**
| Key | Action |
| :--- | :--- |
| `i` | **Enter Insert Mode** (Start typing) |
| `d` | **Delete** the currently selected line |
| `s` | **Save** the current buffer to a file |
| `l` | **Load** an existing file into the empty editor |
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

### **Compile the program and Execute the editor:**
```bash
make run
```
### **Clean build files:**
```bash
make clean
```
## 🚀 Technical Highlights

* Doubly Linked List: Each line of text is stored as a node in a doubly linked list, allowing for $O(1)$ line insertions and removals regardless of file size.
* Optimized Mode Switching: Implemented set_escdelay(15) and advanced input handling to ensure the ESC key provides an instant, lag-free transition from Insert to Command mode.
* Custom UI Windows: Uses newwin() for a dedicated status and menu bar, ensuring the text buffer remains visually distinct from the editor controls.
* Efficient Redrawing: Utilizes wnoutrefresh() and doupdate() to minimize terminal flicker and improve performance on native Linux environments.
* Terminal Restoration: Robust handling of endwin() to ensure the user's terminal environment is perfectly restored upon exiting the program.

---

## 📝 Usage Notes
> [!IMPORTANT]
> **Mode Switching & Navigation:**
> * **Cursor Navigation:** Moving the cursor up and down (`↑` / `↓`) is only enabled in **Command Mode**. You must press **`ESC`** to exit Insert Mode (or any other active function) before you can navigate between lines.
> * **Vertical Constraints:** The editor currently displays text up to the **available terminal height (excluding the status bar)**. Vertical scrolling for files that exceed the current window height is not yet implemented.
> * **Active Buffer Constraints:** While a line is active in Insert Mode, the only special characters explicitly handled are **Backspace** (for deletion) and **Escape** (to exit). 
> * **Input Handling:** Other control sequences or non-alphanumeric keys may not render correctly while the buffer is active.

#### Developed by P Prethusha
