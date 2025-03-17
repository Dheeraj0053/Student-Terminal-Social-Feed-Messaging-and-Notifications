#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>
#include <limits>

using namespace std;

struct Student {
    string name;
    string prn;
    string email;
    string password;
    map<string, int> marks;
    vector<string> notes;
};

bool isValidName(const string& name) {
    for (char ch : name) {
        if (!isalpha(ch) && ch != ' ') {
            return false;
        }
    }
    return true;
}

bool isValidPRN(const string& prn) {
    for (char ch : prn) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos);
}

bool isValidPassword(const string& password) {
    return password.length() >= 6;
}

bool isValidMark(int mark) {
    return mark >= 0 && mark <= 100;
}

void sendEmail(const string& email) {
    cout << "Email sent to: " << email << endl;
    cout << "You have registered successfully!\n";
}

void saveStudentData(const vector<Student>& students) {
    ofstream outFile("students_data.txt");
    for (const auto& student : students) {
        outFile << student.name << " " << student.prn << " " << student.email << " " << student.password << "\n";
        outFile << "MARKS:\n";
        for (const auto& mark : student.marks) {
            outFile << mark.first << " " << mark.second << "\n";
        }
        outFile << "NOTES:\n";
        for (const auto& note : student.notes) {
            outFile << note << "\n";
        }
        outFile << "END\n";
    }
    outFile.close();
}

void loadStudentData(vector<Student>& students) {
    ifstream inFile("students_data.txt");
    if (!inFile) {
        cout << "No existing data found. Starting fresh.\n";
        return;
    }

    string line;
    Student student;
    while (getline(inFile, line)) {
        if (line == "MARKS:") {
            while (getline(inFile, line) && line != "NOTES:") {
                stringstream ss(line);
                string subject;
                int marks;
                ss >> subject >> marks;
                student.marks[subject] = marks;
            }
        } else if (line == "NOTES:") {
            while (getline(inFile, line) && line != "END") {
                student.notes.push_back(line);
            }
        } else if (line == "END") {
            students.push_back(student);
            student = Student();
        } else {
            stringstream ss(line);
            ss >> student.name >> student.prn >> student.email >> student.password;
        }
    }
    inFile.close();
}

void registerStudent(vector<Student>& students) {
    Student student;

    while (true) {
        cout << "Enter your name: ";
        getline(cin, student.name);
        if (isValidName(student.name)) {
            break;
        }
        cout << "Invalid name! Please enter only alphabets and spaces.\n";
    }

    while (true) {
        cout << "Enter your PRN no: ";
        getline(cin, student.prn);
        if (isValidPRN(student.prn)) {
            break;
        }
        cout << "Invalid PRN! Please enter only numbers.\n";
    }

    while (true) {
        cout << "Enter your email id: ";
        getline(cin, student.email);
        if (isValidEmail(student.email)) {
            break;
        }
        cout << "Invalid email! Please enter a valid email address.\n";
    }

    while (true) {
        cout << "Create a password (minimum 6 characters): ";
        getline(cin, student.password);
        if (isValidPassword(student.password)) {
            break;
        }
        cout << "Invalid password! Password must be at least 6 characters long.\n";
    }

    students.push_back(student);
    saveStudentData(students);
    sendEmail(student.email);
    cout << "Registration successful! Please check your email for confirmation.\n";
}

Student* loginStudent(vector<Student>& students) {
    string email, password;

    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your password: ";
    getline(cin, password);

    for (auto& student : students) {
        if (student.email == email && student.password == password) {
            cout << "Login successful! Welcome, " << student.name << ".\n";
            return &student;
        }
    }

    cout << "Login failed! Invalid email or password.\n";
    return nullptr;
}

void fillMarks(Student* student) {
    vector<string> subjects = {
        "Advanced Programming",
        "Machine Learning",
        "Deep Learning",
        "Cryptography",
        "Artificial Intelligence"
    };

    cout << "\nFill your marks for the following subjects (0 to 100):\n";
    for (const auto& subject : subjects) {
        int marks;
        while (true) {
            cout << subject << ": ";
            cin >> marks;
            if (cin.fail() || !isValidMark(marks)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 100.\n";
            } else {
                student->marks[subject] = marks;
                break;
            }
        }
    }

    char choice;
    cout << "Do you want to add marks for another subject? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cin.ignore();
        string subject;
        int marks;
        cout << "Enter the subject name: ";
        getline(cin, subject);
        while (true) {
            cout << "Enter marks for " << subject << ": ";
            cin >> marks;
            if (cin.fail() || !isValidMark(marks)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 100.\n";
            } else {
                student->marks[subject] = marks;
                break;
            }
        }
    }

    cout << "Marks have been saved successfully!\n";
}

void addNotes(Student* student) {
    string note;
    cout << "Enter your note: ";
    cin.ignore();
    getline(cin, note);
    student->notes.push_back(note);
    cout << "Note has been added successfully!\n";
}

void studentMenu(Student* student, vector<Student>& students) {
    int choice;

    while (true) {
        cout << "\nWelcome, " << student->name << "!\n";
        cout << "1. Fill Marks\n";
        cout << "2. Add Notes\n";
        cout << "3. View Data\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                fillMarks(student);
                saveStudentData(students);
                break;
            case 2:
                addNotes(student);
                saveStudentData(students);
                break;
            case 3:
                cout << "\nYour Data:\n";
                cout << "Marks:\n";
                for (const auto& mark : student->marks) {
                    cout << mark.first << ": " << mark.second << endl;
                }
                cout << "\nNotes:\n";
                for (const auto& note : student->notes) {
                    cout << "- " << note << endl;
                }
                break;
            case 4:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}

int main() {
    vector<Student> students;
    loadStudentData(students);

    int choice;
    while (true) {
        cout << "\nStudent Terminal: Social Feed, Messaging, and Notifications\n";
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                registerStudent(students);
                break;
            case 2: {
                Student* loggedInStudent = loginStudent(students);
                if (loggedInStudent) {
                    studentMenu(loggedInStudent, students);
                }
                break;
            }
            case 3:
                cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}