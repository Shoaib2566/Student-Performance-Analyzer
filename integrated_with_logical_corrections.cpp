#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <iomanip>
#include <numeric> 
#include <cmath>    

using namespace std;

string toLower(string s) {
    for (char &c : s) {
        c = tolower(c);
    }
    return s;
}

bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);

    return atPos != string::npos &&
           dotPos != string::npos &&
           atPos > 0 &&
           dotPos > atPos + 1 &&
           dotPos < email.length() - 1;
}

struct ScoreNode {
    float obtained;
    float total;
    string assessmentType; 
    ScoreNode* next;

    ScoreNode(float o, float t, string type) {
        obtained = o;
        total = t;
        assessmentType = type;
        next = NULL;
    }
};

class Subject {
public:
    string name;
    ScoreNode* scoreHead; 

    Subject() {
        scoreHead = NULL;
        name = "";
    }

    void addScore(float o, float t, string type) {
        ScoreNode* newNode = new ScoreNode(o, t, type);
        if (scoreHead == NULL) {
            scoreHead = newNode;
        } else {
            ScoreNode* temp = scoreHead;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
    }
};

class Student {
public:
    string id;
    string name;
    string email;
    string password;
    string role; 

    map<string, Subject> subjects; 

    float totalMarks;
    float average;
    int rank;

    Student() {
        totalMarks = 0;
        average = 0;
        rank = 0;
    }
};

class StudentNode {
public:
    Student data;
    StudentNode* next;
    StudentNode() { next = NULL; }
};

StudentNode* studentHead = NULL;

struct MarksRecord {
    string studentID;
    string studentName;
    string subject;
    string assessment;
    int obtained;
    int total;
};

struct AnalyzedStudent {
    string studentID;
    string name;
    float totalObtained; 
    float totalMax;      
    float averagePercentage; 
    
    vector<float> scoreHistory;
    map<string, float> subjectScores;

    AnalyzedStudent() : totalObtained(0), totalMax(0), averagePercentage(0) {}
};

class PerformanceAnalyzer {
public:
    static vector<AnalyzedStudent> convertFromStorage(const vector<MarksRecord>& records) {
        map<string, AnalyzedStudent> studentMap;
        map<string, map<string, pair<float, float>>> tempSubjectTotals;

        for (const auto& rec : records) {
            if (studentMap.find(rec.studentID) == studentMap.end()) {
                AnalyzedStudent s;
                s.studentID = rec.studentID;
                s.name = rec.studentName;
                studentMap[rec.studentID] = s;
            }

            AnalyzedStudent& s = studentMap[rec.studentID];
            s.totalObtained += rec.obtained;
            s.totalMax += rec.total;

            if (rec.total > 0) {
                float percent = (static_cast<float>(rec.obtained) / rec.total) * 100.0f;
                s.scoreHistory.push_back(percent);
            }

            tempSubjectTotals[rec.studentID][rec.subject].first += rec.obtained;
            tempSubjectTotals[rec.studentID][rec.subject].second += rec.total;
        }

        vector<AnalyzedStudent> result;
        for (auto& pair : studentMap) {
            AnalyzedStudent& s = pair.second;

            if (s.totalMax > 0) {
                s.averagePercentage = (s.totalObtained / s.totalMax) * 100.0f;
            }

            for (auto& subPair : tempSubjectTotals[s.studentID]) {
                string subject = subPair.first;
                float obt = subPair.second.first;
                float tot = subPair.second.second;
                if (tot > 0) {
                    s.subjectScores[subject] = (obt / tot) * 100.0f;
                }
            }
            result.push_back(s);
        }
        return result;
    }
    
    static void sort(vector<float>& values) {
        int n = values.size();
        for (int i = 0; i < n - 1; ++i) {
            int minIdx = i;
            for (int j = i + 1; j < n; ++j) {
                if (values[j] < values[minIdx]) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                swap(values[i], values[minIdx]);
            }
        }
    }

    static float predictNextScore(const vector<float>& history) {
        if (history.empty()) return 0.0f;

        float weightedSum = 0.0f;
        float totalWeights = 0.0f;
        int n = history.size();

        for (int i = 0; i < n; ++i) {
            int weight = i + 1; 
            weightedSum += history[i] * weight;
            totalWeights += weight;
        }

        return (totalWeights == 0) ? 0.0f : (weightedSum / totalWeights);
    }

    static string detectTrend(const vector<float>& history) {
        if (history.size() < 2) return "STABLE"; 

        float latest = history.back();
        float sumPrev = 0.0f;
        int count = 0;

        for(size_t i=0; i < history.size()-1; ++i) {
            sumPrev += history[i];
            count++;
        }
        
        float avgPrev = sumPrev / count;
        float diff = latest - avgPrev;

        if (diff > 5.0f) return "IMPROVING";
        if (diff < -5.0f) return "DECLINING";
        return "STABLE";
    }

    static vector<string> identifyWeakSubjects(const AnalyzedStudent& student) {
        vector<string> weak;
        for (const auto& pair : student.subjectScores) {
            if (pair.second < 40.0f) { 
                weak.push_back(pair.first);
            }
        }
        return weak;
    }

    static void getPerformanceStats(const vector<float>& history, float& minVal, float& maxVal, float& medianVal) {
        if (history.empty()) { minVal = maxVal = medianVal = 0.0f; return; }
        
        vector<float> sorted = history;
        sort(sorted);

        minVal = sorted.front();
        maxVal = sorted.back();
        
        size_t sz = sorted.size();
        if (sz % 2 == 0) medianVal = (sorted[sz/2 - 1] + sorted[sz/2]) / 2.0f;
        else medianVal = sorted[sz/2];
    }
};

//Bridge Function
vector<MarksRecord> exportToAnalysisFormat() {
    vector<MarksRecord> records;
    StudentNode* temp = studentHead;
    while(temp) {
    	if (temp->data.role != "student") {
            temp = temp->next;
            continue;
        }
        for(auto& entry : temp->data.subjects) {
            Subject& sub = entry.second;
            ScoreNode* score = sub.scoreHead;
            while(score) {
                MarksRecord rec;
                rec.studentID = temp->data.id;
                rec.studentName = temp->data.name;
                rec.subject = sub.name;
                rec.assessment = score->assessmentType;
                rec.obtained = (int)score->obtained;
                rec.total = (int)score->total;
                records.push_back(rec);
                score = score->next;
            }
        }
        temp = temp->next;
    }
    return records;
}

void loadUsers() {
    ifstream file("users.txt");
    string line;
    if (!file) return; 

    while (getline(file, line)) {
        stringstream ss(line);
        Student tempStudent;
        getline(ss, tempStudent.name, ',');
        getline(ss, tempStudent.id, ',');
        getline(ss, tempStudent.email, ',');
        getline(ss, tempStudent.password, ',');
        getline(ss, tempStudent.role);
        tempStudent.role = toLower(tempStudent.role);

        StudentNode* newNode = new StudentNode();
        newNode->data = tempStudent;
        
        if (studentHead == NULL) {
            studentHead = newNode;
        } else {
            StudentNode* temp = studentHead;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
    }
    file.close();
}

void loadMarks() {
    ifstream file("marks.txt");
    string line;
    if (!file) return;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, sName, subName, type, obtStr, totStr;
        
        if (getline(ss, id, ',') && getline(ss, sName, ',') && 
            getline(ss, subName, ',') && getline(ss, type, ',') && 
            getline(ss, obtStr, ',') && getline(ss, totStr)) {
            
            StudentNode* temp = studentHead;
            while (temp != NULL) {
                if (temp->data.id == id && temp->data.role == "student") {
                    temp->data.subjects[subName].name = subName;
                    temp->data.subjects[subName].addScore(stof(obtStr), stof(totStr), type);
                    break;
                }
                temp = temp->next;
            }
        }
    }
    file.close();
}

bool isDuplicateID(const string& id) {
    StudentNode* temp = studentHead;
    while (temp) {
        if (temp->data.id == id) return true;
        temp = temp->next;
    }
    return false;
}

void registerUser() {
    Student s;
    cin.ignore();
    cout << "Name: "; getline(cin, s.name);
    cout << "ID: "; getline(cin, s.id);
    if (isDuplicateID(s.id)) {
    cout << "Registration number already exists. Try again.\n";
    return;
	}
    cout << "Email: "; getline(cin, s.email);
    if (!isValidEmail(s.email)) {
    cout << "Invalid email format.\n";
    return;}
    cout << "Password: "; getline(cin, s.password);
    cout << "Role (teacher/student): "; getline(cin, s.role);
    s.role = toLower(s.role);

    ofstream file("users.txt", ios::app);
    file << s.name << "," << s.id << "," << s.email << "," << s.password << "," << s.role << endl;
    file.close();
    
    StudentNode* newNode = new StudentNode();
    newNode->data = s;
    newNode->next = studentHead;
    studentHead = newNode; 
    
    cout << "Registration Successful!\n";
}

void saveMarkToFile(string id, string name, string sub, string type, float obt, float tot) {
    ofstream file("marks.txt", ios::app);
    file << id << "," << name << "," << sub << "," << type << "," << obt << "," << tot << endl;
    file.close();
}

class Calculation {
public:
    static double SubjectAverage(Subject &sub) {
        if (sub.scoreHead == NULL) return 0;
        double sum = 0;
        int count = 0;
        ScoreNode* temp = sub.scoreHead;
        while (temp) {
            sum += temp->obtained;
            count++;
            temp = temp->next;
        }
        return (count == 0) ? 0 : sum / count;
    }

    static void UpdateStudentStats(Student &stud) {
        double sum = 0;
        int count = 0;
        float totalObtained = 0;

        for (auto &entry : stud.subjects) {
            Subject &sub = entry.second;
            sum += SubjectAverage(sub);
            
            ScoreNode* temp = sub.scoreHead;
            while(temp) {
                totalObtained += temp->obtained;
                temp = temp->next;
            }
            count++;
        }
        
        stud.average = (count == 0) ? 0 : sum / count;
        stud.totalMarks = totalObtained;
    }

    static void CalculateRanks() {
    if (!studentHead) return;

    StudentNode* temp = studentHead;
    while (temp) {
        if (temp->data.role == "student") {
            UpdateStudentStats(temp->data);
        }
        temp = temp->next;
    }

    StudentNode* i = studentHead;
    while (i) {
        if (i->data.role != "student") {
            i = i->next;
            continue;
        }

        StudentNode* maxNode = i;
        StudentNode* j = i->next;

        while (j) {
            if (j->data.role == "student" &&
                j->data.average > maxNode->data.average) {
                maxNode = j;
            }
            j = j->next;
        }

        if (maxNode != i) {
            Student tempData = i->data;
            i->data = maxNode->data;
            maxNode->data = tempData;
        }

        i = i->next;
    }

    int rankCounter = 1;
    temp = studentHead;
    while (temp) {
        if (temp->data.role == "student") {
            temp->data.rank = rankCounter++;
        } else {
            temp->data.rank = 0; 
        }
        temp = temp->next;
    }
}
};

void showWeakSubjects(Student &stu) {
    cout << "\nWeak Subjects (Below 40%):\n";
    int found = 0;

    for (auto &entry : stu.subjects) {
        Subject &sub = entry.second;
        ScoreNode* temp = sub.scoreHead;
        while (temp) {
            float percentage = (temp->total > 0) ? (temp->obtained / temp->total) * 100 : 0;
            if (percentage < 40) {
                cout << " - " << sub.name << " [" << temp->assessmentType << "]: " << percentage << "%\n";
                found = 1;
            }
            temp = temp->next;
        }
    }
    if (!found) cout << "None\n";
}

int isPerformanceDropping(Student &stu) {
	if (stu.role != "student") return 0;
    for (auto &entry : stu.subjects) {
        Subject &sub = entry.second;
        ScoreNode* temp = sub.scoreHead;
        
        while(temp && temp->next) {
            if (temp->next->obtained < temp->obtained) return 1; 
            temp = temp->next;
        }
    }
    return 0;
}

void generateStudentReport(string id) {
    StudentNode* temp = studentHead;
    while (temp != NULL) {	
        	if (temp->data.id == id) {
   				 if (temp->data.role != "student") {
        		cout << "This user is not a student.\n";
       			return;
  		   }

            Calculation::UpdateStudentStats(temp->data); 
            
            cout << "\n========== STUDENT REPORT ==========\n";
            cout << "ID       : " << temp->data.id << endl;
            cout << "Name     : " << temp->data.name << endl;
            cout << "Rank     : " << temp->data.rank << endl;
            cout << "Total Marks: " << temp->data.totalMarks << endl;
            cout << "Average  : " << temp->data.average << endl;

            cout << "\n--- Subject Breakdown ---\n";
            for (auto &entry : temp->data.subjects) {
                cout << "Subject: " << entry.first << "\n";
                ScoreNode* s = entry.second.scoreHead;
                while (s) {
                    float per = (s->obtained / s->total) * 100;
                    cout << "   " << s->assessmentType << ": " << s->obtained << "/" << s->total << " (" << (int)per << "%)\n";
                    s = s->next;
                }
            }
            
            showWeakSubjects(temp->data);
            cout << "=====================================\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Student not found.\n";
}

void filterByMarks(float threshold) {
    cout << "\nStudents scoring below " << threshold << " (Total Marks):\n";

    StudentNode* temp = studentHead;
    bool found = false;

    while (temp != NULL) {
        if (temp->data.role == "student") {
            Calculation::UpdateStudentStats(temp->data);

            if (temp->data.totalMarks < threshold) {
                cout << temp->data.name 
                     << " (ID: " << temp->data.id 
                     << ") - Total Marks: " 
                     << temp->data.totalMarks << endl;
                found = true;
            }
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "No students found below this threshold.\n";
    }
}


void showDroppingStudents() {
    cout << "\nStudents with Dropping Performance:\n";
    StudentNode* temp = studentHead;
    bool found = false;
    while (temp != NULL) {
        if (temp->data.role == "student" && isPerformanceDropping(temp->data)) {
            cout << " - " << temp->data.name << " (" << temp->data.id << ")\n";
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "No consistent drops detected.\n";
}

void searchByName(string name) {
    StudentNode* temp = studentHead;
    bool found = false;
    while (temp != NULL) {
        if (temp->data.name == name && temp->data.role == "student") {
            generateStudentReport(temp->data.id);
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "No student found with name: " << name << endl;
}

void viewPredictiveAnalysis() {
    //Fetch data from Linked List and convert
    vector<MarksRecord> records = exportToAnalysisFormat();
    
    //Check if we actually have data
    if (records.empty()) {
        cout << "\n[!] No student marks found to analyze." << endl;
        cout << "    Please use 'Add Marks' to enter data first." << endl;
    } else {
        vector<AnalyzedStudent> students = PerformanceAnalyzer::convertFromStorage(records);

        // Display Header
        cout << "\n================ PREDICTIVE ANALYSIS REPORT ================\n";
        cout << left << setw(20) << "Name" 
                  << setw(15) << "Trend" 
                  << setw(15) << "Next Score" 
                  << "Weak Subjects" << endl;
        cout << "----------------------------------------------------------------\n";

        //Loop and Analyze
        for (const auto& s : students) {
            float predicted = PerformanceAnalyzer::predictNextScore(s.scoreHistory);
            string trend = PerformanceAnalyzer::detectTrend(s.scoreHistory);
            vector<string> weakSubs = PerformanceAnalyzer::identifyWeakSubjects(s);

            cout << left << setw(20) << s.name 
                      << setw(15) << trend 
                      << fixed << setprecision(1) << predicted << "%        ";

            if (weakSubs.empty()) {
                cout << "None";
            } else {
                for (const auto& sub : weakSubs) cout << sub << " ";
            }
            cout << endl;
        }
        cout << "================================================================\n";
    }

    cout << "\nPress Enter to return to menu.";
    cin.ignore(); 
    cin.get();
}

string login(string role) {
	role = toLower(role);
    string id, pass;
    cout << "Enter ID: "; cin >> id;
    cout << "Enter Password: "; cin >> pass;

    StudentNode* temp = studentHead;
    while (temp != NULL) {
        if (temp->data.id == id && temp->data.password == pass && temp->data.role == role) {
            return id;
        }
        temp = temp->next;
    }
    return "";
}

void teacherMenu() {
    int option;
    do {
        cout << "\n========== TEACHER MENU ==========\n";
        cout << "1. Add Marks\n"; 
        cout << "2. Search Student by ID\n"; 
        cout << "3. Search Student by Name\n"; 
        cout << "4. Filter by Marks Threshold\n"; 
        cout << "5. Show Dropping Performance\n"; 
        cout << "6. View Class Rankings\n"; 
        cout << "7. View Predictive Analysis & Trends\n";
        cout << "8. Logout\n";
        cout << "Choice: "; cin >> option;

        if (option == 1) {
            string sid, sub, type; float obt, tot;
            cout << "Student ID: "; cin >> sid;
            
            StudentNode* temp = studentHead;
            bool exists = false;
            while(temp) { 
				if(temp->data.id == sid && temp->data.role == "student"){ 
					exists=true; 
					break;} 
					temp=temp->next;}
            
            if(exists) {
                cout << "Subject: "; cin >> sub;
                cout << "Assessment Type: "; cin >> type;
                cout << "Obtained: "; cin >> obt;
                cout << "Total: "; cin >> tot;
                
                temp->data.subjects[sub].name = sub;
                temp->data.subjects[sub].addScore(obt, tot, type);
                
                saveMarkToFile(sid, temp->data.name, sub, type, obt, tot);
                cout << "Marks added successfully.\n";
            } else {
                cout << "Student ID not found or user is not a student.\n";
            }
        }
        else if (option == 2) {
            string id; cout << "Enter ID: "; cin >> id;
            generateStudentReport(id);
        }
        else if (option == 3) {
            string name; cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            searchByName(name);
        }
        else if (option == 4) {
            float th; cout << "Enter Threshold: "; cin >> th;
            filterByMarks(th);
        }
        else if (option == 5) {
            showDroppingStudents();
        }
        else if (option == 6) {
            Calculation::CalculateRanks();
            cout << "\n--- CLASS RANKINGS ---\n";
            StudentNode* temp = studentHead;
            while(temp) {
                if (temp->data.role == "student") {
                    cout << "Rank " << temp->data.rank 
                    << ": " << temp->data.name 
                    << " (Avg: " << temp->data.average << ")\n";
               }
            temp = temp->next;
           }
        }
        else if (option == 7) {
            viewPredictiveAnalysis();
        }
    } while (option != 8);
}

int main() {
    loadUsers();
    loadMarks();
    
    Calculation::CalculateRanks();

    int choice;
    while (true) {
        cout << "\n=========== MAIN SYSTEM ===========\n";
        cout << "1. Register New User\n"; 
        cout << "2. Teacher Login\n";     
        cout << "3. Student Login\n";     
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            if (login("teacher") != "") {
                cout << "Login Successful!\n";
                teacherMenu();
            } else {
                cout << "Invalid Teacher Credentials.\n";
            }
        }
        else if (choice == 3) {
            string id = login("student");
            if (id != "") {
                cout << "Login Successful!\n";
                Calculation::CalculateRanks(); 
                generateStudentReport(id); 
            } else {
                cout << "Invalid Student Credentials.\n";
            }
        }
        else {
        	cout << " Exiting the system.\n";
        	return 1;
		}
   }
}
