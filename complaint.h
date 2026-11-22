#ifndef COMPLAINT_H
#define COMPLAINT_H

typedef struct
{
    int complaintId;
    int studentId;
    int priority;
    int roomno;
    char status[100];
    char description[100];
} complaint;

/* Input Helper */
int readInt(int *value);

/* Normal FIFO (Circular) Queue Functions */
void initializeQueue(void);
int isQueueEmpty(void);
int isQueueFull(void);
int enqueue(complaint c);
complaint dequeue(void);
void displayNormalQueue(void);

/* Priority Queue Functions */
void initializePriorityQueue(void);
int isPriorityQueueFull(void);
int hasHigherPriority(complaint a, complaint b);
void swapComplaint(complaint *a, complaint *b);
int insertPriority(complaint c);
void heapifyDown(int i);
complaint deleteHighestPriority(void);
void displayPriorityQueue(void);

/* Complaint History Functions */
void addToHistory(complaint c);
void displayHistory(void);
void freeHistory(void);

/* Hash Table Functions */
int hashFunction(int complaintId);
void insertHash(complaint c);
int searchHash(int complaintId);
void displayComplaintById(int complaintId);

/* File Handling Functions */
void ensureDataFolder(void);
void loadComplaintsFromFile(void);
int saveComplaintToFile(complaint c);
int updateComplaintStatus(int id, const char *newStatus);

/* Complaint Management Functions */
void addComplaint(void);
void resolveComplaint(void);
void displayComplaints(void);
void complaintMenu(void);

#endif
