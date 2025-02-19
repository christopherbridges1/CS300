#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cctype> 

/*
Chris Bridges
* CS300 DSA: Analysis and Design
* Project 2 Hash Table implementation
* 18 FEB 2025
*/

using namespace std;

const int TABLE_SIZE = 211; // Defines the size of the hash table

// Defines the course struct
struct Course {
    string number;
    string name;
    vector<string> prerequisites;
};

//Hash Table class using chaining
class HashTable {
private:
    vector<list<Course>> table; // Array of linked lists for chaining

    // Hash function
    int hashFunction(const string& key) {
        int hash = 0;
        for (char ch : key) {
            hash = (hash * 53 + ch) % TABLE_SIZE; // Hash
        }
        return hash;
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }

    // Insert course into hash table
    void insert(const Course& course) {
        int index = hashFunction(course.number);

        // Check if course already exists
        for (const Course& existingCourse : table[index]) {
            if (existingCourse.number == course.number) {
                return; // Prevents duplicate insertion in case file is loaded twice
            }
        }

        table[index].push_back(course);
    }


    // Search for a course
    Course* search(const string& courseNumber) {
        int index = hashFunction(courseNumber);
        for (Course& course : table[index]) {
            if (course.number == courseNumber) {
                return &course; // Returns the pointer to found course
            }
        }
        return nullptr;
    }

    // Print courses in sorted order
    void printSorted() {
        vector<Course> courses;
        for (const auto& bucket : table) {
            for (const auto& course : bucket) {
                courses.push_back(course);
            }
        }

        if (courses.empty()) {
            cout << "No courses available. Please try loading the courses first.\n";
            return;
        }

        // Sort courses by course number
        sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
            return a.number < b.number;
            });

        cout << "\nCourse List (Sorted):\n";
        for (const auto& course : courses) {
            cout << course.number << " - " << course.name << endl;
        }
    }
};

// Function to load courses from a file into the hash table
void loadCourses(const string& fileName, HashTable& courseHashTable) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error, The file cannot be opened\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> items;
        string item;

        // Read the line and split by commas
        while (getline(ss, item, ',')) {
            items.push_back(item);
        }

        // Ensure at least a course number and name exist
        if (items.size() < 2 || items[0].empty() || items[1].empty()) {
            cout << "Error: A course name and number are required in line: " << line << endl;
            continue;
        }

        Course course;
        course.number = items[0];
        course.name = items[1];

        // Process prerequisites (no "None" placeholders)
        for (size_t i = 2; i < items.size(); ++i) {
            if (!items[i].empty()) { // Only add non-empty prerequisites
                course.prerequisites.push_back(items[i]);
            }
        }

        courseHashTable.insert(course);
    }

    file.close();
    cout << "The courses were loaded successfully.\n";
}

// Function to search for a specific course
void searchCourse(HashTable& courseHashTable, const string& courseNumber) {
    Course* course = courseHashTable.search(courseNumber);
    if (!course) {
        cout << "The course was not found: " << courseNumber << endl;
        return;
    }

    cout << "Course Number: " << course->number << endl;
    cout << "Course Name: " << course->name << endl;
    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (const string& prereq : course->prerequisites) {
            cout << prereq << " ";
        }
    }
    cout << endl;
}

// Main function
int main() {
    HashTable courseHashTable;
    string fileName;
    string courseNumber;
    int choice;

    cout << "Welcome to the ABCU Advising Software\n";

    while (true) {
        cout << "\n ABCU Menu:\n";
        cout << "1. Load Courses\n";
        cout << "2. Print Course List\n";
        cout << "3. Search for a Course\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";

        // Validates user input to ensure only integers are accepted
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number between 1 and 9.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore();  // Clears newline from the buffer

        switch (choice) {
        case 1:
            cout << "Please enter the full file path and name of the course data file: ";
            getline(cin, fileName);
            loadCourses(fileName, courseHashTable);
            break;
        case 2:
            courseHashTable.printSorted();
            break;
        case 3:
            cout << "Please enter a course number: ";
            getline(cin, courseNumber);

            // Forces Course name to upper for a better user experience
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            searchCourse(courseHashTable, courseNumber);
            break;
        case 9:
            cout << "Exiting the program. Thanks!\n";
            return 0;
        default:
            cout << "That option is invalid. Please try again.\n";
        }
    }
}