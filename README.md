# 🎓 Student Performance Analyzer

A comprehensive data-driven academic performance management system designed to record, evaluate, and predict student performance across multiple assessments and subjects. Built as a **Data Structures and Algorithms (DSA)** project using **C++**.

---

## 👥 Team Members

| Name | Registration # | Degree |
|------|---------------|--------|
| Aleena Jamil | 2024081 | BS SWE |
| Sehrish Noor | 2024569 | BS SWE |
| Shoaib Hussain Iqbal | 2024591 | BS SWE |
| Ume Hani Tufail | 2024646 | BS SWE |

---

## 📋 Overview

The **Student Performance Analyzer** provides a comprehensive platform for **students**, **teachers**, and optionally **administrators**, enabling them to make informed decisions through data analysis and automated reporting.

**Key Objectives:**
- Automate the collection and analysis of student marks
- Identify weak areas and performance trends
- Provide teachers and students with data-driven insights
- Support predictive analytics for future performance

---

## ✨ Main Features

- ✅ Registration and login mechanism for Students and Teachers
- 📝 Storage and management of student marks for multiple subjects and assessments
- 🧮 Automatic calculation of total, average, and subject-wise performance
- 🏆 Ranking system to identify top and low-performing students
- 📉 Detection of weak subjects and analysis of performance trends over time
- 🔮 Prediction of future performance using weighted averages and trend data
- 📄 Generation of comprehensive student performance reports

---

## 👤 User Roles & Capabilities

### 🎒 Student
Students will be able to:
- View their own performance scores across all assessments (quizzes, assignments, exams)
- See their current rank in the class
- Track their performance trends (improving, declining, or stable) over time
- Identify their weakest subjects or assessment types
- Compare their scores with class averages
- View predicted performance for upcoming assessments based on trends
- See personalized recommendations for subjects that need improvement

### 👨‍🏫 Teacher/Lecturer
Teachers will be able to:
- Add, update, and delete student records and scores
- View complete class performance statistics for any subject or assessment
- Generate rankings of top N and bottom N performing students
- Identify students showing declining performance trends
- Search for specific students by ID, name, or performance criteria
- View subject-wise performance distribution across the entire class
- Compare performance across different assessment types (quiz vs. exam performance)
- Identify consistently weak areas across multiple students
- Export performance reports for individual students or the entire class
- Filter students based on custom performance thresholds

### 🏛️ Administration *(Optional)*
Administrators will be able to:
- View aggregate performance statistics across multiple courses and sections
- Compare performance trends between different teachers or sections
- Identify courses or subjects with consistently low performance
- Generate institutional-level reports for quality assurance
- View semester-wise or year-wise performance comparisons
- Access anonymized data for policy-making and curriculum planning

---

## 🔧 Detailed Feature Requirements

### 1. Registration / Login Mechanism
- **1.1** Homescreen with options to log in or register a new account
- **1.2** Separate login options for students and teachers
- **1.3** Registration captures: name, ID, email, password, and user type
- **1.4** Registration details stored in a text file
- **1.5** Login credentials validated against stored data
- **1.6** Invalid login attempts display appropriate error messages
- **1.7** Successful login redirects users to role-specific dashboards

### 2. Storage and Management of Student Marks
- **2.1** Teachers can add, edit, and delete student marks for different subjects and assessments (quiz, assignment, exam, etc.)
- **2.2** Each record includes: student ID, student name, subject, assessment type, obtained marks, and total marks
- **2.3** All marks saved persistently in files
- **2.4** Automatic updates to relevant calculations when marks are modified

### 3. Automatic Calculation of Performance
- **3.1** Automatic calculation of total marks obtained by each student
- **3.2** Calculation of overall percentage or average marks
- **3.3** Subject-wise average scores for every student
- **3.4** Real-time updates when marks are added or edited

### 4. Ranking System
- **4.1** Ranked list of students based on total or average marks
- **4.2** Automatic ranking updates when marks are modified
- **4.3** Teachers can view top N and bottom N students
- **4.4** Students can see their current class rank on their dashboard

### 5. Detection of Weak Subjects and Performance Trends
- **5.1** Detection of subjects where performance is below 40%
- **5.2** Weak subjects highlighted for each student
- **5.3** Comparison of scores over multiple tests to identify trends (improving, declining, or stable)
- **5.4** Teachers can view students with consistent weaknesses in certain subjects

### 6. Prediction of Future Performance
- **6.1** Prediction of expected scores for upcoming assessments based on recent trends
- **6.2** Predictions calculated using weighted averages or trend-based formulas
- **6.3** Students and teachers can view predicted performance for improvement planning

### 7. Report Generation
- **7.1** Detailed performance reports for each student
- **7.2** Reports include: total marks, averages, rank, scores in each subject, and weak subjects
- **7.3** Teachers can export reports for individual students or the whole class
- **7.4** Students can view their own report through their dashboard

### 8. Teacher Functionalities
- **8.1** Search for students by Student ID, name, or performance level
- **8.2** View overall class performance for any subject or assessment type
- **8.3** Display comparison charts or summaries
- **8.4** Filter students based on marks thresholds
- **8.5** Identify students whose performance is dropping over time

### 9. Administrator Functionalities *(Optional)*
- **9.1** View aggregated performance data across multiple classes or sections
- **9.2** Compare performance across teachers or courses
- **9.3** Generate summarized institutional reports
- **9.4** Extended functionality to be implemented in future versions

---

## 🧠 DSA Concepts Applied

This project demonstrates the practical application of fundamental Data Structures and Algorithms:

| Concept | Application |
|---------|-------------|
| **Arrays & Linked Lists** | Storage and traversal of student and marks data |
| **Queues** | Managing user operations or report generation sequences |
| **Stacks** | Handling temporary computation during report creation |
| **Sorting Algorithms** | Ranking students by performance |
| **Searching Algorithms** | Retrieving user or record data efficiently |
| **File Handling** | Persistent user and marks data storage |
| **Complexity Optimization** | Ensuring efficient performance across large datasets |

---

## ⚙️ System Architecture

**Core Modules:**
1. **Authentication Module** — Handles user registration, login, and validation
2. **Marks Management Module** — Stores and updates marks for students
3. **Performance Calculator** — Computes totals, averages, and trends
4. **Ranking Engine** — Sorts students and identifies top/bottom performers
5. **Report Generator** — Creates detailed performance reports
6. **Predictive Module** — Analyzes previous results to estimate future scores
7. **Admin Dashboard** *(Optional)* — Aggregates institutional-level data

---

## 💾 Data Storage

All data is stored persistently using **text or CSV files**, including:
- User credentials (students and teachers)
- Marks records (subject, assessment type, obtained marks, total marks)
- Generated reports and rankings

*This structure can be extended to databases such as SQLite or MySQL for scalability.*

---

## 🧮 Computational Design

The system follows a modular, function-based architecture emphasizing algorithmic efficiency:

| Operation | Description | Average Complexity |
|-----------|-------------|--------------------|
| Record Insertion | Adding new student marks | O(1) |
| Record Update | Editing or recalculating marks | O(n) |
| Sorting | Ranking students | O(n log n) |
| Search | Finding student by ID or name | O(log n) |
| Trend Analysis | Multi-record evaluation | O(n) |

---

## 🛠️ Technology Stack

- **Language:** C++
- **Core Concepts:** Object-Oriented Programming, Data Structures & Algorithms
- **Storage:** Text/CSV Files
- **IDE:** Code::Blocks / Visual Studio Code / Dev-C++
- **Interface:** Console-based application (CLI)

---

## 📂 Project Structure

```
StudentPerformanceAnalyzer/
│
├── src/
│   ├── main.cpp
│   ├── authentication.cpp
│   ├── marks_management.cpp
│   ├── performance_calculator.cpp
│   ├── ranking_engine.cpp
│   ├── report_generator.cpp
│   └── predictive_module.cpp
│
├── include/
│   ├── authentication.h
│   ├── marks_management.h
│   ├── performance_calculator.h
│   ├── ranking_engine.h
│   ├── report_generator.h
│   └── predictive_module.h
│
├── data/
│   ├── users.txt
│   ├── marks.txt
│   └── reports/
│
├── docs/
│   └── project_documentation.pdf
│
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites
- C++ compiler (GCC, MinGW, or MSVC)
- Code::Blocks, Visual Studio Code, or Dev-C++

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/student-performance-analyzer.git
   ```

2. Navigate to the project directory:
   ```bash
   cd student-performance-analyzer
   ```

3. Compile the project:
   ```bash
   g++ src/*.cpp -o StudentPerformanceAnalyzer
   ```

4. Run the application:
   ```bash
   ./StudentPerformanceAnalyzer
   ```

---

## 📸 Screenshots

*Screenshots will be added as features are implemented*

---

## 🤝 Contributing

This is an academic project developed as part of the DSA course (CS-221). Team members collaborate through Git for version control and code management.

---

## 📄 License

This project is developed for educational purposes as part of university coursework.

---

## 📧 Contact

For questions or suggestions, please contact any of the team members through the university email system.

---

**Course:** Data Structures and Algorithms (CS-221)  
**Institution:** [Your University Name]  
**Semester:** [Current Semester]
