#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

/*
Chris Bridges
CS 300 DSA: Analysis and Design
18 FEB 2025
*/

using namespace std;

const int TABLE_SIZE = 211;

// Defines the Course struct
struct Course {
    string number;
    string name;
    vector<string> prerequisites;
    bool isEmpty = true;
};

// Hash table class 
class HashTable {
private:
    vector<Course> table;

    // Hash function
    int hashFunction(const string& key) {
        unsigned long hash = 0;
        for (char ch : key) {
            hash = (hash * 53 + ch) % TABLE_SIZE; // Prime number 53 for better distribution
        }
        return hash % TABLE_SIZE;
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }

    // Insert course into the hash table
    void insert(const Course& course) {
        int index = hashFunction(course.number);

        // Finds an empty slot
        while (!table[index].isEmpty) {
            if (table[index].number == course.number) {
                return; // Prevents duplicate insertion
            }
            index = (index + 1) % TABLE_SIZE; // Moves to the next index
        }

        // Insert course into the available slot
        table[index] = course;
        table[index].isEmpty = false;
    }

    // Search for a course
    Course* search(const string& courseNumber) {
        int index = hashFunction(courseNumber);

        // search
        while (!table[index].isEmpty) {
            if (table[index].number == courseNumber) {
                return &table[index];   // If found 
            }
            index = (index + 1) % TABLE_SIZE;
        }

        return nullptr; // If not found
    }

    // Print courses in sorted order
    void printSorted() {
        vector<Course> courses;

        // Collects courses that are not empty
        for (const auto& course : table) {
            if (!course.isEmpty) {
                courses.push_back(course);
            }
        }

        if (courses.empty()) {
            cout << "Error: No courses available! Load courses first.\n";
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

// loads courses from a file into the hash table
void loadCourses(const string& fileName, HashTable& courseHashTable) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: The file cannot be opened.\n";
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

        // Ensures a course name and number exist
        if (items.size() < 2) {
            cout << "Error: A course name and number are required in line: " << line << endl;
            continue;
        }

        Course course;
        course.number = items[0];
        course.name = items[1];
        course.isEmpty = false;

        for (size_t i = 2; i < items.size(); ++i) {
            if (!items[i].empty()) {
                course.prerequisites.push_back(items[i]);
            }
        }

        courseHashTable.insert(course);
    }

    file.close();
    cout << "Courses successfully loaded.\n";
}

// Function to search for a specific course
void searchCourse(HashTable& courseHashTable, const string& courseNumber) {
    Course* course = courseHashTable.search(courseNumber);
    if (!course) {
        cout << "Error: Course not found: " << courseNumber << endl;
        return;
    }

    cout << "Course Number: " << course->number << endl;
    cout << "Course Name: " << course->name << endl;
    cout << "Prerequisites:\n";

    if (course->prerequisites.empty()) {
        cout << "  - None\n";
    }
    else {
        for (const string& prereq : course->prerequisites) {
            cout << "  - " << prereq << endl;
        }
    }
}

// Main function
int main() {
    HashTable courseHashTable;
    string fileName, courseNumber;
    int choice;

    cout << "Welcome to the ABCU Advising Software\n";

    while (true) {
        cout << "\n1. Load Courses\n2. Print Course List\n3. Search for a Course\n9. Exit\nEnter choice: ";

        // Input validation to prevent infinite looping
        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter course data file path: ";
            getline(cin, fileName);
            loadCourses(fileName, courseHashTable);
            break;
        case 2:
            courseHashTable.printSorted();
            break;
        case 3:
            cout << "Enter course number: ";
            getline(cin, courseNumber);
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);   // Forces to upper for search improvement `
            searchCourse(courseHashTable, courseNumber);
            break;
        case 9:
            cout << "Exiting the program. Thanks!\n";
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }
}
