HOSTEL COMPLAINT RESOLUTION SYSTEM (DS Project in C)
====================================================

Files
-----
main.c        Entry point (calls complaintMenu)
complaint.h   complaint struct + function declarations
complaint.c   Circular queue, max-heap priority queue, linked list history,
              hash table search, CSV file handling, menu
route.h       Campus map constants + Dijkstra declarations
route.c       Campus graph (adjacency matrix) + Dijkstra shortest route
hostel.exe    Ready-to-run Windows build

Run (Windows)
-------------
Double-click hostel.exe, or from a terminal in this folder:
    hostel

Build from source
-----------------
    gcc main.c complaint.c route.c -o hostel

Data
----
Complaints are saved in data\complaints.csv. The data folder is created
automatically next to where the program is run, and complaints are
reloaded every time the program starts.

Menu
----
1. Register Complaint
2. View Pending Complaints
3. Resolve Next Complaint   (also shows the Dijkstra route to the room's block)
4. View Resolved History
5. Search Complaint by ID
6. Find Shortest Route (Dijkstra)
7. Exit

Room numbers: 1xx = Block A, 2xx = Block B, 3xx = Block C, 4xx = Block D.

See L1-notes-hostel-complaint-system.pdf for full explanation notes.
