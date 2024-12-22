
/* Sadeen Khatib
 * 1212164
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int element;
    char firstName[20];
    char lastName[20];
    char Address[80];
    char Date[10];
    int ClassId;
    struct Node* left;
    struct Node* right;
};

typedef struct Node* TNode;

struct StudentInfo {
    int id;
    char firstName[20];
    char lastName[20];
};

TNode MakeEmpty(TNode T);
TNode ReadFromFile(TNode T, const char* filename);
TNode Insert(int id, char firstName[20], char lastName[20], char address[80], int classId, char date[10], TNode T);
TNode Delete(int id, TNode T);
TNode FindMin(TNode T);
void CollectNames(TNode T, struct StudentInfo names[], int* index);
int CompareNames(const void* a, const void* b);
void ListStudents(TNode T);
TNode FindAndUpdate(int id, TNode T);
void ListCity(TNode T, char city[80]);
void ListStudentsByClass(TNode T);
void ListIdClass(TNode T, int classId);
void SaveInFile(TNode T, const char* filename);
void SaveStudentsInfoInFile(TNode T, FILE* StudentINFO);


int main() {
    // Create a BST
    TNode BST = NULL;

    const char* filename = "students.data";
    // Read students information from the file
    BST = ReadFromFile(BST, filename);

    int s; // Option
    do {
        printf("\n- MENU -\n");
        printf("1. Insert a Student\n");
        printf("2. Find a Student and update the student info\n");
        printf("3. List All Students\n");
        printf("4. List Students from a specific city\n");
        printf("5. List Students by Class\n");
        printf("6. List Students in a specific class\n");
        printf("7. Delete a Student\n");
        printf("8. Save students to a file\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &s);

        int ID, classId;
        char firstName[20], lastName[20], address[80], date[10];
        switch (s) {
            case 1: {
                // Insert a student with associated data
                printf("Enter Student's First and Last Name: ");
                scanf("%s %s", firstName, lastName);
                printf("Enter Student's Address of City: ");
                scanf("%s", address);
                printf("Enter Student's Class ID he/she is currently in: ");
                scanf("%d", &classId);
                printf("Enter Date of enrollment: ");
                scanf("%s", date);
                printf("Enter student ID: ");
                scanf("%d", &ID);
                BST = Insert(ID, firstName, lastName, address, classId, date, BST);
                break;
            }
            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &ID);
                BST = FindAndUpdate(ID, BST);
                break;
            case 3:
                ListStudents(BST);
                break;
            case 4:
                printf("Enter a city: ");
                scanf("%s", address);
                printf("List of Students from %s: \n", address);
                ListCity(BST, address);
                break;
            case 5:
                ListStudentsByClass(BST);
                break;
            case 6:
                printf("Enter Class ID: ");
                scanf("%d", &classId);
                printf("List of Students in Class %d: \n", classId);
                ListIdClass(BST, classId);
                break;
            case 7:
                printf("Enter Student's ID: ");
                scanf("%d", &ID);
                BST = Delete(ID, BST);
                break;
            case 8:
                // Save student information to the file
                SaveInFile(BST, filename);
                printf("---Student information saved to file: %s---\n", filename);
                break;
            case 9:
                // Exit
                printf("Exiting the program...\n");
                break;
                return -1;
            default:
                printf("Invalid option! You Can try again.\n");
                break;
        }
    } while (s != 9);

    return 0;
}

TNode MakeEmpty(TNode T) {
    if (T != NULL) {
        MakeEmpty(T->left);
        MakeEmpty(T->right);
        free(T);
    }
    return NULL;
}

TNode ReadFromFile(TNode T, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return T;
    }

    int id, classId;
    char firstName[20], lastName[20], address[80], date[10];
    while (fscanf(file, "%d %s %s %s %d %s", &id, firstName, lastName, address, &classId, date) == 6) {
        T = Insert(id, firstName, lastName, address, classId, date, T);
    }

    fclose(file);
    return T;
}

TNode Insert(int id, char firstName[20], char lastName[20], char address[80], int classId, char date[10], TNode T) {
    if (T == NULL) {
        T = (TNode)malloc(sizeof(struct Node));
        T->element = id;
        strcpy(T->firstName, firstName);
        strcpy(T->lastName, lastName);
        strcpy(T->Address, address);
        strcpy(T->Date, date);
        T->ClassId = classId;
        T->left = T->right = NULL;
    }
    else if (id < T->element) {
        T->left = Insert(id, firstName, lastName, address, classId, date, T->left);
    }
    else if (id > T->element) {
        T->right = Insert(id, firstName, lastName, address, classId, date, T->right);
    }
    else {
        printf("Duplicate ID: %d\n", id);
        return T;// Return T without inserting a new node
    }
    return T;
}

TNode Delete(int id, TNode T) {
    TNode temp; // Minimum id to be root replace a deleted root
    if (T == NULL) {
        printf("The student already deleted or doesn't exist!\n");
    }
    else if (id < T->element) {  //Go Left
        T->left = Delete(id, T->left);
    }
    else if (id > T->element) { //Go Right
        T->right = Delete(id, T->right);
    }
        /* Two Children*/
    else if (T->left && T->right) {
        temp = FindMin(T->right); // Minimum id to be root replace a deleted root
        T->element = temp->element;
        strcpy(T->firstName, temp->firstName);
        strcpy(T->lastName, temp->lastName);
        strcpy(T->Address, temp->Address);
        strcpy(T->Date, temp->Date);
        T->ClassId = temp->ClassId;
        T->right = Delete(T->element, T->right);
    }
        /* One or zero children */
    else {
        temp = T;
        if (T->left == NULL) {
            T = T->right;
        } else if (T->right == NULL) {
            T = T->left;
        }
        free(temp);
    }
    return T;
}

TNode FindMin(TNode T) {
    if (T == NULL) {
        return NULL;
    }
    else if (T->left == NULL) {
        return T;
    }
    else {
        return FindMin(T->left);
    }
}

//Find a student by	his/her student	ID,	and	support	updating of	the	student info if found
TNode FindAndUpdate(int id, TNode T) {
    if (T == NULL) {
        printf("Student Not Found!\n");
        return T; // Return the unchanged tree
    }

    if (id < T->element) {
        T->left = FindAndUpdate(id, T->left);
    }
    else if (id > T->element) {
        T->right = FindAndUpdate(id, T->right);
    }
    else {
        printf("\nID: %d\t Name: %s %s\t Address: %s\t Class ID: %d\t Date: %s\n",
               T->element, T->firstName,T->lastName, T->Address, T->ClassId, T->Date);
        printf("Update Student Info:\n");
        int update;
        printf("1- Delete Student.\n");
        printf("2- Update Class ID.\n");
        printf("3- Update Address city.\n");
        printf("4- Update Name.\n");
        printf("5- Update Date.\n");
        printf("6- No update.\n");
        scanf("%d", &update);

        if (update == 1) {
            T = Delete(id,T);
            printf("---The Student deleted---\n");
        }
        else if (update == 2) {
            int newClass;
            printf("Enter the new ID of the class he/she is currently in:\n");
            scanf("%d", &newClass);
            T->ClassId = newClass;
            printf("---The Id Updated---\n");

        }
        else if (update == 3) {
            char city[80];
            printf("Enter a new city for the student's address:\n");
            scanf(" %s", city);
            strcpy(T->Address, city);
            printf("---The City Updated---\n");
        }
        else if (update == 4) {
            char firstname[20] , lastname[20];
            printf("Enter a new name for the student:\n");
            scanf(" %s %s",firstname, lastname );
            strcpy(T->firstName, firstname);
            strcpy(T->lastName,lastname);
            printf("---The Name Updated---\n");

        }
        else if (update == 5) {
            char date[10];
            printf("Enter a new date for the student:\n");
            scanf(" %s", date);
            strcpy(T->Date, date);
            printf("---The Date Updated---\n");
        }
        else if(update == 6) {
            return T;
        }
    }
    return T;
}


void CollectNames(TNode T, struct StudentInfo names[], int* index) {
    if (T != NULL) {
        CollectNames(T->left, names, index); //the Left subtree is traversed first
        /*Copy the student Info (T->element) to names[*index] */
        names[*index].id = T->element; //It copies the student ID (T->element) to names[*index].id
        strcpy(names[*index].firstName, T->firstName);
        strcpy(names[*index].lastName, T->lastName);
        (*index)++;
        CollectNames(T->right, names, index);
    }
}

// Comparison function for sorting names based on first name and last name
int CompareNames(const void* a, const void* b) {
    struct StudentInfo* studentA = (struct StudentInfo*)a;
    struct StudentInfo* studentB = (struct StudentInfo*)b;

    // Compare first names
    int firstNameComparison = strcmp(studentA->firstName, studentB->firstName);

    if (firstNameComparison != 0) {
        return firstNameComparison;
    }

    // If first names are the same, compare Last names
    return strcmp(studentA->lastName, studentB->lastName);
}

/* List	all	students in	lexicographic order	of their names.*/
void ListStudents(TNode T) {
    //Declares an array of type 'Struct StudentInfo' with a size 200 to store Student Information
    struct StudentInfo names[200];
    int index = 0; //initializes an index
    CollectNames(T, names, &index);

    if (index == 0) {
        printf("No students found.\n");
    }
    else {
        // Sort the names array in lexicographic order
        //using this modified comparison function in the qsort function
        // quick sort algorithm -> O(n log n)
        qsort(names, index, sizeof(struct StudentInfo), CompareNames);

        printf("List of Students:\n");
        printf("ID\t   Name\t\n");
        for (int i = 0; i < index; i++) {
            printf("%d\t   %s %s\n", names[i].id, names[i].firstName, names[i].lastName);
        }
    }
}

// Search for a city and list all students from that city in lexicographic order of their names
void NamesInSameCity(TNode T, struct StudentInfo names[], int* index, char city[80]) {
    if (T != NULL) {
        NamesInSameCity(T->left, names, index, city);
        if (strcmp(T->Address, city) == 0) {
            names[*index].id = T->element;
            strcpy(names[*index].firstName, T->firstName);
            strcpy(names[*index].lastName, T->lastName);
            (*index)++;
        }
        NamesInSameCity(T->right, names, index, city);
    }
}

void ListCity(TNode T, char city[80]) {
    struct StudentInfo names[100];
    int index = 0;
    NamesInSameCity(T, names, &index, city);

    if (index == 0) {
        printf("No students found from %s\n", city);
    } else {
        // Sort students in lexicographic order of their names
        qsort(names, index, sizeof(struct StudentInfo), CompareNames);
        for (int i = 0; i < index; i++) {
            printf("%d\t   %s %s\n", names[i].id, names[i].firstName, names[i].lastName);
        }
    }
}
void StudentsInSameClass(TNode T, struct StudentInfo names[], int* index, int classId) {
    if (T != NULL) {
        StudentsInSameClass(T->left, names, index, classId);
        if (T->ClassId == classId) {
            names[*index].id = T->element;
            strcpy(names[*index].firstName, T->firstName);
            strcpy(names[*index].lastName, T->lastName);
            (*index)++;
        }
        StudentsInSameClass(T->right, names, index, classId);
    }
}

//List all students	by their class in lexicographic	order of their names.
void ListStudentsByClass(TNode T) {
    // all possible class IDs
    int MAX_CLASS_ID;
    for (int classId = 1; classId <= MAX_CLASS_ID; classId++) {
        // Collect student information for the current class ID
        struct StudentInfo names[200];
        int index = 0;
        StudentsInSameClass(T, names, &index, classId);

        if (index > 0) {
            // Sort the names array in lexicographic order
            qsort(names, index, sizeof(struct StudentInfo), CompareNames);

            printf("List of Students in Class %d:\n", classId);
            printf("ID\t   Name\t\n");
            for (int i = 0; i < index; i++) {
                printf("%d\t   %s %s\n", names[i].id, names[i].firstName, names[i].lastName);
            }
            printf("\n");
        }
    }
}


void ListIdClass(TNode T, int classId) {
    struct StudentInfo names[100];
    int index = 0;
    StudentsInSameClass(T, names, &index, classId);

    if (index == 0) {
        printf("No students found in %s\n", classId);
    } else {
        // Sort students in lexicographic order of their names
        qsort(names, index, sizeof(struct StudentInfo), CompareNames);
        for (int i = 0; i < index; i++) {
            printf("%d\t   %s %s\n", names[i].id, names[i].firstName, names[i].lastName);
        }
    }
}

void SaveInFile(TNode T, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }
    SaveStudentsInfoInFile(T, file);
    fclose(file);
}

void SaveStudentsInfoInFile(TNode T, FILE* StudentINFO) {
    if (T != NULL) {
        SaveStudentsInfoInFile(T->left, StudentINFO);
        fprintf(StudentINFO, "%d\t       %s %s\t                     %s\t              %d\t          %s\n", T->element, T->firstName, T->lastName, T->Address, T->ClassId, T->Date);
        SaveStudentsInfoInFile(T->right, StudentINFO);
    }
}

