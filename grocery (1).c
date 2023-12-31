//Yousef Khalil  Grocery Store Database 11/12/2020

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct item {
    int id;
    char name[20];
    double cost;
    char category;
    struct item* next;
};

struct item* head = NULL;
int edited = 0;

void addNewItem();
void deleteItem();
void changeCost();
void searchItem();
void displayInventory();
void saveDatabase(char* filename);

int main() {
    char filename[30];
    printf("Welcome to the grocery shop\nPlease input a file name to be uploaded into the store: ");
    scanf("%s", filename);

    FILE* filePTR = fopen(filename, "r");
    if (filePTR == NULL) {
        printf("File not found!");
    } else {
        int id;
        char name[20];
        double cost;
        char category;

        struct item* temp = head;

        while (fscanf(filePTR, "%d %s %lf %c", &id, name, &cost, &category) != EOF) {
            if (head == NULL) {
                head = (struct item*)malloc(sizeof(struct item));
                head->id = id;
                head->category = category;
                head->cost = cost;
                head->next = NULL;
                strcpy(head->name, name);
                temp = head;
            } else {
                while (temp->next != NULL) {
                    temp = temp->next;
                }

                struct item* new_item = (struct item*)malloc(sizeof(struct item));
                new_item->id = id;
                new_item->category = category;
                new_item->cost = cost;
                new_item->next = NULL;
                strcpy(new_item->name, name);

                temp->next = new_item;
                temp = new_item;
            }
        }
    }
    fclose(filePTR);

    int optionNumber = 0;
    printf("\nPlease make one of the following selections:\n1. Add new item\n2. Delete Item\n3. Change the cost of an item\n4. Search for an item\n5. Display inventory details\n6. Quit\n");
    scanf("%*c"); // Consume the newline character from the previous input
    do {
        printf("Enter choice: ");
        scanf("%d", &optionNumber);

        switch (optionNumber) {
            case 1:
                addNewItem();
                break;
            case 2:
                deleteItem();
                break;
            case 3:
                changeCost();
                break;
            case 4:
                searchItem();
                break;
            case 5:
                displayInventory();
                break;
            case 6: {
                char input[10], command[10];
                printf("\nQuitting..\n");
                if (edited == 1) {
                    printf("Would you like to save the changes made to the file? (yes/no): ");
                    fflush(stdin);
                    fgets(input, sizeof(input), stdin);
                    sscanf(input, "%s", command);
                    if (strncmp(command, "yes", 3) == 0) {
                        char newFileName[30];
                        printf("What file name would you like to save the new database? ");
                        scanf("%s", newFileName);
                        printf("The file has been saved. Thanks for shopping!\n");
                        remove(filename);
                        saveDatabase(newFileName);
                        return 0;
                    }
                } else if (strncmp(command, "no", 2) == 0) {
                    printf("Quit successful!\n");
                    return 0;
                }
                break;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        printf("\n-----------------\n");
    } while (optionNumber != 6);

    return 0;
}
void addNewItem() {
    int id;
    char name[20];
    double cost;
    char category, white;

    printf("\nPlease enter the item's id: ");
    scanf("%d", &id);

    printf("\nPlease enter the item's name: ");
    scanf("%19s", name);

    printf("\nPlease enter the item's price: ");
    scanf("%lf", &cost);

    scanf("%c", &white);

    printf("\nPlease enter the item's category: ");
    scanf(" %c%*c", &category);

    if (head == NULL) {
        head = (struct item*)malloc(sizeof(struct item));
        head->id = id;
        head->category = category;
        head->cost = cost;
        head->next = NULL;
        strcpy(head->name, name);
    } else {
        struct item* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }

        struct item* new_item = (struct item*)malloc(sizeof(struct item));
        new_item->id = id;
        new_item->category = category;
        new_item->cost = cost;
        new_item->next = NULL;
        strcpy(new_item->name, name);

        temp->next = new_item;
    }

    edited = 1;
}
void deleteItem() {
    int itemId;
    printf("Enter the ID of the item to delete: ");
    scanf("%d", &itemId);

    struct item* current = head;
    struct item* previous = NULL;

    while (current != NULL) {
        if (current->id == itemId) {
            if (previous == NULL) {
                // Deleting the first item in the list
                head = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            edited = 1;
            printf("Item with ID %d has been deleted.\n", itemId);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Item with ID %d not found in the inventory.\n", itemId);
}

void changeCost() {
    int itemId;
    double newCost;
    printf("Enter the ID of the item to change the cost: ");
    scanf("%d", &itemId);

    struct item* current = head;

    while (current != NULL) {
        if (current->id == itemId) {
            printf("Enter the new cost for item ID %d: ", itemId);
            scanf("%lf", &newCost);
            current->cost = newCost;
            edited = 1;
            printf("Cost for item ID %d has been changed.\n", itemId);
            return;
        }

        current = current->next;
    }

    printf("Item with ID %d not found in the inventory.\n", itemId);
}

void searchItem() {
    char itemName[20];
    printf("Enter the name of the item to search: ");
    scanf("%s", itemName);

    struct item* current = head;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->name, itemName) == 0) {
            printf("Item found:\n");
            printf("ID: %d\n", current->id);
            printf("Name: %s\n", current->name);
            printf("Cost: %.2lf\n", current->cost);
            printf("Category: %c\n", current->category);
            found = 1;
        }

        current = current->next;
    }

    if (!found) {
        printf("Item '%s' not found in the inventory.\n", itemName);
    }
}

void displayInventory() {
    struct item* temp = head;
    printf("ID Name Cost Category\n");
    while (temp != NULL) {
        printf("%d %s %.2lf %c\n", temp->id, temp->name, temp->cost, temp->category);
        temp = temp->next;
    }
}

void saveDatabase(char* filename) {
    FILE* filePTR = fopen(filename, "w");
    if (filePTR == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct item* current = head;

    while (current != NULL) {
        fprintf(filePTR, "%d %s %.2lf %c\n", current->id, current->name, current->cost, current->category);
        current = current->next;
    }

    fclose(filePTR);
    printf("Inventory database has been saved to %s.\n", filename);
}