#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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

void loadMarks() {
    records.clear();
    ifstream file("marks.txt");
    MarksRecord m;
    char comma;
    while (file >> m.studentID >> comma) {
        getline(file, m.studentName, ',');
        getline(file, m.subject, ',');
        getline(file, m.assessment, ',');
        file >> m.obtained >> comma >> m.total;
        records.push_back(m); 
    }
}

void saveMarks() {
    ofstream file("marks.txt");
    for (auto &m : records) {
        file << m.studentID << "," << m.studentName << "," << m.subject << ","
             << m.assessment << "," << m.obtained << "," << m.total << endl;
    }
}

void registerUser() {
    User u;
    ofstream file("users.txt", ios::app);

    cout << "Name: "; getline(cin, u.name);
    cout << "Registration ID: "; getline(cin, u.id);
    cout << "Email: "; getline(cin, u.email);
    cout << "Password: "; getline(cin, u.password);
    cout << "Role (Student/Teacher): "; getline(cin, u.role);

    file << u.name << "," << u.id << "," << u.email << "," << u.password << "," << u.role << endl;
    cout << "Registration Successful!\n";
}

bool login(string role) {
    ifstream file("users.txt");
    string id, pass, line;
    cout << "ID: "; cin >> id;
    cout << "Password: "; cin >> pass;

    while (getline(file, line)) {
        if (line.find(id) != string::npos && line.find(pass) != string::npos && line.find(role) != string::npos)
            return true; 
    }
    return false;
}

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

void updateMarks() {
    loadMarks();
    string id;
    cout << "Enter Student ID to update: "; cin >> id;
    for (auto &m : records) {
        if (m.studentID == id) {
            cout << "New Obtained Marks: "; cin >> m.obtained;
            cout << "New Total Marks: "; cin >> m.total;
            saveMarks();
            cout << "Marks Updated!\n";
            return;
        }
    }
    cout << "Record not found!\n";
}

void deleteMarks() {
    loadMarks();
    string id;
    cout << "Enter Student ID to delete: "; cin >> id;
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

void viewMarks() {
    loadMarks();
    cout << "\n--- Student Marks ---\n";
    for (auto &m : records) {
        cout << m.studentID << " | " << m.studentName << " | " << m.subject
             << " | " << m.assessment << " | " << m.obtained << "/" << m.total << endl;
    }
}


int main() {
    int choice;
    while (true) {
        cout << "\n1. Register\n2. Teacher Login\n3. Student Login\n4. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) registerUser();
        else if (choice == 2 && login("Teacher")) {
            int t;
            do {
                cout << "\n1. Add Marks\n2. Update Marks\n3. Delete Marks\n4. View Marks\n5. Logout\nChoice: ";
                cin >> t;
                if (t == 1) addMarks();
                else if (t == 2) updateMarks();
                else if (t == 3) deleteMarks();
                else if (t == 4) viewMarks();
            } while (t != 5);
        }
        else if (choice == 3 && login("Student")) viewMarks();
        else if (choice == 4) break;
        else cout << "Invalid Option!\n";
    }
    return 0;
}

