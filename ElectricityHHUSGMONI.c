#include <stdio.h>
#include <stdlib.h>

// STRUCT: To store electricity usage data
struct Electricity {
    char name[50];
    int kwh;
    float cost;
};

// FUNCTION: Calculate electricity cost
float calculateCost(int kwh) {
    float cost;

    if (kwh <= 50)
        cost = kwh * 0.5;
    else if (kwh <= 100)
        cost = kwh * 0.75;
    else
        cost = kwh * 1.2;

    return cost;
}

// FUNCTION: Add new electricity record
void addRecord() {
    FILE *f = fopen("electricity.txt", "a");
    struct Electricity e;

    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s", e.name);

    printf("Enter Electricity Usage (kWh): ");
    scanf("%d", &e.kwh);

    // Calculate cost
    e.cost = calculateCost(e.kwh);

    // Save to file
    fprintf(f, "%s %d %.2f\n", e.name, e.kwh, e.cost);

    fclose(f);

    printf("✔ Record saved successfully!\n");
}

// FUNCTION: View all records
void viewRecords() {
    FILE *f = fopen("electricity.txt", "r");
    struct Electricity e;

    if (f == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n=== ELECTRICITY RECORDS ===\n");

    while (fscanf(f, "%s %d %f", e.name, &e.kwh, &e.cost) != EOF) {
        printf("Name: %s | Usage: %d kWh | Cost: %.2f\n",
               e.name, e.kwh, e.cost);
    }

    fclose(f);
}

// FUNCTION: Main menu system
void menu() {
    int choice;

    do {
        printf("\n=== ELECTRICITY MONITORING SYSTEM ===\n");
        printf("1. Add Record\n");
        printf("2. View Records\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addRecord();
                break;
            case 2:
                viewRecords();
                break;
            case 3:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 3);
}

// MAIN FUNCTION
int main() {
    menu();
    return 0;
}