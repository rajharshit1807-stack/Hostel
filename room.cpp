#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
const int HASH_SIZE = 10;

class Student
{
public:
    int id;
    string name;

    void input()
    {
        cout << "Enter Student ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Student Name: ";
        getline(cin, name);
    }

    void display()
    {
        cout << "Student ID: " << id << endl;
        cout << "Name: " << name << endl;
    }
};

class HashTable
{
private:
    struct Node
    {
        int id;
        int roomIndex;
        Node* next;

        Node(int studentID, int room)
        {
            id = studentID;
            roomIndex = room;
            next = NULL;
        }
    };

    Node* table[HASH_SIZE];

    int hashFunction(int id)
    {
        return id % HASH_SIZE;
    }

public:
    HashTable()
    {
        for (int i = 0; i < HASH_SIZE; i++)
            table[i] = NULL;
    }

    int search(int id)
    {
        int index = hashFunction(id);
        Node* current = table[index];

        while (current != NULL)
        {
            if (current->id == id)
                return current->roomIndex;

            current = current->next;
        }

        return -1;
    }

    bool insert(int id, int roomIndex)
    {
        if (search(id) != -1)
            return false;

        int index = hashFunction(id);
        Node* newNode = new Node(id, roomIndex);
        newNode->next = table[index];
        table[index] = newNode;

        return true;
    }

    void remove(int id)
    {
        int index = hashFunction(id);
        Node* current = table[index];
        Node* previous = NULL;

        while (current != NULL)
        {
            if (current->id == id)
            {
                if (previous == NULL)
                    table[index] = current->next;
                else
                    previous->next = current->next;

                delete current;
                return;
            }

            previous = current;
            current = current->next;
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < HASH_SIZE; i++)
        {
            Node* current = table[i];

            while (current != NULL)
            {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
    }
};

class Room
{
private:
    string roomNo;
    int capacity;
    int occupied;
    bool initialized = false;
    Student students[5];

public:
    Room()
{
    roomNo = "";
    capacity = 0;
    occupied = 0;
    initialized = false;
}
    
    void inputRoom(HashTable& hashTable, int roomIndex)
    {
        cout << "\nEnter Room Number: ";
        cin >> roomNo;

        cout << "Enter Room Capacity (1-5): ";
        cin >> capacity;

        while (capacity < 1 || capacity > 5)
        {
            cout << "Enter valid capacity (1-5): ";
            cin >> capacity;
        }

        cout << "Enter Number of Students: ";
cin >> occupied;

while (occupied < 0 || occupied > capacity)
{
    cout << "Invalid number of students!";
    cout << "\nEnter students between 0 and "
         << capacity << ": ";

    cin >> occupied;
}

        for (int i = 0; i < occupied; i++)
        {
            cout << "\nStudent " << i + 1 << endl;
            students[i].input();

            while (hashTable.search(students[i].id) != -1)
            {
                cout << "This ID already exists. Enter another ID.\n";
                students[i].input();
            }

            hashTable.insert(students[i].id, roomIndex);
        }
    }

    int availableBeds()
    {
        return capacity - occupied;
    }

    bool vacant()
    {
        return occupied == 0;
    }

    bool full()
    {
        return occupied == capacity;
    }

    void displayRoom()
    {
        cout << "\n-----------------------------\n";
        cout << "Room: " << roomNo << endl;
        cout << "Capacity: " << capacity << endl;
        cout << "Occupied: " << occupied << endl;
        cout << "Available Beds: " << availableBeds() << endl;

        if (vacant())
            cout << "Status: Vacant\n";
        else if (full())
            cout << "Status: Full\n";
        else
            cout << "Status: Partially Occupied\n";

        cout << "-----------------------------\n";
    }

    void saveToCSV(ofstream& file)
    {
        for (int i = 0; i < occupied; i++)
    {
        file << roomNo << ","
             << capacity << ","
             << occupied << ","
             << students[i].id << ","
             << students[i].name << "\n";
    }

    if (occupied == 0)
    {
        file << roomNo << ","
             << capacity << ","
             << occupied << ",,"
             << "\n";
    }
}
    void loadStudent(string rNo, int cap, int studentID,
                 string studentName, HashTable& hashTable,
                 int roomIndex)
{
    if (!initialized)
    {
        roomNo = rNo;
        capacity = cap;
        occupied = 0;
        initialized = true;
    }

    if (occupied >= capacity || occupied >= 5)
        return;

    if (!hashTable.insert(studentID, roomIndex))
        return;

    students[occupied].id = studentID;
    students[occupied].name = studentName;
    occupied++;
}
    void loadEmptyRoom(string rNo, int cap)
{
    if (!initialized)
    {
        roomNo = rNo;
        capacity = cap;
        occupied = 0;
        initialized = true;
    }
}
    string getRoomNo()
{
    return roomNo;
}

    void showStudents()
    {
        cout << "\nRoommates of " << roomNo << ":\n";

        if (occupied == 0)
        {
            cout << "No students in this room.\n";
            return;
        }

        for (int i = 0; i < occupied; i++)
        {
            cout << "\nStudent " << i + 1 << endl;
            students[i].display();
        }
    }

    void showStudent(int id)
    {
        for (int i = 0; i < occupied; i++)
        {
            if (students[i].id == id)
            {
                students[i].display();
                cout << "Room Number: " << roomNo << endl;
                return;
            }
        }

        cout << "Student record not found in this room.\n";
    }

    void addStudent(HashTable& hashTable, int roomIndex)
    {
        if (full())
        {
            cout << "Room is full.\n";
            return;
        }

        cout << "\nEnter new student details:\n";
        students[occupied].input();

        while (hashTable.search(students[occupied].id) != -1)
        {
            cout << "This ID already exists. Enter another ID.\n";
            students[occupied].input();
        }

        hashTable.insert(students[occupied].id, roomIndex);
        occupied++;

        cout << "Student added successfully.\n";
        cout << "Available Beds: " << availableBeds() << endl;
    }

    void removeStudent(HashTable& hashTable)
    {
        if (occupied == 0)
        {
            cout << "No students in this room.\n";
            return;
        }

        int id;
        cout << "Enter Student ID to remove: ";
        cin >> id;

        int pos = -1;

        for (int i = 0; i < occupied; i++)
        {
            if (students[i].id == id)
            {
                pos = i;
                break;
            }
        }

        if (pos == -1)
        {
            cout << "Student not found in this room.\n";
            return;
        }

        hashTable.remove(id);

        for (int i = pos; i < occupied - 1; i++)
            students[i] = students[i + 1];

        occupied--;

        cout << "Student removed successfully.\n";
        cout << "Available Beds: " << availableBeds() << endl;
    }
};

class LeaveRequest
{
public:
    int studentID;
    string reason;
};

class LeaveQueue
{
private:
    LeaveRequest requests[100];
    int front;
    int rear;
    int count;

public:
    LeaveQueue()
    {
        front = 0;
        rear = -1;
        count = 0;
    }

    void submitRequest(HashTable& hashTable)
    {
        if (count == 100)
        {
            cout << "Leave request queue is full.\n";
            return;
        }

        int id;
        cout << "Enter Student ID: ";
        cin >> id;

        if (hashTable.search(id) == -1)
        {
            cout << "Student not found. Request rejected.\n";
            return;
        }

        cin.ignore(1000, '\n');

        requests[++rear].studentID = id;

        cout << "Enter leave reason: ";
        getline(cin, requests[rear].reason);

        count++;

        cout << "Leave request submitted successfully.\n";
    }

    void displayRequests()
    {
        if (count == 0)
        {
            cout << "No pending leave requests.\n";
            return;
        }

        cout << "\n===== PENDING LEAVE REQUESTS =====\n";

        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % 100;

            cout << "\nStudent ID: "
                 << requests[index].studentID << endl;

            cout << "Reason: "
                 << requests[index].reason << endl;
        }
    }

    
    void processRequest()
    {
    if (count == 0)
    {
        cout << "No pending requests to process.\n";
        return;
    }

    cout << "\n===== LEAVE REQUEST =====\n";
    cout << "Student ID: "
         << requests[front].studentID << endl;
    cout << "Reason: "
         << requests[front].reason << endl;

    int decision;

    cout << "\n1. Approve Leave\n";
    cout << "2. Reject Leave\n";
    cout << "Enter decision: ";
    cin >> decision;

    if (decision != 1 && decision != 2)
    {
        cout << "Invalid decision. Request remains pending.\n";
        return;
    }

    
    string status;

    if (decision == 1)
    {
    status = "Approved";
    cout << "Leave request APPROVED.\n";
    }
    else
    {
    status = "Rejected";
    cout << "Leave request REJECTED.\n";
    }

    saveHistory(requests[front].studentID,
            requests[front].reason,
            status);


    front = (front + 1) % 100;
    count--;

    cout << "Request processed successfully.\n";
}

void saveHistory(int studentID, string reason, string status)
{
    ofstream file("leave_history.csv", ios::app);

    if (!file.is_open())
    {
        cout << "Unable to save leave history.\n";
        return;
    }

    file << studentID << ","
         << reason << ","
         << status << "\n";

    file.close();
    
    cout << "Leave history saved successfully.\n";
}


    
    void saveToCSV()
    {
    ofstream file("leave_requests.csv");

    if (!file.is_open())
    {
        cout << "Unable to save leave requests.\n";
        return;
    }

    file << "StudentID,Reason\n";

    for (int i = 0; i < count; i++)
    {
        int index = (front + i) % 100;

        file << requests[index].studentID << ","
             << requests[index].reason << "\n";
    }

    file.close();
    cout << "Leave requests saved successfully.\n";
}

void loadFromCSV()
{
    ifstream file("leave_requests.csv");

    if (!file.is_open())
        return;

    string line;
    getline(file, line); // Header skip karo

    while (getline(file, line))
    {
        if (count >= 100)
            break;

        stringstream ss(line);
        string idText;
        string reason;

        getline(ss, idText, ',');
        getline(ss, reason);

        if (idText.empty())
            continue;

        int id = stoi(idText);

        requests[rear = (rear + 1) % 100].studentID = id;
        requests[rear].reason = reason;
        count++;
    }

    file.close();
    cout << "Leave requests loaded successfully.\n";
}


};

bool loadRoomsFromCSV(Room rooms[], HashTable& hashTable, int& n)
{
    ifstream file("rooms.csv");

    if (!file.is_open())
        return false;

    string line;
    getline(file, line); // Header skip karo

    n = 0;

    while (getline(file, line))
    {
        stringstream ss(line);
        string roomNo, capacityText, occupiedText;
        string idText, name;

        getline(ss, roomNo, ',');
        getline(ss, capacityText, ',');
        getline(ss, occupiedText, ',');
        getline(ss, idText, ',');
        getline(ss, name);

        int capacity = stoi(capacityText);
        int occupied = stoi(occupiedText);

        int roomIndex = -1;

        for (int i = 0; i < n; i++)
        {
            if (rooms[i].getRoomNo() == roomNo)
            {
                roomIndex = i;
                break;
            }
        }

        if (roomIndex == -1)
        {
            if (n >= 20)
                break;

            roomIndex = n;
            n++;
        }

        if (idText.empty())
        {
            rooms[roomIndex].loadEmptyRoom(roomNo, capacity);
        }
        else
        {
            int studentID = stoi(idText);
            rooms[roomIndex].loadStudent(
                roomNo, capacity, studentID, name,
                hashTable, roomIndex
            );
        }
    }

    file.close();
    return n > 0;
}

int main()
{
    Room rooms[20];
    HashTable hashTable;
    LeaveQueue leaveQueue;
    leaveQueue.loadFromCSV();
    int n;

    cout << "====================================\n";
    cout << "   SMART HOSTEL MANAGEMENT SYSTEM\n";
    cout << "====================================\n";

    
if (loadRoomsFromCSV(rooms, hashTable, n))
{
    cout << "Room data loaded successfully from rooms.csv\n";
    cout << "Total rooms loaded: " << n << endl;
}
else
{
    cout << "No saved room data found. Enter new room details.\n";

    cout << "Enter number of rooms: ";
    cin >> n;

    while (n < 1 || n > 20)
    {
        cout << "Enter number between 1 and 20: ";
        cin >> n;
    }

    for (int i = 0; i < n; i++)
    {
        cout << "\n========== ROOM " << i + 1 << " ==========\n";
        rooms[i].inputRoom(hashTable, i);
    }
}


    int choice;

    do
    {
        cout << "\n========== ROOM MENU ==========\n";
        cout << "1. Display All Rooms\n";
        cout << "2. Show Roommates\n";
        cout << "3. Add Student\n";
        cout << "4. Remove Student\n";
        cout << "5. Show Available Rooms\n";
        cout << "6. Show Vacant Rooms\n";
        cout << "7. Search Student Using Hashing\n";
        cout << "8. Submit Leave Request\n";
        cout << "9. Display Pending Leave Requests\n";
        cout << "10. Process Leave Request\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            for (int i = 0; i < n; i++)
                rooms[i].displayRoom();
        }
        else if (choice >= 2 && choice <= 4)
        {
            int r;
            cout << "Enter room number (1-" << n << "): ";
            cin >> r;

            if (r < 1 || r > n)
            {
                cout << "Invalid room number.\n";
                continue;
            }

            if (choice == 2)
                rooms[r - 1].showStudents();
            else if (choice == 3)
                rooms[r - 1].addStudent(hashTable, r - 1);
            else
                rooms[r - 1].removeStudent(hashTable);
        }
        else if (choice == 5)
        {
            bool found = false;
            cout << "\n========== AVAILABLE ROOMS ==========\n";

            for (int i = 0; i < n; i++)
            {
                if (!rooms[i].full())
                {
                    rooms[i].displayRoom();
                    found = true;
                }
            }

            if (!found)
                cout << "No room is available.\n";
        }
        else if (choice == 6)
        {
            bool found = false;
            cout << "\n========== VACANT ROOMS ==========\n";

            for (int i = 0; i < n; i++)
            {
                if (rooms[i].vacant())
                {
                    rooms[i].displayRoom();
                    found = true;
                }
            }

            if (!found)
                cout << "No vacant room found.\n";
        }
        else if (choice == 7)
        {
            int id;
            cout << "Enter Student ID to search: ";
            cin >> id;

            int roomIndex = hashTable.search(id);

            if (roomIndex == -1)
                cout << "Student not found.\n";
            else
            {
                cout << "Student found!\n";
                rooms[roomIndex].showStudent(id);
            }
        }
        
        else if (choice == 8)
        {
            leaveQueue.submitRequest(hashTable);
        }
        else if (choice == 9)
        {
            leaveQueue.displayRequests();
        }
        else if (choice == 10)
        {
            leaveQueue.processRequest();
        }

        else if (choice == 0)
        {
            cout << "Exiting...\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);
    leaveQueue.saveToCSV();
    ofstream file("rooms.csv");

    if (file.is_open())
{
    file << "RoomNo,Capacity,Occupied,StudentID,StudentName\n";

    for (int i = 0; i < n; i++)
    {
        rooms[i].saveToCSV(file);
    }

    file.close();
    cout << "Room data saved to rooms.csv\n";
}
    else
{
    cout << "Unable to open CSV file.\n";
}

    return 0;
}
