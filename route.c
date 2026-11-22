#include <stdio.h>
#include <limits.h>
#include "route.h"
#include "complaint.h"

#define INF INT_MAX

/* adjacency matrix: campusMap[i][j] = walking distance, 0 = no direct path */
int campusMap[NUM_LOCATIONS][NUM_LOCATIONS];

const char *locations[NUM_LOCATIONS] = {
    "Maintenance Office",
    "Warden Office",
    "Security Gate",
    "Mess",
    "Block A (rooms 100-199)",
    "Block B (rooms 200-299)",
    "Block C (rooms 300-399)",
    "Block D (rooms 400-499)",
    "Laundry"};

/* indexes into locations[] */
#define WARDEN_OFFICE 1
#define SECURITY_GATE 2
#define MESS 3
#define BLOCK_A 4
#define BLOCK_B 5
#define BLOCK_C 6
#define BLOCK_D 7
#define LAUNDRY 8

void addPath(int from, int to, int distance)
{
    /* paths can be walked both ways: undirected graph */
    campusMap[from][to] = distance;
    campusMap[to][from] = distance;
}

void initializeCampusMap(void)
{
    int i;
    int j;
    for (i = 0; i < NUM_LOCATIONS; i++)
    {
        for (j = 0; j < NUM_LOCATIONS; j++)
        {
            campusMap[i][j] = 0;
        }
    }
    addPath(MAINTENANCE_OFFICE, WARDEN_OFFICE, 50);
    addPath(MAINTENANCE_OFFICE, MESS, 120);
    addPath(MAINTENANCE_OFFICE, LAUNDRY, 60);
    addPath(WARDEN_OFFICE, SECURITY_GATE, 80);
    addPath(WARDEN_OFFICE, BLOCK_A, 70);
    addPath(SECURITY_GATE, BLOCK_A, 90);
    addPath(SECURITY_GATE, BLOCK_D, 200);
    addPath(MESS, BLOCK_A, 100);
    addPath(MESS, BLOCK_B, 60);
    addPath(BLOCK_A, BLOCK_B, 80);
    addPath(BLOCK_B, BLOCK_C, 70);
    addPath(LAUNDRY, BLOCK_C, 150);
    addPath(BLOCK_C, BLOCK_D, 60);
}

const char *locationName(int location)
{
    if (location < 0 || location >= NUM_LOCATIONS)
    {
        return "Unknown";
    }
    return locations[location];
}

void displayLocations(void)
{
    int i;
    printf("\n========== HOSTEL LOCATIONS ==========\n");
    for (i = 0; i < NUM_LOCATIONS; i++)
    {
        printf("%d. %s\n", i + 1, locations[i]);
    }
}

/* room 1xx -> Block A, 2xx -> Block B, 3xx -> Block C, 4xx -> Block D */
int blockForRoom(int roomno)
{
    switch (roomno / 100)
    {
    case 1:
        return BLOCK_A;
    case 2:
        return BLOCK_B;
    case 3:
        return BLOCK_C;
    case 4:
        return BLOCK_D;
    default:
        return -1;
    }
}

/* Dijkstra's algorithm, O(V^2) array version:
   dist[v] = shortest distance from source to v
   prev[v] = location before v on that shortest path (-1 for none) */
void dijkstra(int source, int dist[], int prev[])
{
    int visited[NUM_LOCATIONS];
    int i;
    int v;
    int u;
    int count;

    for (i = 0; i < NUM_LOCATIONS; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;

    for (count = 0; count < NUM_LOCATIONS; count++)
    {
        /* pick the unvisited location with the smallest distance */
        u = -1;
        for (i = 0; i < NUM_LOCATIONS; i++)
        {
            if (!visited[i] && dist[i] != INF && (u == -1 || dist[i] < dist[u]))
            {
                u = i;
            }
        }
        if (u == -1)
        {
            break; /* remaining locations are unreachable */
        }
        visited[u] = 1;

        /* relax every edge leaving u */
        for (v = 0; v < NUM_LOCATIONS; v++)
        {
            if (campusMap[u][v] != 0 && !visited[v] &&
                dist[u] + campusMap[u][v] < dist[v])
            {
                dist[v] = dist[u] + campusMap[u][v];
                prev[v] = u;
            }
        }
    }
}

/* follows prev[] back from the destination, then prints it forwards */
void printPath(int prev[], int destination)
{
    int path[NUM_LOCATIONS];
    int length = 0;
    int at;
    int i;
    for (at = destination; at != -1; at = prev[at])
    {
        path[length++] = at;
    }
    for (i = length - 1; i >= 0; i--)
    {
        printf("%s", locations[path[i]]);
        if (i > 0)
        {
            printf(" -> ");
        }
    }
}

void showShortestRoute(int source, int destination)
{
    int dist[NUM_LOCATIONS];
    int prev[NUM_LOCATIONS];
    dijkstra(source, dist, prev);
    if (dist[destination] == INF)
    {
        printf("\nNo path from %s to %s.\n", locations[source], locations[destination]);
        return;
    }
    printf("\nShortest route: ");
    printPath(prev, destination);
    printf("\nTotal distance: %d m\n", dist[destination]);
}

/* route the maintenance staff from their office to the student's block */
void showRouteToRoom(int roomno)
{
    int block = blockForRoom(roomno);
    if (block == -1)
    {
        printf("\nRoom %d is not in a mapped block, no route available.\n", roomno);
        return;
    }
    printf("\nRoute for staff to room %d (%s):", roomno, locations[block]);
    showShortestRoute(MAINTENANCE_OFFICE, block);
}

void routeMenu(void)
{
    int source;
    int destination;
    displayLocations();
    printf("\nEnter starting location (1-%d): ", NUM_LOCATIONS);
    if (!readInt(&source) || source < 1 || source > NUM_LOCATIONS)
    {
        printf("\nInvalid location!\n");
        return;
    }
    printf("Enter destination (1-%d): ", NUM_LOCATIONS);
    if (!readInt(&destination) || destination < 1 || destination > NUM_LOCATIONS)
    {
        printf("\nInvalid location!\n");
        return;
    }
    showShortestRoute(source - 1, destination - 1);
}
