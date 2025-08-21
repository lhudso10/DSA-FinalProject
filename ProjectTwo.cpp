//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Leslie O'Boyle
// Version     : 1.0
// Copyright   : Copyright � 2025 SNHU COCE
// Description : Final Project
//============================================================================

// include libraries used
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// create struct to hold course information
struct Course {
    string courseNumber;
    string name;
    vector<string> prereqs;

};


// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};


//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);


public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    this->root = nullptr;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber > course.courseNumber) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the right node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder left
        this->inOrder(node->left);
        //output course number and name
        cout << node->course.courseNumber << ", " << node->course.name << endl;
        //InOder right
        this->inOrder(node->right);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(this->root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (this->root == nullptr) {
        // root is equal to new node bid
        this->root = new Node(course);
    }
    // else
    else {
        // add Node root and bid
        addNode(this->root, course);
    }
}

Course BinarySearchTree::Search(string courseNum) {
    // set current node equal to root
    Node* currNode = this->root;
    transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);
    
    // keep looping downwards until bottom reached or matching bidId found
    while (currNode != nullptr) {
        // if match found, return current bid
        if (currNode->course.courseNumber == courseNum) {
            return currNode->course;
        }
        // if bid is smaller than current node then traverse left
        else if (courseNum < currNode->course.courseNumber) {
            currNode = currNode->left;
        }
        // else larger so traverse right
        else {
            currNode = currNode->right;
        }
    }
    // not found
    Course course;
    return course;
}

//============================================================================
// Static methods
//============================================================================

// method to load the file
static void loadFile(string filename, BinarySearchTree* bst) {
    // open the file
    ifstream file(filename);

    // check to make sure file is open
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // read a line of the file
    string line;
    while (getline(file, line)) {
        // separate the line into course elements
        stringstream lineStream(line);
        string cell;
        vector<string> courseInfo;
        Course courseToAdd;
        while (getline(lineStream, cell, ',')) {
            // if the cell is not an empty cell
            if (cell != "") {
                // put into the course info vector
                courseInfo.push_back(cell);
            }
            
        }

        // set course number to first element in information vector
        courseToAdd.courseNumber = courseInfo[0];
        // set course name to second element in information vector
        courseToAdd.name = courseInfo[1];
        // if the vector has more elements
        if (courseInfo.size() > 2) {
            // add the rest of the elements to the course prereqs vector
            for (int i = 2; i < courseInfo.size(); ++i) {
                courseToAdd.prereqs.push_back(courseInfo[i]);
            }
        }

        // add the course to the binaray search tree
        bst->Insert(courseToAdd);

    }

    // close the file
    file.close();

}

// method to print a course's info
static void PrintCourseInfo(Course course) {
    cout << course.courseNumber << ", " << course.name << endl;
    if (!course.prereqs.empty()) {
        cout << "Prerequisites: ";
        cout << course.prereqs[0];
        for (int i = 1; i < course.prereqs.size(); ++i) {
            cout << ", " << course.prereqs[i];
        }
        cout << endl;
    }
    cout << endl;
}

// main method to run the program
int main()
{
    // define a binary search tree to hold all courses
    BinarySearchTree* bst = new BinarySearchTree();
    // define a course and string to hold a searched course number
    Course course;
    string courseNum;

    cout << "Welcome to the course planner." << endl;

    // while the user does not enter 9
    int choice = 0;
    while (choice != 9) {
        // print the menu
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl << "What would you like to do? ";
        // get user input
        cin >> choice;

        // create switch to perform menu tasks
        switch (choice) {
        // load a file into the binary search tree
        case 1:

            // call method to load files and put into the bst
            loadFile("CS 300 ABCU_Advising_Program_Input.csv", bst);
            cout << endl;

            break;

        // print the courses in order
        case 2:

            cout << "Here is a sample schedule:" << endl << endl;

            // call method to print the bst in order
            bst->InOrder();

            cout << endl;

            break;

        // search for and print a courses information
        case 3:

            cout << "What course do you want to know about? ";
            // get user course number input
            cin >> courseNum;

            // call method to search for the entered course number
            course = bst->Search(courseNum);

            // if the search method does not return an empty course
            if (!course.courseNumber.empty()) {
                // call method to print the course information
                PrintCourseInfo(course);
            }
            // if the search method returns an empty course
            else {
                // display that the course was not found in the course list
                cout << courseNum << " is not in course list.\n" << endl;
                // clear and reset the cin
                cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            break;

        // if anything else is entered
        default:
            // display that the entry was not valid
            cout << "Choice is not a valid option." << endl << endl;
            // clear and reset the cin
            cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    }

    // say goodbye
    cout << "Thank you for using the course planner!" << endl;

}

