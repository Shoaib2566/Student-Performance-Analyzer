#include <iostream>
using namespace std;

class SubjectScoreNode {
public:
    char subject[20];
    char type[20];
    float obtained;
    float total;
    SubjectScoreNode* next;

    SubjectScoreNode() {
        obtained = total = 0;
        next = NULL;
    }
};

class Student {
public:
    int id;
    char name[30];
    float totalMarks;
    float average;
    int rank;
    SubjectScoreNode* scores;

    Student() {
        id = 0;
        totalMarks = 0;
        average = 0;
        rank = 0;
        scores = NULL;
    }
};

class StudentNode {
public:
    Student data;
    StudentNode* next;
    StudentNode() { next = NULL; }
};

StudentNode* studentHead = NULL;

void calculateStudentPerformance(Student &stu) {
    float sum = 0;
    int count = 0;

    SubjectScoreNode* temp = stu.scores;
    while (temp != NULL) {
        sum += temp->obtained;
        count++;
        temp = temp->next;
    }

    stu.totalMarks = sum;
    if (count > 0) stu.average = sum / count;
}

void showWeakSubjects(Student &stu) {
    cout << "\nWeak Subjects (Below 40%):\n";

    SubjectScoreNode* temp = stu.scores;
    int found = 0;

    while (temp != NULL) {
        float percentage = (temp->obtained / temp->total) * 100;
        if (percentage < 40) {
            cout << " - " << temp->subject << " (" << percentage << "%)\n";
            found = 1;
        }
        temp = temp->next;
    }

    if (found == 0) cout << "None\n";
}

void generateStudentReport(int studentID) {
    StudentNode* temp = studentHead;

    while (temp != NULL) {
        if (temp->data.id == studentID) {

            calculateStudentPerformance(temp->data);

            cout << "\n========== STUDENT REPORT ==========\n";
            cout << "ID       : " << temp->data.id << endl;
            cout << "Name     : " << temp->data.name << endl;
            cout << "Rank     : " << temp->data.rank << endl;
            cout << "Total    : " << temp->data.totalMarks << endl;
            cout << "Average  : " << temp->data.average << endl;

            cout << "\nSubject-wise Scores:\n";
            SubjectScoreNode* s = temp->data.scores;

            while (s != NULL) {
                float per = (s->obtained / s->total) * 100;
                cout << s->subject << " (" << s->type << ") = "
                     << s->obtained << "/" << s->total
                     << " [" << per << "%]\n";
                s = s->next;
            }

            showWeakSubjects(temp->data);

            cout << "=====================================\n";
            return;
        }
        temp = temp->next;
    }

    cout << "Student not found.\n";
}

void generateClassReport() {
    StudentNode* temp = studentHead;

    cout << "\n========== CLASS REPORT ==========\n";

    while (temp != NULL) {
        generateStudentReport(temp->data.id);
        temp = temp->next;
    }
}

void searchByID(int id) {
    StudentNode* temp = studentHead;

    while (temp != NULL) {
        if (temp->data.id == id) {
            cout << "\nStudent Found: " << temp->data.name << endl;
            generateStudentReport(id);
            return;
        }
        temp = temp->next;
    }

    cout << "No student found.\n";
}

void searchByName(const char* name) {
    StudentNode* temp = studentHead;

    while (temp != NULL) {
        int match = 1;
        for (int i = 0; name[i] != '\0'; i++) {
            if (temp->data.name[i] != name[i]) {
                match = 0;
                break;
            }
        }

        if (match == 1) {
            cout << "\nStudent Found: ID " << temp->data.id << endl;
            generateStudentReport(temp->data.id);
            return;
        }

        temp = temp->next;
    }

    cout << "No student found.\n";
}

void filterByMarks(float threshold) {
    cout << "\nStudents scoring below " << threshold << ":\n";

    StudentNode* temp = studentHead;
    while (temp != NULL) {
        if (temp->data.totalMarks < threshold) {
            cout << temp->data.name << " (ID " << temp->data.id
                 << ") - Total: " << temp->data.totalMarks << endl;
        }
        temp = temp->next;
    }
}

int isPerformanceDropping(Student &stu) {
    float last = -1;
    SubjectScoreNode* temp = stu.scores;

    while (temp != NULL) {
        if (last != -1 && temp->obtained < last)
            return 1;
        last = temp->obtained;
        temp = temp->next;
    }
    return 0;
}

void showDroppingStudents() {
    cout << "\nStudents with Dropping Performance:\n";

    StudentNode* temp = studentHead;

    while (temp != NULL) {
        if (isPerformanceDropping(temp->data) == 1)
            cout << temp->data.name << " (ID " << temp->data.id << ")\n";

        temp = temp->next;
    }
}

void generateReport() {
    int choice, id;

    cout << "\n======= REPORT MENU =======\n";
    cout << "1. Student Report\n";
    cout << "2. Class Report\n";
    cout << "===========================\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter Student ID: ";
        cin >> id;
        generateStudentReport(id);
    }
    else if (choice == 2) {
        generateClassReport();
    }
    else {
        cout << "Invalid choice.\n";
    }
}

void teacherFunctionalities() {
    cout << "\n========== TEACHER MENU ==========\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "3. Filter by Marks\n";
    cout << "4. Dropping Performance\n";
    cout << "==================================\n";
    cout << "Choose option from MAIN.\n";
}

int main() {

    int choice;
    int id;
    char name[30];
    float threshold;

    while (1) {
        cout << "\n=========== MAIN MENU ===========\n";
        cout << "1. Generate Reports\n";
        cout << "2. Teacher Functionalities\n";
        cout << "3. Exit\n";
        cout << "=================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            generateReport();
        }
        else if (choice == 2) {
            teacherFunctionalities();

            int option;
            cout << "\nEnter Teacher Option: ";
            cin >> option;

            if (option == 1) {
                cout << "Enter ID: ";
                cin >> id;
                searchByID(id);
            }
            else if (option == 2) {
                cout << "Enter Name: ";
                cin >> name;
                searchByName(name);
            }
            else if (option == 3) {
                cout << "Enter Marks Threshold: ";
                cin >> threshold;
                filterByMarks(threshold);
            }
            else if (option == 4) {
                showDroppingStudents();
            }
            else {
                cout << "Invalid Option.\n";
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid Choice.\n";
        }
    }

    return 0;
}
