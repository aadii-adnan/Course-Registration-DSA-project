🎓 Course Registration System DSA Project

A robust Command-Line Interface (CLI) based application developed in C++ to simulate a real-world university course registration system.
This project applies advanced Data Structures and Algorithms (DSA) to efficiently manage student enrollments, prerequisites, and GPA-based waitlists.

🚀 Key Features
👨‍🎓 Student Management
Register students with validated IDs, names, and departments
Search students by ID or department
Update student GPA
📚 Course Management
Add courses with defined capacity limits
Assign prerequisites to courses
🔄 Cycle Detection
Prevents circular dependencies in prerequisites
Uses recursive logic to ensure valid course structures
📊 GPA-Priority Waitlist
Automatically activates when course capacity is full
Uses a Max Heap to prioritize students with higher GPA
↩️ Undo / Redo Functionality
Tracks enrollment and drop actions
Stack-based implementation for rollback operations
💾 Data Persistence
Binary files for efficient storage
Text files for logs and auditing
🛠 Technical Architecture

This system is built using the following data structures:

AVL Trees
Maintains balanced course records
Supports LL, RR, LR, RL rotations
Max Heaps
Manages waitlists with GPA priority
Stacks
Enables Undo/Redo functionality
Linked Lists
Stores and manages student records efficiently
👥 Contributors
Member	Responsibilities
Noor Fatima (Leader)	Core structures, file handling, prerequisite logic, cycle detection, debugging
Adnan Haider	AVL tree implementation, stack-based undo/redo, CLI system
Areeba Kanwal	Add/drop logic, heap-based waitlist, capacity management
Musfirah Imran	Student registration, search features, GPA updates
📈 Future Enhancements
GUI-based interface
Role-based access (Admin / Student)
Automatic schedule conflict detection
CSV import/export support
