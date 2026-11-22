#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complaint.h"
#include "route.h"

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MAKE_DIR(path) mkdir(path, 0777)
#endif

#define MAX_QUEUE 100
#define MAX_HEAP 100
#define HASH_SIZE 1009 /* prime, large enough for pending + resolved complaints */

#define HIGH 3
#define MEDIUM 2
#define LOW 1

#define DATA_FOLDER "data"
#define DATA_FILE "data/complaints.csv"
#define TEMP_FILE "data/complaints.tmp"
#define BACKUP_FILE "data/complaints.bak"

/* circular queue: count tells how many items are stored */
typedef struct
{
    complaint data[MAX_QUEUE];
    int front;
    int rear;
    int count;
} queue;

typedef struct
{
    complaint data[MAX_HEAP];
    int size;
} PriorityQueue;
PriorityQueue priorityQueue;
typedef struct historyNode
{
    complaint data;
    struct historyNode *next;
} historyNode;

// global variables
queue normalQueue;
historyNode *historyHead = NULL;
int nextComplaintId = 1;
complaint hashTable[HASH_SIZE];
int hashUsed[HASH_SIZE] = {0};

/* reads a whole line and converts it to an int; returns 0 on bad input */
int readInt(int *value)
{
    char buffer[64];
    char *end;
    long number;
    int ch;
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return 0;
    }
    if (strchr(buffer, '\n') == NULL)
    {
        /* line was too long: throw away the rest of it */
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }
    number = strtol(buffer, &end, 10);
    if (end == buffer)
    {
        return 0;
    }
    while (*end == ' ' || *end == '\t')
    {
        end++;
    }
    if (*end != '\n' && *end != '\r' && *end != '\0')
    {
        return 0;
    }
    *value = (int)number;
    return 1;
}

int hashFunction(int complaintId)
{
    return complaintId % HASH_SIZE;
}

void insertHash(complaint c)
{
    int index = hashFunction(c.complaintId);
    int start = index;

    do
    {
        if (hashUsed[index] == 1 &&
            hashTable[index].complaintId == c.complaintId)
        {
            hashTable[index] = c;
            return;
        }
        if (hashUsed[index] == 0)
        {
            hashTable[index] = c;
            hashUsed[index] = 1;
            return;
        }
        index = (index + 1) % HASH_SIZE;

    } while (index != start);

    printf("\nHash table is full!\n");
}

int searchHash(int complaintId)
{
    int index = hashFunction(complaintId);
    int start = index;
    do
    {
        if (hashUsed[index] == 0)
        {
            return -1;
        }
        if (hashTable[index].complaintId == complaintId)
        {
            return index;
        }
        index = (index + 1) % HASH_SIZE;
    } while (index != start);
    return -1;
}
void displayComplaintById(int complaintId)
{
    int index = searchHash(complaintId);
    complaint c;
    if (index == -1)
    {
        printf("\nComplaint ID %d not found.\n", complaintId);
        return;
    }
    c = hashTable[index];
    printf("\n====== COMPLAINT DETAILS ======\n");
    printf("Complaint ID: %d\n", c.complaintId);
    printf("Student ID: %d\n", c.studentId);
    printf("Room Number: %d\n", c.roomno);
    printf("Description: %s\n", c.description);
    printf("Priority: %d\n", c.priority);
    printf("Status: %s\n", c.status);
}

void initializeQueue()
{
    normalQueue.front = 0;
    normalQueue.rear = -1;
    normalQueue.count = 0;
}

int isQueueEmpty()
{
    return normalQueue.count == 0;
}

int isQueueFull()
{
    return normalQueue.count == MAX_QUEUE;
}

int enqueue(complaint c)
{
    if (isQueueFull())
    {
        printf("\nNormal complaint queue is full!\n");
        return 0;
    }
    normalQueue.rear = (normalQueue.rear + 1) % MAX_QUEUE;
    normalQueue.data[normalQueue.rear] = c;
    normalQueue.count++;
    return 1;
}
complaint dequeue()
{
    complaint emptyComplaint;
    emptyComplaint.complaintId = -1;
    if (isQueueEmpty())
    {
        printf("\nNormal complaint queue is empty!\n");
        return emptyComplaint;
    }
    complaint c = normalQueue.data[normalQueue.front];
    normalQueue.front = (normalQueue.front + 1) % MAX_QUEUE;
    normalQueue.count--;
    return c;
}

void displayNormalQueue()
{
    int i;
    int k;
    if (isQueueEmpty())
    {
        printf("\nNo routine complaints are pending.\n");
        return;
    }
    printf("\n========== ROUTINE COMPLAINTS ==========\n");
    for (k = 0; k < normalQueue.count; k++)
    {
        i = (normalQueue.front + k) % MAX_QUEUE;
        printf("\nComplaint ID: %d", normalQueue.data[i].complaintId);
        printf("\nStudent ID: %d", normalQueue.data[i].studentId);
        printf("\nRoom Number: %d", normalQueue.data[i].roomno);
        printf("\nProblem: %s", normalQueue.data[i].description);
        printf("\nPriority: %d", normalQueue.data[i].priority);
        printf("\nStatus: %s\n", normalQueue.data[i].status);
    }
}

void initializePriorityQueue()
{
    priorityQueue.size = 0;
}

int isPriorityQueueFull()
{
    return priorityQueue.size == MAX_HEAP;
}

/* higher priority wins; on a tie the older complaint (smaller ID) wins */
int hasHigherPriority(complaint a, complaint b)
{
    if (a.priority != b.priority)
    {
        return a.priority > b.priority;
    }
    return a.complaintId < b.complaintId;
}

void swapComplaint(complaint *a, complaint *b)
{
    complaint temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int insertPriority(complaint c)
{
    int i;
    int parent;
    if (isPriorityQueueFull())
    {
        printf("\nPriority queue is full!\n");
        return 0;
    }
    i = priorityQueue.size;
    priorityQueue.data[i] = c;
    priorityQueue.size++;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (!hasHigherPriority(priorityQueue.data[i], priorityQueue.data[parent]))
        {
            break;
        }
        swapComplaint(&priorityQueue.data[parent], &priorityQueue.data[i]);
        i = parent;
    }
    return 1;
}

void heapifyDown(int i)
{
    int left;
    int right;
    int largest;
    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;
        if (left < priorityQueue.size && hasHigherPriority(priorityQueue.data[left], priorityQueue.data[largest]))
        {
            largest = left;
        }
        if (right < priorityQueue.size && hasHigherPriority(priorityQueue.data[right], priorityQueue.data[largest]))
        {
            largest = right;
        }
        if (largest == i)
        {
            break;
        }

        swapComplaint(&priorityQueue.data[i], &priorityQueue.data[largest]);
        i = largest;
    }
}
complaint deleteHighestPriority()
{
    complaint emptyComplaint;
    emptyComplaint.complaintId = -1;
    if (priorityQueue.size == 0)
    {
        printf("\nNo emergency complaints pending\n");
        return emptyComplaint;
    }
    complaint highest = priorityQueue.data[0];

    priorityQueue.data[0] = priorityQueue.data[priorityQueue.size - 1];

    priorityQueue.size--;

    if (priorityQueue.size > 0)
    {
        heapifyDown(0);
    }
    return highest;
}

/* shows emergencies in the order they will be resolved (heap array is not sorted) */
void displayPriorityQueue()
{
    complaint sorted[MAX_HEAP];
    complaint key;
    int i;
    int j;
    if (priorityQueue.size == 0)
    {
        printf("\nNo emergency complaints pending.\n");
        return;
    }
    for (i = 0; i < priorityQueue.size; i++)
    {
        key = priorityQueue.data[i];
        j = i - 1;
        while (j >= 0 && hasHigherPriority(key, sorted[j]))
        {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }
    printf("\n========== EMERGENCY COMPLAINTS ==========\n");
    for (i = 0; i < priorityQueue.size; i++)
    {
        printf("\nComplaint ID: %d", sorted[i].complaintId);
        printf("\nStudent ID: %d", sorted[i].studentId);
        printf("\nRoom Number: %d", sorted[i].roomno);
        printf("\nProblem: %s", sorted[i].description);
        printf("\nPriority: %d", sorted[i].priority);
        printf("\nStatus: %s\n", sorted[i].status);
    }
}
void addToHistory(complaint c)
{
    historyNode *newNode;

    newNode = (historyNode *)malloc(sizeof(historyNode));

    if (newNode == NULL)
    {
        printf("\nMemory allocation failed!\n");
        return;
    }
    newNode->data = c;
    newNode->next = historyHead;
    historyHead = newNode;
}
void displayHistory()
{
    historyNode *temp;
    if (historyHead == NULL)
    {
        printf("\nNo resolved complaints yet.\n");
        return;
    }
    temp = historyHead;
    printf("\n========== RESOLVED COMPLAINT HISTORY ==========\n");
    while (temp != NULL)
    {
        printf("\nComplaint ID: %d", temp->data.complaintId);
        printf("\nStudent ID: %d", temp->data.studentId);
        printf("\nRoom Number: %d", temp->data.roomno);
        printf("\nProblem: %s", temp->data.description);
        printf("\nPriority: %d", temp->data.priority);
        printf("\nStatus: %s\n", temp->data.status);
        temp = temp->next;
    }
}
void freeHistory()
{
    historyNode *temp;
    while (historyHead != NULL)
    {
        temp = historyHead;
        historyHead = historyHead->next;
        free(temp);
    }
}

void ensureDataFolder(void)
{
    /* fails harmlessly if the folder already exists */
    MAKE_DIR(DATA_FOLDER);
}

/* rebuilds queue, heap, hash table and history from the CSV file */
void loadComplaintsFromFile(void)
{
    FILE *fp;
    char line[512];
    complaint c;
    int maxId = 0;
    int loaded = 0;
    fp = fopen(DATA_FILE, "r");
    if (fp == NULL)
    {
        nextComplaintId = 1;
        return;
    }
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "%d,%d,%d,%99[^,],%d,%99[^\r\n]", &c.complaintId, &c.studentId, &c.roomno, c.description, &c.priority, c.status) != 6)
        {
            continue;
        }
        if (c.complaintId > maxId)
        {
            maxId = c.complaintId;
        }
        insertHash(c);
        if (strcmp(c.status, "Resolved") == 0)
        {
            addToHistory(c);
        }
        else if (c.priority == HIGH)
        {
            if (!insertPriority(c))
            {
                printf("Complaint %d could not be loaded.\n", c.complaintId);
            }
        }
        else
        {
            if (!enqueue(c))
            {
                printf("Complaint %d could not be loaded.\n", c.complaintId);
            }
        }
        loaded++;
    }
    fclose(fp);
    nextComplaintId = maxId + 1;
    if (loaded > 0)
    {
        printf("\nLoaded %d complaint(s) from %s\n", loaded, DATA_FILE);
    }
}
int saveComplaintToFile(complaint c)
{
    FILE *fp;
    fp = fopen(DATA_FILE, "a");
    if (fp == NULL)
    {
        printf("\nUnable to open complaints.csv\n");
        return 0;
    }
    if (fprintf(fp, "%d,%d,%d,%s,%d,%s\n", c.complaintId, c.studentId, c.roomno, c.description, c.priority, c.status) < 0)
    {
        fclose(fp);
        printf("\nError writing complaint to file!\n");
        return 0;
    }
    if (fclose(fp) != 0)
    {
        printf("\nError closing complaint file!\n");
        return 0;
    }
    return 1;
}
int updateComplaintStatus(int id, const char *newStatus)
{
    FILE *fp, *temp;
    char line[512];
    int found = 0;
    int error = 0;
    fp = fopen(DATA_FILE, "r");
    if (fp == NULL)
    {
        printf("\nUnable to open complaints.csv!\n");
        return 0;
    }
    temp = fopen(TEMP_FILE, "w");

    if (temp == NULL)
    {
        fclose(fp);
        printf("\nUnable to create temporary file!\n");
        return 0;
    }
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int complaintId, studentId, roomno, priority;
        char description[100], status[100];

        if (sscanf(line, "%d,%d,%d,%99[^,],%d,%99[^\r\n]", &complaintId, &studentId, &roomno, description, &priority, status) == 6)
        {
            if (complaintId == id)
            {
                snprintf(status, sizeof(status), "%s", newStatus);
                found = 1;
            }

            if (fprintf(temp, "%d,%d,%d,%s,%d,%s\n", complaintId, studentId, roomno, description, priority, status) < 0)
            {
                error = 1;
                break;
            }
        }
        else
        {
            if (fputs(line, temp) == EOF)
            {
                error = 1;
                break;
            }
        }
    }
    if (ferror(fp))
        error = 1;
    if (fclose(fp) != 0)
        error = 1;
    if (fclose(temp) != 0)
        error = 1;
    if (error || !found)
    {
        remove(TEMP_FILE);
        if (!found && !error)
            printf("\nComplaint ID not found in CSV file!\n");
        else
            printf("\nError updating complaint file!\n");
        return 0;
    }
    remove(BACKUP_FILE);
    if (rename(DATA_FILE, BACKUP_FILE) != 0)
    {
        remove(TEMP_FILE);
        printf("\nUnable to back up complaints.csv!\n");
        return 0;
    }
    if (rename(TEMP_FILE, DATA_FILE) != 0)
    {
        rename(BACKUP_FILE, DATA_FILE);
        remove(TEMP_FILE);
        printf("\nUnable to replace complaints.csv!\n");
        return 0;
    }
    remove(BACKUP_FILE);
    return 1;
}
void addComplaint()
{
    complaint c;
    int type;
    printf("\n========== REGISTER COMPLAINT ==========\n");
    printf("Enter Student ID: ");
    if (!readInt(&c.studentId) || c.studentId <= 0)
    {
        printf("\nInvalid Student ID!\n");
        return;
    }
    printf("Enter Room Number: ");
    if (!readInt(&c.roomno) || c.roomno <= 0)
    {
        printf("\nInvalid Room Number!\n");
        return;
    }
    printf("\nSelect Complaint Type:\n");
    printf("1. Main door handle broken\n");
    printf("2. Geyser not working\n");
    printf("3. Short circuit in switch board\n");
    printf("4. Toilet flush damaged\n");
    printf("5. Room cleaning\n");
    printf("6. Fan in the hall not working\n");
    printf("7. Electricity spark in the geyser\n");
    printf("8. Wi-Fi not working\n");
    printf("9. Tubelight fused\n");
    printf("10. Termite infestation in the almirah\n");
    printf("11. Roommate and study problem\n");
    printf("12. Security and leave management problem\n");
    printf("13. Laundry management\n");
    printf("14. Lack of emergency support\n");
    printf("15. Difficulty reporting harassment or ragging\n");

    printf("\nEnter your choice (1-15): ");
    if (!readInt(&type))
    {
        printf("\nInvalid input! Please enter a number.\n");
        return;
    }
    switch (type)
    {
    case 1:
        strcpy(c.description, "Main door handle broken");
        c.priority = MEDIUM;
        break;

    case 2:
        strcpy(c.description, "Geyser not working");
        c.priority = MEDIUM;
        break;

    case 3:
        strcpy(c.description, "Short circuit in switch board");
        c.priority = HIGH;
        break;

    case 4:
        strcpy(c.description, "Toilet flush damaged");
        c.priority = MEDIUM;
        break;

    case 5:
        strcpy(c.description, "Room cleaning");
        c.priority = LOW;
        break;

    case 6:
        strcpy(c.description, "Fan in the hall not working");
        c.priority = MEDIUM;
        break;

    case 7:
        strcpy(c.description, "Electricity spark in the geyser");
        c.priority = HIGH;
        break;

    case 8:
        strcpy(c.description, "Wi-Fi not working");
        c.priority = MEDIUM;
        break;

    case 9:
        strcpy(c.description, "Tubelight fused");
        c.priority = MEDIUM;
        break;

    case 10:
        strcpy(c.description, "Termite infestation in the almirah");
        c.priority = MEDIUM;
        break;

    case 11:
        strcpy(c.description, "Roommate and study problem");
        c.priority = LOW;
        break;

    case 12:
        strcpy(c.description, "Security and leave management problem");
        c.priority = MEDIUM;
        break;

    case 13:
        strcpy(c.description, "Laundry management");
        c.priority = LOW;
        break;

    case 14:
        strcpy(c.description, "Lack of emergency support");
        c.priority = HIGH;
        break;

    case 15:
        strcpy(c.description, "Difficulty reporting harassment or ragging");
        c.priority = HIGH;
        break;

    default:
        printf("\nInvalid complaint type! Please select 1-15.\n");
        return;
    }
    c.complaintId = nextComplaintId;
    strcpy(c.status, "Pending");

    /* check space first so nothing has to be undone after saving */
    if ((c.priority == HIGH && isPriorityQueueFull()) ||
        (c.priority != HIGH && isQueueFull()))
    {
        printf("\nComplaint queue is full! Complaint registration failed.\n");
        return;
    }
    if (!saveComplaintToFile(c))
    {
        printf("\nComplaint could not be saved. Registration failed.\n");
        return;
    }
    if (c.priority == HIGH)
    {
        insertPriority(c);
        printf("\nEmergency complaint added to priority queue.\n");
    }
    else
    {
        enqueue(c);
        printf("\nComplaint added to normal FIFO queue.\n");
    }
    insertHash(c);

    nextComplaintId++;
    printf("\nComplaint registered successfully!");
    printf("\nComplaint ID: %d", c.complaintId);
    printf("\nProblem: %s", c.description);

    printf("\nPriority: ");

    if (c.priority == HIGH)
        printf("HIGH");
    else if (c.priority == MEDIUM)
        printf("MEDIUM");
    else
        printf("LOW");

    printf("\nStatus: %s\n", c.status);
}

void resolveComplaint()
{
    complaint c;
    if (priorityQueue.size > 0)
    {
        c = priorityQueue.data[0];
    }
    else if (!isQueueEmpty())
    {
        c = normalQueue.data[normalQueue.front];
    }
    else
    {
        printf("\nNo pending complaints.\n");
        return;
    }
    printf("\n========== COMPLAINT TO RESOLVE ==========\n");
    printf("Complaint ID: %d\n", c.complaintId);
    printf("Student ID: %d\n", c.studentId);
    printf("Room Number: %d\n", c.roomno);
    printf("Problem: %s\n", c.description);
    showRouteToRoom(c.roomno); /* Dijkstra: shortest route for the staff */
    if (!updateComplaintStatus(c.complaintId, "Resolved"))
    {
        printf("\nCould not save resolution.\n");
        printf("Complaint remains pending.\n");
        return;
    }
    if (priorityQueue.size > 0)
    {
        c = deleteHighestPriority();
    }
    else
    {
        c = dequeue();
    }
    strcpy(c.status, "Resolved");
    insertHash(c); /* updates the existing entry */
    addToHistory(c);
    printf("\nComplaint %d has been resolved.\n", c.complaintId);
    printf("Status saved in complaints.csv.\n");
}
void displayComplaints()
{
    printf("\n========================================");
    printf("\n       PENDING COMPLAINTS");
    printf("\n========================================");

    displayPriorityQueue();

    displayNormalQueue();
}
void complaintMenu()
{
    int choice;
    initializeQueue();
    initializePriorityQueue();
    initializeCampusMap();
    ensureDataFolder();
    loadComplaintsFromFile();

    do
    {
        printf("\n\n=================================");
        printf("\n     COMPLAINT RESOLUTION");
        printf("\n=================================");

        printf("\n1. Register Complaint");
        printf("\n2. View Pending Complaints");
        printf("\n3. Resolve Next Complaint");
        printf("\n4. View Resolved History");
        printf("\n5. Search Complaint by ID");
        printf("\n6. Find Shortest Route (Dijkstra)");
        printf("\n7. Exit Complaint Module");

        printf("\n\nEnter your choice: ");
        if (!readInt(&choice))
        {
            if (feof(stdin))
            {
                choice = 7; /* input closed: exit cleanly */
            }
            else
            {
                choice = 0; /* falls to "Invalid choice!" */
            }
        }
        switch (choice)
        {
        case 1:
            addComplaint();
            break;

        case 2:
            displayComplaints();
            break;

        case 3:
            resolveComplaint();
            break;

        case 4:
            displayHistory();
            break;

        case 5:
        {
            int id;
            printf("\nEnter Complaint ID to search: ");
            if (!readInt(&id))
            {
                printf("\nInvalid Complaint ID!\n");
                break;
            }
            displayComplaintById(id);
            break;
        }
        case 6:
            routeMenu();
            break;

        case 7:
            printf("\nExiting Complaint Module...\n");
            break;

        default:
            printf("\nInvalid choice!\n");
        }

    } while (choice != 7);
    freeHistory();
}
