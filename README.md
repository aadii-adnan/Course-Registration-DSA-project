# 🎓 Course Registration System DSA Project

A comprehensive console-based application designed to simulate a real-world university course enrollment system.

This project demonstrates the practical application of **Data Structures and Algorithms (DSA)** for handling complex operations like prerequisite validation, waitlist prioritization, and efficient data management.

## 🚀 Features

### 1. Student & Course Management
* **Student Registration:** Add students with validated IDs, names, departments, and GPA.
* **Search Functionality:** Retrieve students by ID or department.
* **Course Creation:** Add courses with defined capacity limits.
* **Prerequisite Assignment:** Link courses with dependency requirements.

### 2. Enrollment Logic & Validation
* **Smart Enrollment System:** Handles add/drop operations efficiently.
* **Prerequisite Enforcement:** Ensures students meet course requirements before enrollment.
* **Cycle Detection:** Prevents invalid circular dependencies using recursive graph logic.

### 3. Waitlist & Priority Handling
* **GPA-Based Waitlist:** Automatically activates when course capacity is full.
* **Max-Heap Implementation:** Ensures highest GPA students get priority enrollment.

### 4. Action Tracking (Undo/Redo)
* **Undo Operations:** Revert last enrollment or drop action.
* **Redo Operations:** Reapply reverted actions.
* **Stack-Based Tracking:** Maintains action history efficiently.

### 5. Data Persistence
* **Binary File Storage:** Efficient saving/loading of system data.
* **Text File Logs:** Maintains readable records for auditing.

## 🛠 Technical Stack

* **Language:** C++
* **Core Focus:** Data Structures & Algorithms
* **Data Structures Used:**
    * **AVL Trees:** Balanced course storage for $O(\log n)$ search time.
    * **Max-Heaps:** Priority waitlist management.
    * **Stacks:** Undo/Redo system history tracking.
    * **Linked Lists:** Student records and enrollment links.
* **Concepts Applied:**
    * Tree rotations (LL, RR, LR, RL)
    * Heap operations (Insert, Extract-Max)
    * Graph traversal (Cycle Detection)

## 👥 Contributors

* **Noor Fatima (Leader)** – Core structures, file handling, prerequisite logic, debugging
* **Adnan Haider** – AVL tree implementation, undo/redo stacks, CLI system
* **Areeba Kanwal** – Enrollment logic, heap-based waitlist, capacity handling
* **Musfirah Imran** – Student management, search features, GPA updates

## 📈 Future Improvements

* **GUI-based system:** Transition to a desktop or web interface.
* **Role-based access:** Separate dashboards for Admin and Student views.
* **Automatic timetable conflict detection:** Prevent overlapping schedules.
* **CSV import/export support:** For easier bulk data management.

---
**Developed for Semester Project – COMSATS University Islamabad**
