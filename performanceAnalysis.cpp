#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <numeric>
#include <cmath>
#include <iomanip>
using namespace std;

struct MarksRecord {
    string studentID;
    string studentName;
    string subject;
    string assessment;
    int obtained;
    int total;
};

struct Student {
    string studentID;
    string name;
    float totalObtained; 
    float totalMax;      
    float averagePercentage; 
    
    // History of assessment percentages for trend analysis
    vector<float> scoreHistory;
    
    // Subject-wise performance (Subject Name -> Average %)
    map<string, float> subjectScores;

    // Default constructor
    Student() : totalObtained(0), totalMax(0), averagePercentage(0) {}
};

// PERFORMANCE ANALYZER CLASS

class PerformanceAnalyzer {
public:
    // 1. DATA CONVERSION (Bridge between Storage and Analysis)
    
    //readying data for analysis
    static vector<Student> convertFromStorage(const vector<MarksRecord>& records) {
        map<string, Student> studentMap;
        // Temporary map: StudentID -> Subject -> Pair<ObtainedSum, TotalSum>
        map<string, map<string, pair<float, float>>> tempSubjectTotals;

        for (const auto& rec : records) {
            // Initialize student if not exists
            if (studentMap.find(rec.studentID) == studentMap.end()) {
                Student s;
                s.studentID = rec.studentID;
                s.name = rec.studentName;
                studentMap[rec.studentID] = s;
            }

            Student& s = studentMap[rec.studentID];
            
            // Update Global Totals
            s.totalObtained += rec.obtained;
            s.totalMax += rec.total;

            // Add to History (Calculate percentage for this specific assessment)
            if (rec.total > 0) {
                float percent = (static_cast<float>(rec.obtained) / rec.total) * 100.0f;
                s.scoreHistory.push_back(percent);
            }

            // Aggregate Subject Data
            tempSubjectTotals[rec.studentID][rec.subject].first += rec.obtained;
            tempSubjectTotals[rec.studentID][rec.subject].second += rec.total;
        }

        // Finalize Calculations
        vector<Student> result;
        for (auto& pair : studentMap) {
            Student& s = pair.second;

            // Calculate Overall Average
            if (s.totalMax > 0) {
                s.averagePercentage = (s.totalObtained / s.totalMax) * 100.0f;
            }

            // Calculate Subject Averages
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
                // Change comparison to < for Ascending order
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

        // Weight strategy: Older scores get low weight, recent scores get high weight.
        for (int i = 0; i < n; ++i) {
            int weight = i + 1; 
            weightedSum += history[i] * weight;
            totalWeights += weight;
        }

        return (totalWeights == 0) ? 0.0f : (weightedSum / totalWeights);
    }

    // 3. TREND ANALYSIS

    static string detectTrend(const vector<float>& history) {
        if (history.size() < 2) return "STABLE"; // Not enough data

        float latest = history.back();
        float sumPrev = 0.0f;
        int count = 0;

        for(size_t i=0; i < history.size()-1; ++i) {
            sumPrev += history[i];
            count++;
        }
        
        float avgPrev = sumPrev / count;
        float diff = latest - avgPrev;

        // Threshold 5%
        if (diff > 5.0f) return "IMPROVING";
        if (diff < -5.0f) return "DECLINING";
        return "STABLE";
    }

    static vector<string> identifyWeakSubjects(const Student& student) {
        vector<string> weak;
        for (const auto& pair : student.subjectScores) {
            if (pair.second < 40.0f) { // 40% Threshold
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

int main() {
    cout << "=== Performance Analysis Module Driver ===\n" << endl;

    // 1. Simulate Raw Data (The format used by the Storage Team)
    // We create a vector of MarksRecord just like reading from the file
    vector<MarksRecord> rawRecords = {
        // --- Student 1: Aleena (Improving Trend) ---
        {"2024081", "Aleena Jamil", "DSA", "Quiz 1", 6, 10},      // 60%
        {"2024081", "Aleena Jamil", "DSA", "Quiz 2", 8, 10},      // 80%
        {"2024081", "Aleena Jamil", "DSA", "Midterm", 45, 50},    // 90%
        {"2024081", "Aleena Jamil", "Calculus", "Quiz 1", 9, 10}, // 90%
        
        // --- Student 2: Shoaib (Declining Trend) ---
        {"2024591", "Shoaib Iqbal", "DSA", "Quiz 1", 10, 10},     // 100%
        {"2024591", "Shoaib Iqbal", "DSA", "Quiz 2", 8, 10},      // 80%
        {"2024591", "Shoaib Iqbal", "DSA", "Midterm", 30, 50},    // 60%
        {"2024591", "Shoaib Iqbal", "Physics", "Quiz 1", 8, 10},  // 80%

        // --- Student 3: Ume Hani (Weak Subject Issue) ---
        {"2024646", "Ume Hani", "DSA", "Quiz 1", 9, 10},          // 90%
        {"2024646", "Ume Hani", "Calculus", "Quiz 1", 3, 10},     // 30% (Weak)
        {"2024646", "Ume Hani", "Calculus", "Midterm", 15, 50}    // 30% (Weak)
    };

    cout << "Raw data loaded (" << rawRecords.size() << " records).\n";

    // 2. Convert Data
    // This aggregates the raw rows into Student profiles
    cout << "Converting to Student Profiles...\n" << endl;
    vector<Student> students = PerformanceAnalyzer::convertFromStorage(rawRecords);

    // 3. Run Analysis for each student
    cout << left << setw(20) << "Name" 
              << setw(15) << "Trend" 
              << setw(15) << "Prediction" 
              << "Weak Subjects" << endl;
    cout << "----------------------------------------------------------------\n";

    for (const auto& s : students) {
        // A. Prediction
        float predicted = PerformanceAnalyzer::predictNextScore(s.scoreHistory);
        
        // B. Trend
        string trend = PerformanceAnalyzer::detectTrend(s.scoreHistory);

        // C. Weak Subjects
        vector<string> weakSubs = PerformanceAnalyzer::identifyWeakSubjects(s);

        // Output Row
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

    cout << "\n----------------------------------------------------------------\n";

    // 4. Detailed Stats Check (Optional)
    // Just to demonstrate the stats function for the first student
    if (!students.empty()) {
        float minVal, maxVal, medianVal;
        PerformanceAnalyzer::getPerformanceStats(students[0].scoreHistory, minVal, maxVal, medianVal);
        cout << "\nDetailed Stats for " << students[0].name << ":\n";
        cout << "Min: " << minVal << "%, Max: " << maxVal << "%, Median: " << medianVal << "%\n";
    }

    return 0;
}