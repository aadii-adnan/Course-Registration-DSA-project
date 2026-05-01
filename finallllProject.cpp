#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

// ======================== Constants ========================
const int MAX_STUDENTS = 1000;
const int MAX_COURSES = 100;
const int MAX_COURSES_PER_STUDENT = 10;
const int MAX_STUDENTS_PER_COURSE = 100;
const int ID_LENGTH = 20;
const int NAME_LENGTH = 50;
const int DEPT_LENGTH = 30;
const int MAX_PREREQUISITES = 5;
const int MAX_WAITLIST_SIZE = 100;



// ======================== Data Structures ========================
struct WaitlistStudent {
    char id[ID_LENGTH];
    float gpa;
};


struct Course {
    char id[ID_LENGTH];
    char name[NAME_LENGTH];
    char department[DEPT_LENGTH];
    int capacity;
    int enrolledCount;
    char enrolledStudents[MAX_STUDENTS_PER_COURSE][ID_LENGTH];
    char prerequisites[MAX_PREREQUISITES][ID_LENGTH];
    int prereqCount;


    WaitlistStudent waitlist[MAX_WAITLIST_SIZE];
    int waitlistSize;

};


struct Student {
    char id[ID_LENGTH];
    char name[NAME_LENGTH];
    char department[DEPT_LENGTH];
    int courseCount;
    float gpa;
    char registeredCourses[MAX_COURSES_PER_STUDENT][ID_LENGTH];

    Student* next;
};


struct avlNode{
    Course data;
    int height;
    avlNode *left;
    avlNode *right;
};


struct Action {
    char type[10];
    char studentId[ID_LENGTH];
    char courseId[ID_LENGTH];
};



struct stackNode{
    Action action;
    stackNode *next;
};


void push(stackNode*& top, Action action) {
    stackNode* newNode = new stackNode;
    newNode->action = action;
    newNode->next = top;
    top = newNode;
}


bool pop (stackNode *&top, Action&action){
    if (top==nullptr){
        return false;
    }

    stackNode*temp = top;
    action = top->action;
    top = top->next;
    delete temp;
    return true;
}


// ======================== Global Data ========================
Student* studentHead = nullptr;
avlNode* courseRoot = nullptr;
int studentCount = 0;
int courseCount = 0;

stackNode* undoStack = nullptr;
stackNode* redoStack = nullptr;


//===================== Helper funcs =========================

void swapWaitlistStudents(WaitlistStudent& a, WaitlistStudent& b) {
    WaitlistStudent temp = a;
    a = b;
    b = temp;
}


bool isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlnum(char c) {
    return isAlpha(c) || isDigit(c);
}

bool isValidStudentId(const char* id) {
    int len = strlen(id);
    if (len < 5 || len >= ID_LENGTH) return false;
    bool hasLetter = false, hasNumber = false;
    for (int i = 0; i < len; ++i) {
        if (isAlpha(id[i])) hasLetter = true;
        else if (isDigit(id[i])) hasNumber = true;
        else if (id[i] != '-' && id[i] != '_') return false;
    }
    return hasLetter && hasNumber;
}

bool isValidCourseId(const char* id) {
    int len = strlen(id);
    if (len < 2 || len >= ID_LENGTH) return false;
    if (!isAlpha(id[0])) return false;
    for (int i = 1; i < len; ++i)
        if (!isAlnum(id[i]) && id[i] != '-' && id[i] != '_') return false;
    return true;
}

bool isValidDepartment(const char* dept) {
    int len = strlen(dept);
    if (len < 2 || len >= DEPT_LENGTH) return false;
    for (int i = 0; i < len; ++i)
        if (!isAlnum(dept[i]) && dept[i] != ' ' && dept[i] != '-') return false;
    return true;
}

bool isValidName(const char* name) {
    int len = strlen(name);
    if (len < 2 || len >= NAME_LENGTH) return false;
    if (!isAlpha(name[0])) return false;
    for (int i = 1; i < len; ++i)
        if (!isAlpha(name[i]) && name[i] != ' ' && name[i] != '-') return false;
    return true;
}

//bool studentExists(const char* id);

Student* findStudent(const char* id) {
    Student* current = studentHead;
    while (current) {
        if (strcmp(current->id, id) == 0) return current;
        current = current->next;
    }
    return nullptr;
}

bool studentExists(const char* id) {
    return findStudent(id) != nullptr;
}

bool hasPrerequisites(const Student* student, const Course& course) {
    bool meetsAll = true;

    for (int i = 0; i < course.prereqCount; ++i) {
        bool found = false;
        for (int j = 0; j < student->courseCount; ++j) {
            if (strcmp(student->registeredCourses[j], course.prerequisites[i]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            meetsAll = false;
            // No need to break here - we want to show all missing prerequisites
        }
    }

    return meetsAll;
}



//======================== Student LL funcs ==============================

void addStudentToList(Student* newStudent) {
    if (studentHead == nullptr) {
        studentHead = newStudent;
    }
    else {
        // Add to the end of the list
        Student* current = studentHead;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newStudent;
    }
    studentCount++;
}



void deleteStudentList() {
    Student* current = studentHead;
    while (current != nullptr) {
        Student* next = current->next;
        delete current;
        current = next;
    }
    studentHead = nullptr;
    studentCount = 0;
}

//==================================== Courses AVL funcs =================================


int getHeight (avlNode *root){
    return root? root->height : 0;
}

void updateHeight (avlNode *root){
     if (root){
        int hl = getHeight(root->left);
        int hr = getHeight (root->right);

        root->height = 1 + (hl > hr ? hl : hr);
     }
}

int getBalance (avlNode *root){
    int hl = getHeight(root->left);
    int hr = getHeight (root->right);
    return root? hl-hr : 0;
}

avlNode * rightRotate (avlNode* x){
    avlNode *y = x->left;
    avlNode *T2 = y->right;


    y->right = x;
    x->left = T2;

    updateHeight(x);
    updateHeight(y);

    return y;

}


avlNode * leftRotate (avlNode* y){
    avlNode *x = y->right;
    avlNode *T2 = x->left;


    x->left = y;
    y->right = T2;

    updateHeight(y);
    updateHeight(x);

    return x;

}


avlNode* insertCourseAVL(avlNode* root, Course course) {
    if (!root) {
        avlNode* node = new avlNode();
        node->data = course;
        node->height = 1;
        node->left = node->right = nullptr;
        courseCount++;
        return node;
    }




    int cmp = strcmp(course.id, root->data.id);
    if (cmp < 0) {
        root->left = insertCourseAVL(root->left, course);
    }
    else if (cmp > 0) {
        root->right = insertCourseAVL(root->right, course);
    }
    else {
        return root;
    }

    updateHeight(root);
    int balance = getBalance(root);

    // LL
    if (balance > 1 && strcmp(course.id, root->left->data.id) < 0)
        return rightRotate(root);

    // RR
    if (balance < -1 && strcmp(course.id, root->right->data.id) > 0)
        return leftRotate(root);

    // LR
    if (balance > 1 && strcmp(course.id, root->left->data.id) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RL
    if (balance < -1 && strcmp(course.id, root->right->data.id) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}




void inorderTraversal (avlNode *root){
    if (root){
        inorderTraversal(root->left);
        printf("Course ID: %s | Name: %s | Dept: %s | Enrolled: %d/%d\n",
        root->data.id, root->data.name, root->data.department,
        root->data.enrolledCount, root->data.capacity);
        inorderTraversal(root->right);
    }
}


avlNode* searchCourse (avlNode *root, const char*id){
    if(!root){
        return nullptr;
    }

    int cmp = strcmp(id,root->data.id);
    if (cmp==0) return root;
    else if (cmp<0){
        return searchCourse (root->left,id);
    }
    else
      return searchCourse (root->right,id);


}


avlNode* searchCourseAVL(avlNode* root, const char* id) {
    return searchCourse(root, id);
}


void deleteAVL(avlNode* root) {
    if (root) {
        deleteAVL(root->left);
        deleteAVL(root->right);
        delete root;
    }
}


bool courseExists(const char* courseId) {
    return searchCourseAVL(courseRoot, courseId) != nullptr;
}


// ========================= pre req helper func ============================

bool isPrerequisiteCycle(const char* startCourseId, const char* targetCourseId) {
    avlNode* startNode = searchCourseAVL(courseRoot, startCourseId);
    if (!startNode) return false;

    for (int i = 0; i < startNode->data.prereqCount; ++i) {
        const char* prereqId = startNode->data.prerequisites[i];
        if (strcmp(prereqId, targetCourseId) == 0) {
            return true;
        }
        if (isPrerequisiteCycle(prereqId, targetCourseId)) {
            return true;
        }
    }
    return false;
}


//===================================== Waitlist heap funcs ====================================

bool compareWaitlistStudents(const WaitlistStudent& a, const WaitlistStudent& b) {
    if (a.gpa != b.gpa) {
        return a.gpa > b.gpa;
    }
    return strcmp(a.id, b.id) < 0;
}
void heapifyUp(WaitlistStudent heap[], int& size, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (compareWaitlistStudents(heap[index], heap[parent])) {

            swapWaitlistStudents(heap[index], heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapifyDown(WaitlistStudent heap[], int& size, int index) {
    while (index < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size && compareWaitlistStudents(heap[left], heap[largest])) {
            largest = left;
        }
        if (right < size && compareWaitlistStudents(heap[right], heap[largest])) {
            largest = right;
        }

        if (largest != index) {

            swapWaitlistStudents(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}
void addToWaitlist(Course& course, const char* studentId, float gpa) {
    for (int i = 0; i < course.waitlistSize; ++i) {
    if (strcmp(course.waitlist[i].id, studentId) == 0) {
        cout << "Student is already on the waitlist for this course!\n";
        return;
    }
    }


    if (course.waitlistSize >= MAX_WAITLIST_SIZE) {
        cout << "Waitlist is full!\n";
        return;
    }

    WaitlistStudent ws;
    strcpy(ws.id, studentId);
    ws.gpa = gpa;

    course.waitlist[course.waitlistSize] = ws;
    heapifyUp(course.waitlist, course.waitlistSize, course.waitlistSize);
    course.waitlistSize++;

    cout << "Student " << studentId << " added to waitlist for course " << course.id << endl;
}

bool removeFromWaitlist(Course& course, char* studentId) {
    if (course.waitlistSize == 0) {
        return false;
    }

    strcpy(studentId, course.waitlist[0].id);
    course.waitlist[0] = course.waitlist[course.waitlistSize - 1];
    course.waitlistSize--;
    heapifyDown(course.waitlist, course.waitlistSize, 0);

    return true;
}



// ==================================== Enrollment System related func ===============================================

bool addStudentToCourse(const char* studentId, const char* courseId,const float gpa, bool isFromUndoRedo = false){
    Student* student = findStudent(studentId);
    avlNode* courseNode = searchCourseAVL(courseRoot, courseId);

    if (student == nullptr || courseNode == nullptr) {
        cout << "Student or Course not found!\n";
        return false;
    }
     if (student->gpa == 0.0) {
            student->gpa = gpa;
        }
    // Check if departments match
    if (strcmp(student->department, courseNode->data.department) != 0) {
        cout << "Error: Student department (" << student->department
            << ") doesn't match course department (" << courseNode->data.department << ")!\n";
        return false;
    }

   if (!hasPrerequisites(student, courseNode->data)) {
    cout << "Student doesn't meet prerequisites for this course!\n";
    cout << "Required prerequisites:\n";
    for (int i = 0; i < courseNode->data.prereqCount; i++) {
        // Look up the prerequisite course to get its name
        avlNode* prereqNode = searchCourseAVL(courseRoot, courseNode->data.prerequisites[i]);
        if (prereqNode) {
            cout << " - " << courseNode->data.prerequisites[i] << " (" << prereqNode->data.name << ")\n";
        } else {
            cout << " - " << courseNode->data.prerequisites[i] << " (Course not found in system)\n";
        }
    }
    return false;
}


    // Check if already registered
    for (int i = 0; i < student->courseCount; i++) {
        if (strcmp(student->registeredCourses[i], courseId) == 0) {
            cout << "Student is already registered in this course!\n";
            return false;
        }
    }
    // Check course capacity
    if (courseNode->data.enrolledCount >= courseNode->data.capacity) {
        cout << "Course is full! Adding to waitlist...\n";
        addToWaitlist(courseNode->data, studentId, gpa);
        return false;
    }

    // Add to both student's courses and course's students
    if (student->courseCount >= MAX_COURSES_PER_STUDENT) {
        cout << "Student has reached maximum course limit!\n";
        return false;
    }

    strcpy(student->registeredCourses[student->courseCount++], courseId);
    strcpy(courseNode->data.enrolledStudents[courseNode->data.enrolledCount++], studentId);
    cout << "Student " << studentId << " added to course " << courseId << " successfully!\n";




    if (!isFromUndoRedo) {
    Action act;
    strcpy(act.type, "enroll");
    strcpy(act.studentId, studentId);
    strcpy(act.courseId, courseId);
    push(undoStack, act);

    Action temp;
    while (redoStack != nullptr) {
        pop(redoStack, temp);
    }
}


    return true;
}



bool removeStudentFromCourse(const char* studentId, const char* courseId, bool isFromUndoRedo = false){
    Student* student = findStudent(studentId);
    avlNode* courseNode = searchCourseAVL(courseRoot, courseId);

    if (student == nullptr || courseNode == nullptr) {
        cout << "Student or Course not found!\n";
        return false;
    }

    // Find and remove course from student's list
    bool found = false;
    for (int i = 0; i < student->courseCount; i++) {
        if (strcmp(student->registeredCourses[i], courseId) == 0) {
            for (int j = i; j < student->courseCount - 1; j++) {
                strcpy(student->registeredCourses[j], student->registeredCourses[j + 1]);
            }
            student->courseCount--;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Student is not registered in this course!\n";
        return false;
    }

    // Find and remove student from course's list
    found = false;
    for (int i = 0; i < courseNode->data.enrolledCount; i++) {
        if (strcmp(courseNode->data.enrolledStudents[i], studentId) == 0) {
            for (int j = i; j < courseNode->data.enrolledCount - 1; j++) {
                strcpy(courseNode->data.enrolledStudents[j], courseNode->data.enrolledStudents[j + 1]);
            }
            courseNode->data.enrolledCount--;
            found = true;
            break;
        }
    }

    cout << "Student " << studentId << " removed from course " << courseId << " successfully!\n";

    // Check if we can add someone from waitlist
    if (courseNode->data.waitlistSize > 0) {
        char waitlistedStudent[ID_LENGTH];
        if (removeFromWaitlist(courseNode->data, waitlistedStudent)) {
            Student* ws = findStudent(waitlistedStudent);
            if (ws != nullptr && ws->courseCount < MAX_COURSES_PER_STUDENT) {
                strcpy(ws->registeredCourses[ws->courseCount++], courseId);
                strcpy(courseNode->data.enrolledStudents[courseNode->data.enrolledCount++], waitlistedStudent);
                cout << "Waitlisted student " << waitlistedStudent << " automatically registered in course " << courseId << endl;
            }
            else {
                cout << "Waitlisted student " << waitlistedStudent << " reached max course limit or not found, not registered.\n";
            }
        }
    }

    if (!isFromUndoRedo) {
    Action act;
    strcpy(act.type, "drop");
    strcpy(act.studentId, studentId);
    strcpy(act.courseId, courseId);
    push(undoStack, act);

    Action temp;
    while (redoStack != nullptr) {
        pop(redoStack, temp);
    }
}

    return true;
}



bool registerStudent() {
    Student* newStudent = new Student();

    // Validate student ID
    do {
        cout << "Enter Student ID: ";
        cin.getline(newStudent->id, ID_LENGTH);
        if (!isValidStudentId(newStudent->id)) {
            cout << "Invalid student ID format. Must contain letters and numbers, 5-19 characters long.\n";
        }
        else if (findStudent(newStudent->id) != nullptr) {
            cout << "Student with this ID already exists!\n";
            delete newStudent;
            return false;
        }
        else {
            break;
        }
    } while (true);

    // Validate name
    do {
        cout << "Enter Name: ";
        cin.getline(newStudent->name, NAME_LENGTH);
        if (!isValidName(newStudent->name)) {
            cout << "Invalid name format. Must start with a letter and contain only letters, spaces, or hyphens.\n";
        }
        else {
            break;
        }
    } while (true);

    // Validate department
    do {
        cout << "Enter Department: ";
        cin.getline(newStudent->department, DEPT_LENGTH);
        if (!isValidDepartment(newStudent->department)) {
            cout << "Invalid department format. Must contain letters, numbers, spaces, or hyphens.\n";
        }
        else {
            break;
        }
    } while (true);
newStudent->gpa=0.0;

    newStudent->courseCount = 0;
    newStudent->next = nullptr;
    addStudentToList(newStudent);
    cout << "Student registered successfully!\n";
    return true;
}

void enrollStudentInCourse() {
    char studentId[ID_LENGTH];
    char courseId[ID_LENGTH];

    cout << "Enter Student ID: ";
    cin.getline(studentId, ID_LENGTH);
    cout << "Enter Course ID: ";
    cin.getline(courseId, ID_LENGTH);

    Student* student = findStudent(studentId);
    if (!student) {
        cout << "Student not found!\n";
        return;
    }

    // Use the GPA that was stored during registration
    addStudentToCourse(studentId, courseId, student->gpa);
}

void dropCourse() {
    char studentId[ID_LENGTH];
    char courseId[ID_LENGTH];

    cout << "Enter Student ID: ";
    cin.getline(studentId, ID_LENGTH);
    cout << "Enter Course ID: ";
    cin.getline(courseId, ID_LENGTH);

    removeStudentFromCourse(studentId, courseId);
}

void addNewCourse() {
    if (courseCount >= MAX_COURSES) {
        cout << "Maximum number of courses reached!\n";
        return;
    }

    Course c;

    // Validate course ID
//    do {
//        cout << "Enter Course ID: ";
//        string input;
//        getline(cin, input);
//
//        const char* src = input.c_str();
//        int i;
//        for (i = 0; i < ID_LENGTH - 1 && src[i]; i++) {
//            c.id[i] = src[i];
//        }
//        c.id[i] = '\0';
//
//        if (!isValidCourseId(c.id)) {
//            cout << "Invalid course ID format. Must start with a letter and contain only alphanumeric characters, hyphens, or underscores.\n";
//        }
//        else if (courseExists(c.id)) {
//            cout << "Course with this ID already exists!\n";
//        }
//        else {
//            break;
//        }
//    } while (true);
do {
    cout << "Enter Course ID: ";
    string input;
    getline(cin, input);

    // Copy characters with bounds checking
    size_t i;
    for (i = 0; i < ID_LENGTH - 1 && i < input.length(); i++) {
        c.id[i] = input[i];
    }
    c.id[i] = '\0';  // Ensure null-termination

    if (!isValidCourseId(c.id)) {
        cout << "Invalid course ID format...\n";
    }
    else if (courseExists(c.id)) {
        cout << "Course with this ID already exists!\n";
    }
    else {
        break;
    }
} while (true);


    // Validate course name
    do {
        cout << "Enter Course Name: ";
        string input;
        getline(cin, input);
        strncpy(c.name, input.c_str(), NAME_LENGTH);
        c.name[NAME_LENGTH - 1] = '\0';
        if (!isValidName(c.name)) {
            cout << "Invalid course name format. Must start with a letter and contain only letters, spaces, or hyphens.\n";
        }
        else {
            break;
        }
    } while (true);

    // Validate department
    do {
        cout << "Enter Department: ";
        string input;
        getline(cin, input);
        strncpy(c.department, input.c_str(), DEPT_LENGTH);
        c.department[DEPT_LENGTH - 1] = '\0';
        if (!isValidDepartment(c.department)) {
            cout << "Invalid department format. Must contain letters, numbers, spaces, or hyphens.\n";
        }
        else {
            break;
        }
    } while (true);

    // Validate capacity
    bool validCapacity = false;
    do {
        cout << "Enter Capacity: ";
        string input;
        getline(cin, input);
        istringstream iss_cap(input);
        if (!(iss_cap >> c.capacity) || c.capacity <= 0) {
            cout << "Invalid capacity. Please enter a positive integer.\n";
        }
        else {
            validCapacity = true;
        }
    } while (!validCapacity);

    c.enrolledCount = 0;
    c.waitlistSize = 0;
    c.prereqCount = 0;

    courseRoot = insertCourseAVL(courseRoot, c);
    cout << "Course added successfully!\n";
}

//================================= print funcs =================================

void printAllStudents() {
    cout << "\n--- Loaded Students (" << studentCount << ") ---\n";
    Student* current = studentHead;
    while (current != nullptr) {
        cout << "ID: " << current->id << endl;
        cout << "Name: " << current->name << endl;
        cout << "Department: " << current->department << endl;

        cout << "Courses Registered: " << current->courseCount << endl;
        for (int j = 0; j < current->courseCount; j++) {
            cout << " - " << current->registeredCourses[j] << endl;
        }
        cout << "------------------------\n";
        current = current->next;
    }
}

void printAllCoursesHelper(avlNode* root) {
    if (root) {
        printAllCoursesHelper(root->left);

        cout << "ID: " << root->data.id << endl;
        cout << "Name: " << root->data.name << endl;
        cout << "Department: " << root->data.department << endl;
        cout << "Capacity: " << root->data.capacity << endl;
        cout << "Enrolled: " << root->data.enrolledCount << endl;
        for (int j = 0; j < root->data.enrolledCount; j++) {
            cout << " - " << root->data.enrolledStudents[j] << endl;
        }

        cout << "Waitlist (" << root->data.waitlistSize << " students):\n";
        WaitlistStudent temp[MAX_WAITLIST_SIZE];
        int tempSize = root->data.waitlistSize;

        for (int i = 0; i < tempSize; i++) {
            temp[i] = root->data.waitlist[i];
        }

        while (tempSize > 0) {
            cout << " - " << temp[0].id << " (GPA: " << temp[0].gpa << ")\n";
            temp[0] = temp[tempSize - 1];
            tempSize--;
            heapifyDown(temp, tempSize, 0);
        }
        cout << "------------------------\n";

        printAllCoursesHelper(root->right);
    }
}

void printAllCourses() {
    cout << "\n--- Loaded Courses ---\n";
    printAllCoursesHelper(courseRoot);
}

// ======================== Binary File Handling ========================

void saveStudents(const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    file.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));

    Student* current = studentHead;
    while (current != nullptr) {
        file.write(current->id, sizeof(current->id));
        file.write(current->name, sizeof(current->name));
        file.write(current->department, sizeof(current->department));
        file.write(reinterpret_cast<char*>(&current->courseCount), sizeof(current->courseCount));

        for (int j = 0; j < current->courseCount; j++) {
            file.write(current->registeredCourses[j], sizeof(current->registeredCourses[j]));
        }

        current = current->next;
    }
    file.close();
}

void loadStudents(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "No existing student data found." << endl;
        return;
    }

    // Clear existing list
    deleteStudentList();

    int count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (int i = 0; i < count; i++) {
        Student* newStudent = new Student();
        file.read(newStudent->id, sizeof(newStudent->id));
        file.read(newStudent->name, sizeof(newStudent->name));
        file.read(newStudent->department, sizeof(newStudent->department));
        file.read(reinterpret_cast<char*>(&newStudent->courseCount), sizeof(newStudent->courseCount));

        for (int j = 0; j < newStudent->courseCount; j++) {
            file.read(newStudent->registeredCourses[j], sizeof(newStudent->registeredCourses[j]));
        }

        newStudent->next = nullptr;
        addStudentToList(newStudent);
    }
    file.close();
}

void saveCoursesAVLHelper(ofstream& binFile, ofstream& txtFile, avlNode* root) {
    if (!root) return;

    saveCoursesAVLHelper(binFile, txtFile, root->left);

    // Write binary
    binFile.write(root->data.id, sizeof(root->data.id));
    binFile.write(root->data.name, sizeof(root->data.name));
    binFile.write(root->data.department, sizeof(root->data.department));
    binFile.write(reinterpret_cast<char*>(&root->data.capacity), sizeof(root->data.capacity));
    binFile.write(reinterpret_cast<char*>(&root->data.enrolledCount), sizeof(root->data.enrolledCount));
    for (int j = 0; j < root->data.enrolledCount; j++) {
        binFile.write(root->data.enrolledStudents[j], sizeof(root->data.enrolledStudents[j]));
    }
    binFile.write(reinterpret_cast<char*>(&root->data.waitlistSize), sizeof(root->data.waitlistSize));
    for (int j = 0; j < root->data.waitlistSize; j++) {
        binFile.write(root->data.waitlist[j].id, sizeof(root->data.waitlist[j].id));
        binFile.write(reinterpret_cast<char*>(&root->data.waitlist[j].gpa), sizeof(root->data.waitlist[j].gpa));
    }
    binFile.write(reinterpret_cast<char*>(&root->data.prereqCount), sizeof(root->data.prereqCount));
    for (int j = 0; j < root->data.prereqCount; j++) {
        binFile.write(root->data.prerequisites[j], sizeof(root->data.prerequisites[j]));
    }

    // Write human-readable
    txtFile << "Course ID: " << root->data.id << endl;
    txtFile << "Name: " << root->data.name << endl;
    txtFile << "Department: " << root->data.department << endl;
    txtFile << "Capacity: " << root->data.capacity << endl;
    txtFile << "Enrolled Count: " << root->data.enrolledCount << endl;
    txtFile << "Enrolled Students: ";
    for (int j = 0; j < root->data.enrolledCount; j++) {
        txtFile << root->data.enrolledStudents[j] << " ";
    }
    txtFile << endl;

    txtFile << "Waitlist Size: " << root->data.waitlistSize << endl;
    for (int j = 0; j < root->data.waitlistSize; j++) {
        txtFile << "  Waitlisted Student ID: " << root->data.waitlist[j].id
                << ", GPA: " << root->data.waitlist[j].gpa << endl;
    }
    txtFile << "--------------------------" << endl;

    saveCoursesAVLHelper(binFile, txtFile, root->right);
}

void saveCourses(const char* filenameBin, const char* filenameTxt) {
    ofstream binFile(filenameBin, ios::binary);
    ofstream txtFile(filenameTxt);
    if (!binFile || !txtFile) {
        cerr << "Error opening course files!" << endl;
        return;
    }

    binFile.write(reinterpret_cast<char*>(&courseCount), sizeof(courseCount));
    saveCoursesAVLHelper(binFile, txtFile, courseRoot);

    binFile.close();
    txtFile.close();
}

void loadCourses(const char* filenameBin) {
    ifstream file(filenameBin, ios::binary);
    if (!file) {
        cout << "No existing course data found." << endl;
        return;
    }

    // Clear current tree
    deleteAVL(courseRoot);
    courseRoot = nullptr;
    courseCount = 0;

    int total;
    file.read(reinterpret_cast<char*>(&total), sizeof(total));

    for (int i = 0; i < total; i++) {
        Course course;
        file.read(course.id, sizeof(course.id));
        file.read(course.name, sizeof(course.name));
        file.read(course.department, sizeof(course.department));
        file.read(reinterpret_cast<char*>(&course.capacity), sizeof(course.capacity));
        file.read(reinterpret_cast<char*>(&course.enrolledCount), sizeof(course.enrolledCount));
        for (int j = 0; j < course.enrolledCount; j++) {
            file.read(course.enrolledStudents[j], sizeof(course.enrolledStudents[j]));
        }
        file.read(reinterpret_cast<char*>(&course.waitlistSize), sizeof(course.waitlistSize));
        for (int j = 0; j < course.waitlistSize; j++) {
            file.read(course.waitlist[j].id, sizeof(course.waitlist[j].id));
            file.read(reinterpret_cast<char*>(&course.waitlist[j].gpa), sizeof(course.waitlist[j].gpa));
        }
        // After loading waitlist data
        file.read(reinterpret_cast<char*>(&course.prereqCount), sizeof(course.prereqCount));
        for (int j = 0; j < course.prereqCount; j++) {
            file.read(course.prerequisites[j], sizeof(course.prerequisites[j]));
        }

        // Rebuild heap
        for (int j = course.waitlistSize / 2 - 1; j >= 0; j--) {
            heapifyDown(course.waitlist, course.waitlistSize, j);
        }

        // Insert into AVL tree
        courseRoot = insertCourseAVL(courseRoot, course);
    }

    file.close();
}

void saveStudentsToTextFile(const char* filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening student text file: " << filename << endl;
        return;
    }

    file << "===== STUDENT LIST =====" << endl;
    Student* current = studentHead;
    int index = 1;
    while (current != nullptr) {
        file << index++ << ". ID: " << current->id << endl;
        file << "   Name: " << current->name << endl;
        file << "   Department: " << current->department << endl;
        file << "   Registered Courses: ";
        if (current->courseCount == 0) {
            file << "None";
        } else {
            for (int j = 0; j < current->courseCount; j++) {
                file << current->registeredCourses[j];
                if (j != current->courseCount - 1) file << ", ";
            }
        }
        file << endl << "--------------------------" << endl;
        current = current->next;
    }

    file.close();
}

void saveCoursesAVLToText(ofstream& file, avlNode* root, int& index) {
    if (root == nullptr) return;

    saveCoursesAVLToText(file, root->left, index);

    file << index++ << ". Course ID: " << root->data.id << endl;
    file << "   Name: " << root->data.name << endl;
    file << "   Department: " << root->data.department << endl;
    file << "   Capacity: " << root->data.capacity << endl;
    file << "   Enrolled Students: ";
    if (root->data.enrolledCount == 0) {
        file << "None";
    } else {
        for (int j = 0; j < root->data.enrolledCount; j++) {
            file << root->data.enrolledStudents[j];
            if (j != root->data.enrolledCount - 1) file << ", ";
        }
    }
    file << endl;

    file << "   Waitlist (GPA Priority): ";
    if (root->data.waitlistSize == 0) {
        file << "None";
    } else {
        for (int j = 0; j < root->data.waitlistSize; j++) {
            file << "[" << root->data.waitlist[j].id << ", GPA: " << root->data.waitlist[j].gpa << "]";
            if (j != root->data.waitlistSize - 1) file << ", ";
        }
    }
    file << endl << "-------------------------" << endl;

    saveCoursesAVLToText(file, root->right, index);
}

void saveCoursesToTextFile(const char* filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening course text file: " << filename << endl;
        return;
    }

    file << "===== COURSE LIST =====" << endl;
    int index = 1;
    saveCoursesAVLToText(file, courseRoot, index);
    file.close();
}


//============================ UNDO AND REDO function ==========================


void performAction(const char* type, const char* studentId, const char* courseId,const float gpa) {
    Action act;
    strcpy(act.type, type);
    strcpy(act.studentId, studentId);
    strcpy(act.courseId, courseId);

    if (strcmp(type, "enroll") == 0) {
        addStudentToCourse(studentId, courseId,gpa);
    }
    else if (strcmp(type, "drop") == 0) {
        removeStudentFromCourse(studentId, courseId);
    }

    push(undoStack, act);



    Action temp;
    while (redoStack != nullptr) {
        pop(redoStack, temp);
    }
}

void undoLastAction() {
    Action lastAct;
    if (!pop(undoStack, lastAct)) {
        cout << "Nothing to undo\n";
        return;
    }

    if (strcmp(lastAct.type, "enroll") == 0) {
        // Reverse of enrollment = drop
        removeStudentFromCourse(lastAct.studentId, lastAct.courseId, true);
    }
    else if (strcmp(lastAct.type, "drop") == 0) {
        // Reverse of drop = enroll
        addStudentToCourse(lastAct.studentId, lastAct.courseId, true);
    }

    // Push original action to redo stack
    push(redoStack, lastAct);
    cout << "Undo successful: " << lastAct.type << " " << lastAct.courseId << " for student " << lastAct.studentId << "\n";
}

void redoLastAction() {
    Action lastAct;
    if (!pop(redoStack, lastAct)) {
        cout << "Nothing to redo\n";
        return;
    }

    if (strcmp(lastAct.type, "enroll") == 0) {
        addStudentToCourse(lastAct.studentId, lastAct.courseId, true);
    }
    else if (strcmp(lastAct.type, "drop") == 0) {
        removeStudentFromCourse(lastAct.studentId, lastAct.courseId, true);
    }

    // Push re-executed action back to undo stack
    push(undoStack, lastAct);
    cout << "Redo successful: " << lastAct.type << " " << lastAct.courseId << " for student " << lastAct.studentId << "\n";
}


//===================== Department filter ================================
bool filterCoursesByDepartmentHelper(avlNode* root, const char* department) {
    if (root == nullptr) {
        return false;
    }

    bool foundLeft = filterCoursesByDepartmentHelper(root->left, department);
    bool foundCurrent = false;

    if (strcmp(root->data.department, department) == 0) {
        cout << "ID: " << root->data.id << endl;
        cout << "Name: " << root->data.name << endl;
        cout << "Capacity: " << root->data.capacity << endl;
        cout << "Enrolled: " << root->data.enrolledCount << endl;
        cout << "------------------------\n";
        foundCurrent = true;
    }

    bool foundRight = filterCoursesByDepartmentHelper(root->right, department);

    return foundLeft || foundCurrent || foundRight;
}
void filterCoursesByDepartment() {
    char department[DEPT_LENGTH];

    do {
        cout << "Enter Department to filter: ";
        cin.getline(department, DEPT_LENGTH);
        if (!isValidDepartment(department)) {
            cout << "Invalid department format. Must contain letters, numbers, spaces, or hyphens.\n";
        } else {
            break;
        }
    } while (true);

    cout << "\n--- Courses in Department: " << department << " ---\n";

    if (!filterCoursesByDepartmentHelper(courseRoot, department)) {
        cout << "No courses found in this department.\n";
    }
}

void checkWaitlistPosition() {
    char studentId[ID_LENGTH], courseId[ID_LENGTH];

    cout << "Enter Student ID: ";
    cin.getline(studentId, ID_LENGTH);
    cout << "Enter Course ID: ";
    cin.getline(courseId, ID_LENGTH);

    // Check if student exists
    Student* student = findStudent(studentId);
    if (!student) {
        cout << "Student not found!" << endl;
        return;
    }

    // Check if course exists
    avlNode* courseNode = searchCourseAVL(courseRoot, courseId);
    if (!courseNode) {
        cout << "Course not found!" << endl;
        return;
    }

    // Search for student in waitlist
    bool found = false;
    int position = 0;

    // Create a temporary copy of the waitlist to traverse
    WaitlistStudent tempWaitlist[MAX_WAITLIST_SIZE];
    int tempSize = courseNode->data.waitlistSize;

    for (int i = 0; i < tempSize; i++) {
        tempWaitlist[i] = courseNode->data.waitlist[i];
    }

    // Check each student in the waitlist
    for (int i = 0; i < tempSize; i++) {
        position++;
        if (strcmp(tempWaitlist[i].id, studentId) == 0) {
            found = true;
            break;
        }
    }

    if (found) {
        cout << "Student " << studentId << " is #" << position
             << " in the waitlist for " << courseId << endl;
    } else {
        cout << "Student is not on the waitlist for this course." << endl;
    }
}

void updateStudentGPA() {
    char studentId[ID_LENGTH];
    float newGPA;

    cout << "Enter Student ID: ";
    cin.getline(studentId, ID_LENGTH);

    Student* student = findStudent(studentId);
    if (!student) {
        cout << "Student not found!\n";
        return;
    }

    cout << "Current GPA: " << student->gpa << endl;

    while (true) {
        cout << "Enter new GPA (0.0-4.0): ";
        cin >> newGPA;
        cin.ignore();

        if (newGPA >= 0.0 && newGPA <= 4.0) {
            student->gpa = newGPA;
            cout << "GPA updated successfully!\n";
            break;
        }
        cout << "Invalid GPA. Please enter a value between 0.0 and 4.0.\n";
    }
}

void searchStudentById(){
char id[ID_LENGTH];

    cout << "Enter Student ID to search: ";
    cin.getline(id, ID_LENGTH);

    if (!isValidStudentId(id)) {
        cout << "Invalid student ID format!\n";
        return;
    }

    Student* student = findStudent(id);

    if (student == nullptr) {
        cout << "No student found with ID: " << id << endl;
        return;
    }

    // Display all student information
    cout << "\n--- Student Details ---\n";
    cout << "ID: " << student->id << endl;
    cout << "Name: " << student->name << endl;
    cout << "Department: " << student->department << endl;
    cout << fixed << setprecision(2);
    cout << "GPA: " << student->gpa << endl;
    cout << "Registered Courses (" << student->courseCount << "):\n";

    if (student->courseCount == 0) {
        cout << " - No courses registered\n";
    } else {
        for (int i = 0; i < student->courseCount; i++) {
            cout << " - " << student->registeredCourses[i] << endl;

            // Optional: Show course names too
            avlNode* courseNode = searchCourseAVL(courseRoot, student->registeredCourses[i]);
            if (courseNode) {
                cout << "   (" << courseNode->data.name << ")\n";
            }
        }
    }
    cout << "-----------------------\n";
    }


void searchStudentsByDepartment() {
    char department[DEPT_LENGTH];

    // Validate department input
    do {
        cout << "Enter Department to search: ";
        cin.getline(department, DEPT_LENGTH);
        if (!isValidDepartment(department)) {
            cout << "Invalid department format. Must contain letters, numbers, spaces, or hyphens.\n";
        } else {
            break;
        }
    } while (true);

    cout << "\n--- Students in Department: " << department << " ---\n";

    bool found = false;
    Student* current = studentHead;
    int count = 0;

    while (current != nullptr) {
        if (strcmp(current->department, department) == 0) {
            count++;
            found = true;
            cout << count << ". ID: " << current->id << endl;
            cout << "   Name: " << current->name << endl;
            cout << "   GPA: " << current->gpa << endl;
            cout << "   Registered Courses: ";

            if (current->courseCount == 0) {
                cout << "None";
            } else {
                for (int j = 0; j < current->courseCount; j++) {
                    cout << current->registeredCourses[j];
                    if (j != current->courseCount - 1) cout << ", ";
                }
            }
            cout << "\n------------------------\n";
        }
        current = current->next;
    }

    if (!found) {
        cout << "No students found in this department.\n";
    } else {
        cout << "Total students in " << department << " department: " << count << endl;
    }
}


void searchCourseById() {
    char courseId[ID_LENGTH];

    cout << "Enter Course ID to search: ";
    cin.getline(courseId, ID_LENGTH);

    if (!isValidCourseId(courseId)) {
        cout << "Invalid course ID format!\n";
        return;
    }

    avlNode* courseNode = searchCourseAVL(courseRoot, courseId);

    if (courseNode == nullptr) {
        cout << "No course found with ID: " << courseId << endl;
        return;
    }

    // Display all course information
    cout << "\n--- Course Details ---\n";
    cout << "ID: " << courseNode->data.id << endl;
    cout << "Name: " << courseNode->data.name << endl;
    cout << "Department: " << courseNode->data.department << endl;
    cout << "Capacity: " << courseNode->data.capacity << endl;
    cout << "Enrolled Students (" << courseNode->data.enrolledCount << "):\n";

    if (courseNode->data.enrolledCount == 0) {
        cout << " - No students enrolled\n";
    } else {
        for (int i = 0; i < courseNode->data.enrolledCount; i++) {
            cout << " - " << courseNode->data.enrolledStudents[i] << endl;
        }
    }

    cout << "Prerequisites (" << courseNode->data.prereqCount << "):\n";
    if (courseNode->data.prereqCount == 0) {
        cout << " - No prerequisites\n";
    } else {
        for (int i = 0; i < courseNode->data.prereqCount; i++) {
            cout << " - " << courseNode->data.prerequisites[i] << endl;
        }
    }

    cout << "Waitlist (" << courseNode->data.waitlistSize << " students):\n";
    if (courseNode->data.waitlistSize == 0) {
        cout << " - No students on waitlist\n";
    } else {
        // Create a temporary copy of the waitlist to display
        WaitlistStudent tempWaitlist[MAX_WAITLIST_SIZE];
        int tempSize = courseNode->data.waitlistSize;

        for (int i = 0; i < tempSize; i++) {
            tempWaitlist[i] = courseNode->data.waitlist[i];
        }

        // Display waitlist in priority order
        while (tempSize > 0) {
            cout << " - " << tempWaitlist[0].id << " (GPA: " << tempWaitlist[0].gpa << ")\n";
            tempWaitlist[0] = tempWaitlist[tempSize - 1];
            tempSize--;
            heapifyDown(tempWaitlist, tempSize, 0);
        }
    }
    cout << "-----------------------\n";
}
// ======================== Main Program ========================
int main() {
    loadStudents("students.dat");
    loadCourses("courses.dat");
    saveStudentsToTextFile("students.txt");        // text
    saveCoursesToTextFile("courses.txt");

    char studentId[ID_LENGTH];
    int choice;
    do {
        cout << endl;
    cout << "                   |*|" << endl;
    cout << "                   |         WELCOME TO COURSE REGISTRATION SYSTEM   |" << endl;
    cout << "                   |                                                 |" << endl;
    cout << "                   |-------------------------------------------------|" << endl;
    cout << "                   |     Manage Students, Courses, and Enrollment    |" << endl;
    cout << "                   |                                                 |" << endl;
    cout << "                   |                  Main Menu                      |" << endl;
    cout << "                   |-------------------------------------------------|" << endl;
    cout << "                   |  Press 1 : to Add New Student                   |" << endl;
    cout << "                   |  Press 2 : to Add New Course                    |" << endl;
    cout << "                   |  Press 3 : to Register Student in a Course      |" << endl;
    cout << "                   |  Press 4 : to Drop a Course                     |" << endl;
    cout << "                   |  Press 5 : to View All Students                 |" << endl;
    cout << "                   |  Press 6 : to View All Courses                  |" << endl;
    cout << "                   |  Press 7 : to Add Prerequisite to Course        |" << endl;
    cout << "                   |  Press 8 : to Filter Courses by Department      |" << endl;
    cout << "                   |  Press 9 : to Undo Last Action                  |" << endl;
    cout << "                   |  Press 10: to Redo Last Action                  |" << endl;
    cout << "                   |  Press 11: update student's GPA                 |" << endl;
    cout << "                   |  Press 12: to Check Waitlist Position           |" << endl;
    cout << "                   |  Press 13: to track student by ID               |" << endl;
    cout << "                   |  Press 14: to Search Students by Department     |" << endl;
    cout << "                   |  Press 15: to Search Course by ID               |" << endl;
    cout << "                   |  Press 16: to Save & Exit                       |" << endl;
    cout << "                   |*|"<<endl;
    cout   << endl;
    cout << "Enter Your Choice: ";
    cin>>choice;

        cin.ignore();


        switch (choice) {
            case 1:
                registerStudent();
                break;

            case 2:
                addNewCourse();
                break;

            case 3:
                enrollStudentInCourse();
                break;

            case 4:
                dropCourse();
                break;

            case 5:
                printAllStudents();
                break;

            case 6:
                printAllCourses();
                break;

            case 7: {
                char courseId[ID_LENGTH], prereqId[ID_LENGTH];
                cout << "Enter Course ID to add prerequisite to: ";
                cin.getline(courseId, ID_LENGTH);
                cout << "Enter Prerequisite Course ID: ";
                cin.getline(prereqId, ID_LENGTH);

                avlNode* courseNode = searchCourseAVL(courseRoot, courseId);
                avlNode* prereqNode = searchCourseAVL(courseRoot, prereqId);

                if (!courseNode) {
                    cout << "Course not found!\n";
                    break;
                }
                if (!prereqNode) {
                    cout << "Prerequisite course not found!\n";
                    break;
                }
                if (strcmp(courseId, prereqId) == 0) {
                    cout << "Course cannot be its own prerequisite!\n";
                    break;
                }

                if (isPrerequisiteCycle(prereqId, courseId)) {
                    cout << "Error: Adding this prerequisite would create a circular dependency!\n";
                    break;
                }


                if (courseNode->data.prereqCount >= MAX_PREREQUISITES) {
                    cout << "Maximum prerequisites reached for this course!\n";
                    break;
                }

                bool alreadyPrereq = false;
                for (int i = 0; i < courseNode->data.prereqCount; i++) {
                    if (strcmp(courseNode->data.prerequisites[i], prereqId) == 0) {
                        alreadyPrereq = true;
                        break;
                    }
                }

                if (alreadyPrereq) {
                    cout << "This course is already a prerequisite!\n";
                    break;
                }

                strcpy(courseNode->data.prerequisites[courseNode->data.prereqCount++], prereqId);
                cout << "Prerequisite added successfully!\n";
                break;
            }

            case 8:
                filterCoursesByDepartment();
                break;

            case 9:
                undoLastAction();
                break;

            case 10:
                redoLastAction();
                break;
            case 11:
                updateStudentGPA();
                break;
            case 12:
                checkWaitlistPosition();
                break;
            case 13:
                searchStudentById();
                break;
            case 14:
                searchStudentsByDepartment();
                break;
            case 15:
                searchCourseById();
                break;
            case 16:
                saveStudents("students.dat");
                //saveCourses("courses.dat");
                saveCourses("courses.dat", "courses.txt");
                loadCourses("courses.dat");

                cout << "Data saved. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Try again!\n";
        }

    } while (choice != 16);
    return 0;
}
