#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATA_FILE "electricity.txt"
#define NAME_LEN 50

// STRUCT: To store electricity usage data
struct Electricity {
    char name[NAME_LEN];
    int kwh;
    float cost;
};

// FUNCTION: Safe input for integer values
int readInt(const char *prompt) {
    char buffer[128];
    char *endPtr;
    long value;

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }

        value = strtol(buffer, &endPtr, 10);

        while (isspace((unsigned char)*endPtr)) {
            endPtr++;
        }

        if (endPtr == buffer || *endPtr != '\0') {
            printf("Invalid number. Please try again.\n");
            continue;
        }

        if (value < 0) {
            printf("Value cannot be negative. Please try again.\n");
            continue;
        }

        return (int)value;
    }
}

// FUNCTION: Safe input for name (supports spaces)
void readName(const char *prompt, char *name, size_t size) {
    while (1) {
        printf("%s", prompt);

        if (fgets(name, (int)size, stdin) == NULL) {
            name[0] = '\0';
            return;
        }

        name[strcspn(name, "\n")] = '\0';

        if (strlen(name) == 0) {
            printf("Name cannot be empty. Please try again.\n");
            continue;
        }

        return;
    }
}

// FUNCTION: Calculate electricity cost (slab-wise billing)
float calculateCost(int kwh) {
    float cost = 0.0f;

    if (kwh <= 50) {
        cost = kwh * 0.5f;
    } else if (kwh <= 100) {
        cost = (50 * 0.5f) + ((kwh - 50) * 0.75f);
    } else {
        cost = (50 * 0.5f) + (50 * 0.75f) + ((kwh - 100) * 1.2f);
    }

    return cost;
}

// FUNCTION: Add new electricity record
void addRecord() {
    FILE *f = fopen(DATA_FILE, "a");
    struct Electricity e;

    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    readName("Enter Name: ", e.name, sizeof(e.name));
    if (strlen(e.name) == 0) {
        printf("Input cancelled.\n");
        fclose(f);
        return;
    }

    e.kwh = readInt("Enter Electricity Usage (kWh): ");
    if (e.kwh < 0) {
        printf("Input cancelled.\n");
        fclose(f);
        return;
    }

    e.cost = calculateCost(e.kwh);

    // Save to file in pipe-delimited format to support names with spaces
    fprintf(f, "%s|%d|%.2f\n", e.name, e.kwh, e.cost);

    fclose(f);

    printf("Record saved successfully!\n");
}

// FUNCTION: Parse one record line
int parseRecord(char *line, struct Electricity *e) {
    char *nameToken = strtok(line, "|");
    char *kwhToken = strtok(NULL, "|");
    char *costToken = strtok(NULL, "|\n");

    if (nameToken == NULL || kwhToken == NULL || costToken == NULL) {
        return 0;
    }

    strncpy(e->name, nameToken, NAME_LEN - 1);
    e->name[NAME_LEN - 1] = '\0';
    e->kwh = atoi(kwhToken);
    e->cost = (float)atof(costToken);

    return 1;
}

// FUNCTION: View all records + summary
void viewRecords() {
    FILE *f = fopen(DATA_FILE, "r");
    struct Electricity e;
    char line[256];
    int count = 0;
    int totalKwh = 0;
    float totalCost = 0.0f;

    if (f == NULL) {
        printf("No records found.\n");
        return;
    }

    printf("\n=== ELECTRICITY RECORDS ===\n");

    while (fgets(line, sizeof(line), f) != NULL) {
        if (!parseRecord(line, &e)) {
            continue;
        }

        count++;
        totalKwh += e.kwh;
        totalCost += e.cost;

        printf("%d. Name: %s | Usage: %d kWh | Cost: %.2f\n",
               count, e.name, e.kwh, e.cost);
    }

    fclose(f);

    if (count == 0) {
        printf("No valid records found.\n");
        return;
    }

    printf("\n--- SUMMARY ---\n");
    printf("Total records: %d\n", count);
    printf("Total usage: %d kWh\n", totalKwh);
    printf("Total bill: %.2f\n", totalCost);
    printf("Average usage: %.2f kWh\n", (float)totalKwh / count);
    printf("Average bill: %.2f\n", totalCost / count);
}

// FUNCTION: Search records by name
void searchRecord() {
    FILE *f = fopen(DATA_FILE, "r");
    struct Electricity e;
    char line[256];
    char query[NAME_LEN];
    int found = 0;

    if (f == NULL) {
        printf("No records found.\n");
        return;
    }

    readName("Enter name to search: ", query, sizeof(query));
    if (strlen(query) == 0) {
        fclose(f);
        return;
    }

    printf("\n=== SEARCH RESULTS ===\n");

    while (fgets(line, sizeof(line), f) != NULL) {
        if (!parseRecord(line, &e)) {
            continue;
        }

        if (strstr(e.name, query) != NULL) {
            found = 1;
            printf("Name: %s | Usage: %d kWh | Cost: %.2f\n", e.name, e.kwh, e.cost);
        }
    }

    if (!found) {
        printf("No matching records found.\n");
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
        printf("3. Search Record\n");
        printf("4. Exit\n");
        choice = readInt("Choose: ");

        switch(choice) {
            case 1:
                addRecord();
                break;
            case 2:
                viewRecords();
                break;
            case 3:
                searchRecord();
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 4);
}

// MAIN FUNCTION
int main() {
    menu();
    return 0;
}
