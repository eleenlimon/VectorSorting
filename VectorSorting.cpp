//============================================================================
// Name        : VectorSorting.cpp
// Author      : Eleen Limon
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Forward declaration of a helper function to convert strings to doubles
double strToDouble(string str, char ch);

// Structure to hold bid information
struct Bid {
    string bidId; // Unique identifier for the bid
    string title;  // Title of the bid
    string fund;   // Fund associated with the bid
    double amount; // Monetary amount of the bid

    // Constructor to initialize the bid amount
    Bid() : amount(0.0) {}
};

//============================================================================
// Function Declarations
//============================================================================

void displayBid(const Bid& bid);
Bid getBid();
vector<Bid> loadBids(const string& csvPath);
int partition(vector<Bid>& bids, int begin, int end);
void quickSort(vector<Bid>& bids, int begin, int end);
void selectionSort(vector<Bid>& bids);
double strToDouble(string str, char ch);

//============================================================================
// Function Implementations
//============================================================================

/**
 * Display the bid information to the console.
 *
 * @param bid struct containing the bid info
 */
void displayBid(const Bid& bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
         << bid.fund << endl;
}

/**
 * Load a CSV file containing bids into a vector.
 *
 * @param csvPath the path to the CSV file to load
 * @return a vector holding all the bids read from the CSV
 */
vector<Bid> loadBids(const string& csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    vector<Bid> bids; // Vector to hold loaded bids
    csv::Parser file = csv::Parser(csvPath); // Initialize CSV parser

    try {
        // Loop through each row of the CSV file
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid; // Create a new Bid instance
            bid.bidId = file[i][1]; // Read bid ID
            bid.title = file[i][0];  // Read title
            bid.fund = file[i][8];   // Read fund
            bid.amount = strToDouble(file[i][4], '$'); // Convert amount string to double

            bids.push_back(bid); // Add the bid to the vector
        }
    } catch (csv::Error &e) {
        cerr << "Error loading CSV: " << e.what() << endl; // Handle CSV errors
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts for quick sort.
 *
 * @param bids Reference to the vector<Bid> to be partitioned
 * @param begin Starting index for the partition
 * @param end Ending index for the partition
 * @return Index of the pivot after partitioning
 */
int partition(vector<Bid>& bids, int begin, int end) {
    string pivot = bids[(begin + end) / 2].title; // Choose the middle element as pivot
    int low = begin; // Initialize low index
    int high = end;  // Initialize high index

    while (true) {
        // Increment low index while the bid title is less than the pivot
        while (bids[low].title < pivot) low++;
        // Decrement high index while the bid title is greater than the pivot
        while (pivot < bids[high].title) high--;

        if (low >= high) return high; // If indices cross, return high index

        // Swap the elements at low and high indices
        swap(bids[low], bids[high]);
        low++; // Move low index up
        high--; // Move high index down
    }
}

/**
 * Perform quick sort on the vector of bids by title.
 *
 * @param bids Reference to the vector<Bid> to be sorted
 * @param begin Starting index for the sort
 * @param end Ending index for the sort
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    if (begin < end) {
        int mid = partition(bids, begin, end); // Partition the bids
        quickSort(bids, begin, mid); // Recursively sort left partition
        quickSort(bids, mid + 1, end); // Recursively sort right partition
    }
}

/**
 * Perform selection sort on the vector of bids by title.
 *
 * @param bids Reference to the vector<Bid> to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    int size = bids.size(); // Get the size of the bids vector

    // Iterate through each position in the vector
    for (size_t pos = 0; pos < size - 1; pos++) {
        int minIndex = pos; // Assume the current position is the minimum
        // Find the minimum element in the remaining unsorted portion
        for (size_t j = pos + 1; j < size; j++) {
            if (bids[j].title < bids[minIndex].title) {
                minIndex = j; // Update the index of the minimum element
            }
        }
        // Swap the found minimum element with the element at the current position
        swap(bids[pos], bids[minIndex]);
    }
}

/**
 * Convert a string to a double after stripping out unwanted characters.
 *
 * @param str The input string to convert
 * @param ch The character to strip out
 * @return The converted double value
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end()); // Remove specified character
    return atof(str.c_str()); // Convert the string to double
}

/**
 * The main function that drives the program.
 */
int main(int argc, char* argv[]) {
    string csvPath; // Variable to hold the path to the CSV file
    switch (argc) {
    case 2:
        csvPath = argv[1]; // Use provided path
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv"; // Default path
    }

    vector<Bid> bids; // Vector to hold all bids
    clock_t ticks; // Timer variable
    int choice = 0; // User's menu choice

    while (choice != 9) {
        // Display menu options
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice; // Get user's choice

        switch (choice) {
        case 1: // Load bids from CSV
            ticks = clock(); // Start timer
            bids = loadBids(csvPath); // Load bids
            cout << bids.size() << " bids read" << endl; // Display number of bids read
            ticks = clock() - ticks; // Calculate elapsed time
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2: // Display all bids
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]); // Display each bid
            }
            cout << endl;
            break;

        case 3: // Perform selection sort
            ticks = clock(); // Start timer
            selectionSort(bids); // Sort bids using selection sort
            ticks = clock() - ticks; // Calculate elapsed time
            cout << bids.size() << " bids sorted" << endl;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4: // Perform quick sort
            ticks = clock(); // Start timer
            quickSort(bids, 0, bids.size() - 1); // Sort bids using quick sort
            ticks = clock() - ticks; // Calculate elapsed time
            cout << bids.size() << " bids sorted" << endl;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
    }

    cout << "Good bye." << endl; // Exit message
    return 0;
}
