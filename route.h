#ifndef ROUTE_H
#define ROUTE_H

/* Hostel campus map as a weighted graph (distances in metres) */
#define NUM_LOCATIONS 9
#define MAINTENANCE_OFFICE 0

/* Graph Functions */
void addPath(int from, int to, int distance);
void initializeCampusMap(void);
const char *locationName(int location);
void displayLocations(void);
int blockForRoom(int roomno);

/* Dijkstra Shortest Path Functions */
void dijkstra(int source, int dist[], int prev[]);
void printPath(int prev[], int destination);
void showShortestRoute(int source, int destination);
void showRouteToRoom(int roomno);
void routeMenu(void);

#endif
