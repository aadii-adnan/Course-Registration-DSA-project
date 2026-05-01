# Course-Registration-DSA-project

[cite_start]A robust, Command-Line Interface (CLI) based application designed to simulate a real-world university registration environment[cite: 23, 24]. [cite_start]This system manages student enrollments, course prerequisites with cycle detection, and GPA-based waitlists using advanced Data Structures and Algorithms (DSA)[cite: 24, 25].

## 🚀 Key Features

* [cite_start]**Student Management**: Register students with validated IDs, update GPAs, and view enrolled courses[cite: 32, 34, 35].
* [cite_start]**Course Management**: Add courses with capacity limits and assign prerequisites[cite: 37, 38].
* [cite_start]**Cycle Detection**: Employs recursive logic to detect and prevent circular dependencies in prerequisites[cite: 38, 61].
* [cite_start]**GPA-Priority Waitlist**: Students are added to a Max-Heap waitlist when courses are full; they are automatically enrolled once slots are freed[cite: 42, 43, 56].
* [cite_start]**Undo/Redo Functionality**: Tracks enrollment and drop actions using stack-based tracking for easy rollback[cite: 45, 46, 57].
* [cite_start]**Data Persistence**: Features binary file storage for efficiency and text file outputs for auditing[cite: 51, 52].

## 🛠 Technical Architecture

This project leverages specific data structures to solve complex registration problems:
* [cite_start]**AVL Trees**: Manages and balances course records to ensure $O(\log n)$ search and insertion[cite: 27, 55].
* [cite_start]**Max-Heaps**: Ensures the waitlist always prioritizes students with the highest GPA[cite: 28, 64].
* [cite_start]**Stacks**: Power the Undo/Redo features by tracking the history of user actions[cite: 29, 45].
* [cite_start]**Linked Lists**: Used for the efficient management of student records[cite: 26].

## 👥 Contributors

This project was developed by:

| Member | Responsibilities |
| :--- | :--- |
| **Noor Fatima** (Leader) | [cite_start]Core structs, file handling, prerequisite cycle detection, and final debugging[cite: 72]. |
| **Adnan Haider** | [cite_start]AVL Tree logic (balancing LL, RR, LR, RL), Undo/Redo stack integration, and CLI Main Menu[cite: 72]. |
| **Areeba Kanwal** | [cite_start]Add/Drop logic, Max-Heap waitlist integration, and enrollment debugging[cite: 72]. |
| **Musfirah Imran** | [cite_start]Student registration, ID/Department search, and waitlist/GPA update functionality[cite: 72]. |

## 📈 Future Enhancements

* [cite_start]Development of a Graphical User Interface (GUI)[cite: 67].
* [cite_start]Implementation of Admin vs. Student login roles[cite: 68].
* [cite_start]Course schedule conflict detection[cite: 69].
* [cite_start]Support for CSV data import/export[cite: 70].

