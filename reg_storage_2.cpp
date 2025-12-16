#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct User {
    string name;
    string id;
    string email;
    string password;
    string role;
};

struct MarksRecord {
    string studentID;
    string studentName;
    string subject;
    string assessment;
    int obtained;
    int total;
};

vector<MarksRecord> records;

// Load marks from file
void loadMarks() {
    records.clear();
    ifstream file("marks.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        MarksRecord m;
        string obtainedStr, totalStr;
        if (getline(ss, m.studentID, ',') &&
            getline(ss, m.studentName, ',') &&
            getline(ss, m.subject, ',') &&
            getline(ss, m.assessment, ',') &&
            getline(ss, obtainedStr, ',') &&
            getline(ss, totalStr)) {
            m.obtained = stoi(obtainedStr);
            m.total = stoi(totalStr);
            records.push_back(m);
        }
    }
}

// Save marks to file
void saveMarks() {
    ofstream file("marks.txt");
    for (auto &m : records) {
        file << m.studentID << "," << m.studentName << "," << m.subject << ","
             << m.assessment << "," << m.obtained << "," << m.total << endl;
    }
}

// Register new user
void registerUser() {
    User u;
    ofstream file("users.txt", ios::app);
    cin.ignore();

    cout << "Name: "; getline(cin, u.name);
    cout << "Registration ID: "; getline(cin, u.id);
    cout << "Email: "; getline(cin, u.email);
    cout << "Password: "; getline(cin, u.password);
    cout << "Role (Student/Teacher): "; getline(cin, u.role);

    file << u.name << "," << u.id << "," << u.email << "," << u.password << "," << u.role << endl;
    cout << "Registration Successful!\n";
}

// Login function returns the ID of logged-in user
string login(const string &role) {
    ifstream file("users.txt");
    string id, pass, line;
    cin.ignore();
    cout << "ID: "; getline(cin, id);
    cout << "Password: "; getline(cin, pass);

    while (getline(file, line)) {
        stringstream ss(line);
        string name, uid, email, pwd, userRole;
        if (getline(ss, name, ',') && getline(ss, uid, ',') &&
            getline(ss, email, ',') && getline(ss, pwd, ',') &&
            getline(ss, userRole)) {
            if (uid == id && pwd == pass && userRole == role)
                return uid; // login successful
        }
    }
    return ""; // login failed
}

// Add marks
void addMarks() {
    MarksRecord m;
    cin.ignore();
    cout << "Student ID: "; getline(cin, m.studentID);
    cout << "Student Name: "; getline(cin, m.studentName);
    cout << "Subject: "; getline(cin, m.subject);
    cout << "Assessment Type: "; getline(cin, m.assessment);
    cout << "Obtained Marks: "; cin >> m.obtained;
    cout << "Total Marks: "; cin >> m.total;

    if (m.obtained > m.total) {
        cout << "Invalid Marks!\n";
        return;
    }

    loadMarks();
    records.push_back(m);
    saveMarks();
    cout << "Marks Added!\n";
}

// Update marks
void updateMarks() {
    loadMarks();
    string id;
    cin.ignore();
    cout << "Enter Student ID to update: "; getline(cin, id);
    for (auto &m : records) {
        if (m.studentID == id) {
            cout << "New Obtained Marks: "; cin >> m.obtained;
            cout << "New Total Marks: "; cin >> m.total;
            if (m.obtained > m.total) {
                cout << "Invalid Marks!\n";
                return;
            }
            saveMarks();
            cout << "Marks Updated!\n";
            return;
        }
    }
    cout << "Record not found!\n";
}

// Delete marks
void deleteMarks() {
    loadMarks();
    string id;
    cin.ignore();
    cout << "Enter Student ID to delete: "; getline(cin, id);
    for (auto it = records.begin(); it != records.end(); ++it) {
        if (it->studentID == id) {
            records.erase(it);
            saveMarks();
            cout << "Record Deleted!\n";
            return;
        }
    }
    cout << "Record not found!\n";
}

// View marks
void viewMarks(const string &studentID = "") {
    loadMarks();
    cout << "\n--- Student Marks ---\n";
    for (auto &m : records) {
        // Show only this student's marks if studentID is provided
        if (studentID.empty() || m.studentID == studentID) {
            cout << m.studentID << " | " << m.studentName << " | " << m.subject
                 << " | " << m.assessment << " | " << m.obtained << "/" << m.total << endl;
        }
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\n1. Register\n2. Teacher Login\n3. Student Login\n4. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) registerUser();

        else if (choice == 2) {
            string teacherID = login("Teacher");
            if (!teacherID.empty()) {
                int t;
                do {
                    cout << "\n1. Add Marks\n2. Update Marks\n3. Delete Marks\n4. View Marks\n5. Logout\nChoice: ";
                    cin >> t;
                    switch (t) {
                        case 1: addMarks(); break;
                        case 2: updateMarks(); break;
                        case 3: deleteMarks(); break;
                        case 4: viewMarks(); break; // teachers see all
                    }
                } while (t != 5);
            } else {
                cout << "Invalid ID or Password!\n";
            }
        }

        else if (choice == 3) {
            string studentID = login("Student");
            if (!studentID.empty()) {
                viewMarks(studentID); // show only this student's marks
            } else {
                cout << "Invalid ID or Password!\n";
            }
        }

        else if (choice == 4) break;
        else cout << "Invalid Option!\n";
    }
    return 0;
}

