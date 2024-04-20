#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
using namespace std;
// Passenger structure to hold passenger details
struct Passenger {
int age;
string name;
string className;
char berth; // 'L' for lower berth, 'U' for upper berth
int seatNumber; // Seat number
int id; // Unique identifier for each passenger
Passenger(int _age, string _name, string _className, char _berth, int _seatNumber, int
_id) : age(_age), name(_name), className(_className), berth(_berth),
seatNumber(_seatNumber), id(_id) {}
};
// Comparator for priority queues based on passenger age and class
struct AgeClassComparator {
bool operator()(const Passenger& p1, const Passenger& p2) const {
if (p1.age != p2.age) {
return p1.age < p2.age; // Younger passengers have higher priority
} else {
// If ages are equal, prioritize based on class name
return p1.className < p2.className;
}
}
};
string getCurrentDate() {
// Get current date
time_t now = time(0);
tm *ltm = localtime(&now);
stringstream currentDate;
currentDate << setw(2) << setfill('0') << ltm->tm_mday << "-" << setw(2) <<
setfill('0') << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year;
return currentDate.str();
}
// Function to check if the booking date and time are valid
bool isValidBooking(const string& selectedDate, const string& selectedTime) {
// Get the current date
time_t now = time(0);
tm *ltm = localtime(&now);
stringstream currentDate;
currentDate << setw(2) << setfill('0') << ltm->tm_mday << "-" << setw(2) <<
setfill('0') << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year;
// Parse the selected date
stringstream ss(selectedDate);
int day, month, year;
char dash;
ss >> day >> dash >> month >> dash >> year;
// Compare the parsed selected date with the current date
if (year < 1900 + ltm->tm_year || (year == 1900 + ltm->tm_year && month < 1 +
ltm->tm_mon) || (year == 1900 + ltm->tm_year && month == 1 + ltm->tm_mon && day <
ltm->tm_mday)) {
cout << "Invalid date! Booking date cannot be in the past." << endl;
return false;
} else if (year == 1900 + ltm->tm_year && month == 1 + ltm->tm_mon && day ==
ltm->tm_mday) {
// Check if the selected time is in the future
auto now = chrono::system_clock::now();
time_t now_time = chrono::system_clock::to_time_t(now);
tm *ltm = localtime(&now_time);
stringstream currentTime;
currentTime << setw(2) << setfill('0') << ltm->tm_hour << ":" << setw(2) <<
setfill('0') << ltm->tm_min;
if (selectedTime < currentTime.str()) {
cout << "Invalid time! Booking time must be at least 1 day prior." << endl;
return false;
}
}
return true;
}
// Function to allocate berths to passengers
void allocateBerths(int n, map<string, priority_queue<Passenger, vector<Passenger>,
AgeClassComparator>>& classPassengers,
map<string, int>& availableLowerBerths, map<string, int>& availableUpperBerths,
const string& selectedDate, const string& selectedTime) {
// Map to track allocated seat numbers for each class and berth type
map<string, int> lowerBerthSeatNumbers;
map<string, int> upperBerthSeatNumbers;
// Vector to hold unallocated passengers
vector<Passenger> unallocatedPassengers;
// Loop through each class
for (auto& cp : classPassengers) {
const string& className = cp.first;
priority_queue<Passenger, vector<Passenger>, AgeClassComparator>& pq =
cp.second;
// Initialize seat numbers for lower and upper berths
lowerBerthSeatNumbers[className] = 1;
upperBerthSeatNumbers[className] = 1;
// Loop through passengers in the priority queue
while (!pq.empty() && (availableLowerBerths[className] > 0 ||
availableUpperBerths[className] > 0)) {
Passenger passenger = pq.top();
pq.pop();
// Allocate lower berth to the passenger if available and priority condition is met
if (availableLowerBerths[className] > 0) {
passenger.berth = 'L';
passenger.seatNumber = lowerBerthSeatNumbers[className]++;
availableLowerBerths[className]--;
cout << "Allocates seat to " << passenger.name << " in " << className << "(Seat: " << passenger.seatNumber << ", Berth: Lower) on " << selectedDate << " at " <<selectedTime << endl;
}
// Allocate upper berth to the passenger if available and priority condition is not met for lower berth
else if (availableUpperBerths[className] > 0) {
passenger.berth = 'U';
passenger.seatNumber = upperBerthSeatNumbers[className]++;
availableUpperBerths[className]--;cout << "Allocates seat to " << passenger.name << " in " << className << "(Seat: " << passenger.seatNumber << ", Berth: Upper) on " << selectedDate << " at " <<selectedTime << endl;
}
// Output if no berths are available
else {
cout << "No berth available for " << passenger.name << " in " << className << endl;
unallocatedPassengers.push_back(passenger);
}
}
}
// Print unallocated passengers
if (!unallocatedPassengers.empty()) {
cout << "Unallocated passengers:" << endl;
for (const auto& passenger : unallocatedPassengers) {
cout << passenger.name << " (" << passenger.age << " years)" << endl;
}
} else {
cout << "All passengers have been allocated berths." << endl;
}
}
int main() {
int c = 1;
do {
// Seed random number generator for demo purposes
srand(time(0));
int totalPassengers; // Number of passengers including family members
cout << "Enter the total number of passengers (including family members): ";
cin >> totalPassengers;
string selectedTime;
string selectedDate;
// Take the selected date input
cout << "Enter the date at which you wish to travel (DD-MM-YYYY): ";
cin >> selectedDate;
// Validate the selected date against the current date
if (!isValidBooking(selectedDate, selectedTime)) {
cout << "Do you want to continue booking? (0, 1): ";
cin >> c;
continue; // Skip the rest of the current iteration and go to the next one
}
// Take the selected time input
int timeChoice;
do {
cout << "Select the time:\n";
cout << "1. 07:00\n";
cout << "2. 11:00\n";
cout << "3. 17:00\n";
cout << "4. 22:00\n";
cout << "Enter your choice: ";
cin >> timeChoice;
if (timeChoice < 1 || timeChoice > 4) {
cout << "Invalid choice. Please try again." << endl;
}
} while (timeChoice < 1 || timeChoice > 4);
switch (timeChoice) {
case 1:
selectedTime = "07:00";
break;
case 2:
selectedTime = "11:00";
break;
case 3:
selectedTime = "17:00";
break;
case 4:
selectedTime = "22:00";
break;
default:
cout << "Invalid choice" << endl;
return 1;
}
// Take input for family travel
int isFamilyTravel;
cout << "Are you traveling with family? (0 = No, 1 = Yes): ";
cin >> isFamilyTravel;
int familyCount = 0;
if (isFamilyTravel) {
do {
cout << "Enter the count of family members: ";
cin >> familyCount;
if (familyCount >= totalPassengers) {
cout << "Family count should be less than total passengers. Please enter again." << endl;
}
} while (familyCount >= totalPassengers);
totalPassengers -= familyCount;
// Adjusting total passengers count based on family size
// Take input for family members
cout << "Enter details for family members:" << endl;
map<string, priority_queue<Passenger, vector<Passenger>,
AgeClassComparator>> classPassengers; // Declare map here
for (int i = 0; i < familyCount; ++i) {
int age;
cout << "Enter age for family member " << i + 1 << ": ";
cin >> age;
string name;
cout << "Enter name for family member " << i + 1 << ": ";
cin.ignore(); // Ignore newline character left in the input stream
getline(cin, name);
int choice;
do {
cout << "Select the class for family member " << i + 1 << ":\n";
cout << "1. Sleeper Class\n";
cout << "2. Third Tier AC (3A)\n";
cout << "3. Second Tier AC (2A)\n";
cout << "4. First Tier AC (1A)\n";
cout << "Enter your choice: ";
cin >> choice;
if (choice < 1 || choice > 4)
cout << "Invalid choice. Please try again." << endl;
} while (choice < 1 || choice > 4);
string className;
switch (choice) {
case 1:
className = "Sleeper";
break;
case 2:
className = "Third Tier AC (3A)";
break;
case 3:
className = "Second Tier AC (2A)";
break;
case 4:
className = "First Tier AC (1A)";
break;
}
classPassengers[className].push(Passenger(age, name, className, ' ', 0, i +1)); // Use family member index as unique id
}
// Initialize available berths for each class
map<string, int> availableLowerBerths;
map<string, int> availableUpperBerths;
for (const auto& cp : classPassengers) {
const string& className = cp.first;
availableLowerBerths[className] = 5; // 5 lower berths or available passengers, whichever is smaller
availableUpperBerths[className] = 5; // Remaining seats are upper berths
}
// Allocate berths to family members
allocateBerths(familyCount, classPassengers, availableLowerBerths,
availableUpperBerths, selectedDate, selectedTime);
}
// Proceed with input for remaining passengers
map<string, priority_queue<Passenger, vector<Passenger>,
AgeClassComparator>> classPassengers; // Moved declaration here
for (int i = 0; i < totalPassengers; ++i) {
int age;
cout << "Enter age for passenger " << i + 1 << ": ";
cin >> age;
string Name;
cout << "Enter name for passenger " << i + 1 << ": ";
cin.ignore();
getline(cin, Name);
string className;
int choice;
do {
cout << "Select the class:\n";
cout << "1. Sleeper Class\n";
cout << "2. Third Tier AC (3A)\n";
cout << "3. Second Tier AC (2A)\n";
cout << "4. First Tier AC (1A)\n";
cout << "Enter your choice: ";
cin >> choice;
if (choice < 1 || choice > 4)
cout << "Invalid choice. Please try again." << endl;
} while (choice < 1 || choice > 4);
switch (choice) {
case 1:
className = "Sleeper";
break;
case 2:
className = "Third Tier AC (3A)";
break;
case 3:
className = "Second Tier AC (2A)";
break;
case 4:
className = "First Tier AC (1A)";
break;
}
classPassengers[className].push(Passenger(age, Name, className, ' ', 0, i + 1));
// Use passenger index as unique id
}
// Initialize available berths for each class
map<string, int> availableLowerBerths;
map<string, int> availableUpperBerths;
for (const auto& cp : classPassengers) {
const string& className = cp.first;
availableLowerBerths[className] = 5; // 5 lower berths or available passengers, whichever is smaller
availableUpperBerths[className] = 5; // Remaining seats are upper berths
}
// Allocate berths to remaining passengers
allocateBerths(totalPassengers, classPassengers, availableLowerBerths,
availableUpperBerths, selectedDate, selectedTime);
cout << "Do you want to continue booking? (0, 1): ";
cin >> c;
} while(c);
return 0;
}
