#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n = 0; 
int m = 0; 

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];

int last_request[MAX_PROCESSES][MAX_RESOURCES];  
int request_count[MAX_PROCESSES];                 
int resource_pressure[MAX_RESOURCES];             

int cycle_count = 0;

void readInput() {
    printf("Enter number of processes (<= %d): ", MAX_PROCESSES);
    scanf("%d", &n);
    if (n < 1) { printf("Invalid process count. Exiting.\n"); exit(1); }

    printf("Enter number of resource types (<= %d): ", MAX_RESOURCES);
    scanf("%d", &m);
    if (m < 1) { printf("Invalid resource count. Exiting.\n"); exit(1); }

    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("\nEnter Maximum Requirement Matrix (%d x %d):\n", n, m);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("\nEnter Available Resources (1 x %d):\n", m);
    for(int j=0;j<m;j++){
        scanf("%d", &available[j]);
    }

    memset(last_request, 0, sizeof(last_request));
    memset(request_count, 0, sizeof(request_count));
    memset(resource_pressure, 0, sizeof(resource_pressure));
}
