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
void calculateNeed() {
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            need[i][j] = maximum[i][j] - allocation[i][j];
            if (need[i][j] < 0) need[i][j] = 0; 
        }
    }
}

void printMatrices() {
    printf("\n--- System State (Cycle %d) ---\n", cycle_count);
    printf("\nAllocation matrix:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) printf("%3d ", allocation[i][j]);
        printf("\n");
    }
    printf("\nMaximum matrix:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) printf("%3d ", maximum[i][j]);
        printf("\n");
    }
    printf("\nNeed matrix:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) printf("%3d ", need[i][j]);
        printf("\n");
    }
    printf("\nAvailable vector:\n");
    for(int j=0;j<m;j++) printf("%3d ", available[j]);
    printf("\n");
}


int isSafeState(int safeSeq[]) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES];
    for(int j=0;j<m;j++) work[j] = available[j];
    for(int i=0;i<n;i++) finish[i] = 0;

    int count = 0;
    while (count < n) {
        int found = 0;
        for(int p=0;p<n;p++){
            if (!finish[p]) {
                int canFinish = 1;
                for(int j=0;j<m;j++){
                    if (need[p][j] > work[j]) { canFinish = 0; break; }
                }
                if (canFinish) {
                    for(int j=0;j<m;j++) work[j] += allocation[p][j];
                    safeSeq[count++] = p;
                    finish[p] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break;
    }
    return (count == n);
}
void printSafeSequence(int safeSeq[], int length) {
    printf("[DETECTION] Safe sequence: ");
    for (int i = 0; i < length; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");
}
int detectDeadlock() {
    int safeSeq[MAX_PROCESSES];
    int safe = isSafeState(safeSeq);

    if (safe) {
        printSafeSequence(safeSeq, n);
        return 0;  
    } else {
        printf("[DETECTION] No safe sequence found -> System is in DEADLOCK.\n");
        return 1;  
    }
}



double aiPredictDeadlockProb() {
    long totalNeed = 0;
    long totalAvailable = 0;
    int activeProcs = 0;
    long totalPressure = 0;

    for(int i=0;i<n;i++){
        int procNeed = 0;
        for(int j=0;j<m;j++) procNeed += need[i][j];
        if (procNeed > 0) activeProcs++;
        totalNeed += procNeed;
    }
    for(int j=0;j<m;j++) {
        totalAvailable += available[j];
        totalPressure += resource_pressure[j];
    }

    double demand_ratio = (double)totalNeed / (double)(totalAvailable + 1); 
    if (demand_ratio > 5.0) demand_ratio = 5.0; 
    double pressure_ratio = (double)totalPressure / (double)(n + 1); 
    if (pressure_ratio > 5.0) pressure_ratio = 5.0;

    double concurrency = (double)activeProcs / (double)n; 

    double prob = 0.6 * (demand_ratio / 5.0) + 0.25 * (pressure_ratio / 5.0) + 0.15 * concurrency;
    if (prob < 0) prob = 0;
    if (prob > 1) prob = 1.0;
    return prob;
}

