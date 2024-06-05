#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include "vehicle_sales_calculator.h" 

BSTNode* createNode(Vehicle vehicle) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    newNode->vehicle = vehicle; 
    newNode->left = NULL;  
    newNode->right = NULL; 
    return newNode;
}

void insertVehicle(BSTNode** root, Vehicle vehicle) {
    if (*root == NULL) {
        *root = createNode(vehicle); 
        return;
    }

    if (vehicle.price < (*root)->vehicle.price) {
        insertVehicle(&(*root)->left, vehicle); 
    } else {
        insertVehicle(&(*root)->right, vehicle); 
    }
}

void traverseInOrderDesc(BSTNode* root) {
    if (root != NULL) {
        traverseInOrderDesc(root->right); 
        printf("%-15s %-15s $%-13.2f\n", root->vehicle.make, root->vehicle.model, root->vehicle.price);
        traverseInOrderDesc(root->left); 
    }
}

void suggestVehicles(BSTNode* root, double budget) {
    BSTNode* current = root;
    BSTNode* topVehicles[2] = {NULL, NULL}; 

    while (current != NULL) {
        if (current->vehicle.price <= budget) {
            if (topVehicles[0] == NULL || current->vehicle.price > topVehicles[0]->vehicle.price) {
                topVehicles[1] = topVehicles[0]; 
                topVehicles[0] = current; 
            } else if (topVehicles[1] == NULL || current->vehicle.price > topVehicles[1]->vehicle.price) {
                topVehicles[1] = current; 
            }

            current = current->left;
        } else {
            current = current->right;
        }
    }

    printf("MAKE           MODEL          PRICE         \n");
    if (topVehicles[0] != NULL) {
        printf("%-15s %-15s $%-13.2f\n", topVehicles[0]->vehicle.make, topVehicles[0]->vehicle.model, topVehicles[0]->vehicle.price);
    }
    if (topVehicles[1] != NULL) {
        printf("%-15s %-15s $%-13.2f\n", topVehicles[1]->vehicle.make, topVehicles[1]->vehicle.model, topVehicles[1]->vehicle.price);
    }
}

void addModel(BSTNode** root) {
    Vehicle vehicle; 
    printf("Enter make: "); 
    scanf("%s", vehicle.make);
    printf("Enter model: "); 
    scanf("%s", vehicle.model);
    printf("Enter price: "); 
    scanf("%lf", &vehicle.price);

    insertVehicle(root, vehicle); 
}

void loadFromCSV(BSTNode** root, const char* filename) {
    FILE* file = fopen(filename, "r"); 
    if (file == NULL) {
        printf("No CSV file found.\n"); 
        return;
    }

    char line[256]; 
    while (fgets(line, sizeof(line), file)) {
        Vehicle vehicle; 
        sscanf(line, "%[^,],%[^,],%lf", vehicle.make, vehicle.model, &vehicle.price); 
        insertVehicle(root, vehicle); 
    }

    fclose(file); 
}

int main() {
    BSTNode* root = NULL; 

    loadFromCSV(&root, "vehicles.csv");

    int option; 
    double budget; 

    do {
        printf("Vehicle Sales Calculator\n");
        printf("1. Suggest Vehicle\n");
        printf("2. Add Model to Catalog\n");
        printf("3. View Catalog\n");
        printf("4. Exit\n");
        printf("> ");

        scanf("%d", &option); 

        switch (option) {
            case 1:
                printf("Enter budget: "); 
                scanf("%lf", &budget);
                suggestVehicles(root, budget); 
                break;
            case 2:
                addModel(&root); 
                break;
            case 3:
                printf("MAKE           MODEL          PRICE         \n");
                traverseInOrderDesc(root); 
                break;
            case 4:
                printf("Exiting...\n"); 
                break;
            default:
                printf("Invalid option. Please try again.\n"); 
        }

        printf("\n");
    } while (option != 4); 

    return 0;
}
