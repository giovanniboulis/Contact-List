# Contact Management System
### CSE333 – Data Structures and Algorithms | Group Project – Spring 2026

---

## Group Members

| Name | ID |
|------|----|
| George Ibrahim Abdo| [2300285] |
| [Giovanni boulis abdo] | [2300063] |
| [Ali said Abdelhamid Khalil ] | [2300227] |
| [MOHAMED ELSAYED MOUSTAFA HAMOUDA] | [2200095] |
| [Adham Ahmed Hussein Abdelaal] | [2100725] |

---

## Project Description

A fully functional Contact Management System implemented in C++ that allows users to store, search, update, and delete contact information including name, phone number, email, address, and group category. The system is available in two versions:

- **Console version** — runs in the terminal with a menu-driven interface
- **GUI version (Bonus)** — a modern desktop application built with Qt6

The project demonstrates meaningful use of multiple data structures and algorithms covered in CSE333, with every core operation justified by its time complexity.

---

## Data Structures Used and Why

### 1. Hash Table (`HashTable.h`)
**Why:** The Hash Table is the primary storage structure for all contacts. It uses the contact's name as a unique key and the **djb2 hash function** to map names to bucket indices. This gives:
- Insert → **O(1) average**
- Search by name → **O(1) average**
- Delete → **O(1) average**

Collisions are handled using **separate chaining** (each bucket holds a linked list). This is far more efficient than a simple array or linked list, which would require O(n) for search and delete.

### 2. Stack (`UndoStack.h`)
**Why:** A Stack (Last In, First Out) is used to implement the undo feature for deleted contacts. Every time a contact is deleted, it is pushed onto the stack. If the user wants to undo, the most recently deleted contact is popped and restored. Both push and pop are **O(1)**, making this a natural and efficient fit for undo functionality.

### 3. Vector + Binary Search (`BinarySearch.h`)
**Why:** When searching by phone or email (fields that are not the hash key), we collect all contacts into a vector, sort it by the target field using `std::sort` (**O(n log n)**), and apply binary search (**O(log n)**). This is significantly faster than a linear scan for large datasets and demonstrates the divide-and-conquer paradigm. Vectors are also used for the sorted display feature (A–Z).

---

## Implemented Features

1. **Add Contact** — inserts a new contact into the Hash Table (O(1) average)
2. **Search by Name** — direct Hash Table lookup (O(1) average)
3. **Search by Phone** — binary search on sorted vector (O(n log n))
4. **Search by Email** — binary search on sorted vector (O(n log n))
5. **Update Contact** — in-place update via Hash Table (O(1) average)
6. **Delete Contact** — removes from Hash Table and pushes to undo stack (O(1))
7. **Undo Last Delete** — pops from stack and re-inserts into Hash Table (O(1))
8. **Display All Contacts (Sorted A–Z)** — sorted copy using std::sort (O(n log n))
9. **Filter by Group** — linear scan returning contacts in a given group (O(n))
10. **GUI Interface (Bonus)** — full Qt6 desktop GUI with sidebar navigation, tables, forms, and live filter

---

## How to Compile and Run

### Console Version

**Requirements:** g++ with C++17 support

```bash
g++ -std=c++17 -o contacts main_console.cpp
./contacts        # Linux/macOS
contacts.exe      # Windows
```

### GUI Version (Bonus)

**Requirements:** Qt6 installed with MinGW 64-bit, Qt Creator

**Using Qt Creator (recommended on Windows):**
1. Open Qt Creator
2. Click File → Open File or Project
3. Select `ContactManager.pro`
4. Click Configure Project
5. Press the green ▶ Run button

**Using command line (Linux):**
```bash
sudo apt install qtbase5-dev qt5-qmake
qmake ContactManager.pro
make
./ContactManager
```

### Project Files Structure

```
Project/
├── Contact.h           → Contact data structure (name, phone, email, address, group)
├── HashTable.h         → Hash Table with separate chaining
├── UndoStack.h         → Stack-based undo for delete operations
├── BinarySearch.h      → Binary search and sorting utilities
├── ContactManager.h    → Facade connecting all data structures
├── main_console.cpp    → Console version entry point
├── MainWindow.h        → Qt GUI window declaration
├── MainWindow.cpp      → Qt GUI window implementation
├── main.cpp            → Qt GUI entry point
└── ContactManager.pro  → Qt build file
```

---

## Time Complexity Summary

| Operation | Data Structure Used | Time Complexity |
|---|---|---|
| Add contact | Hash Table insert | O(1) average |
| Search by name | Hash Table search | O(1) average |
| Search by phone | Sort + Binary Search | O(n log n) |
| Search by email | Sort + Binary Search | O(n log n) |
| Update contact | Hash Table update | O(1) average |
| Delete contact | Hash Table remove + Stack push | O(1) |
| Undo delete | Stack pop + Hash Table insert | O(1) |
| Display all (A–Z) | std::sort on vector | O(n log n) |
| Filter by group | Linear scan | O(n) |

---

## AI Usage Declaration

### Tools Used
- **Claude (claude.ai)** by Anthropic

### What Claude Was Used For

- **GUI implementation** — The Qt6 GUI layer (`MainWindow.h`, `MainWindow.cpp`, and the Qt `main.cpp`) was generated with the help of Claude. The group provided the existing backend files and requested a clean Qt desktop interface that connects to the existing ContactManager without modifying any backend logic.

- **Code structure advice** — Claude suggested separating concerns into individual header files (one per data structure), which improved readability and modularity. The group reviewed and agreed with this approach.

- **Debugging assistance** — Claude helped identify a missing method declaration in `MainWindow.h` that caused a linker error during compilation, and helped resolve Qt kit configuration issues during setup on Windows.

- **README and documentation** — This README was written with Claude's assistance based on the group's actual project content.

- **Learning and brainstorming** — Claude was used to explain concepts such as separate chaining, the djb2 hash function, and the trade-offs between binary search and linear scan when the primary key is not available.

### What Was Modified or Rejected

- The initial GUI suggestion from Claude used an HTML/JavaScript approach, which the group rejected in favor of a proper Qt C++ desktop application. Claude then produced the correct Qt implementation.

- Some AI-suggested variable names and layout choices were adjusted by the group to better match the existing code style and naming conventions in the backend.

### Example Where AI Output Was Not Fully Correct

During GUI generation, Claude declared several private methods in `MainWindow.h` but omitted `buildPageSearch()`, `buildPageUpdate()`, and `buildPageUndo()` from the declarations. This caused compilation errors. The group identified the missing declarations by reading the error output and Claude corrected them in the next iteration.

### What the Group Understood and Implemented Themselves

- All five core data structure files (`HashTable.h`, `UndoStack.h`, `BinarySearch.h`, `Contact.h`, `ContactManager.h`) were written by the group
- The console version (`main_console.cpp`) was fully written by the group
- The group understands the djb2 hash function, separate chaining, undo stack logic, and binary search implementation in detail
- Every design decision — choice of hash table as primary structure, stack for undo, binary search for secondary fields — was made and justified by the group

> Using AI tools did not reduce our understanding of the project. Every AI-generated component was reviewed, tested, and understood before submission.