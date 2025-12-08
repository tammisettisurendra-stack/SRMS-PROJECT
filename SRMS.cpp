#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE "students.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[20];
char currentRole[20];

int loginsystem();
void mainmenu();
void admin_menu();
void user_menu();
void add_student();
void display_students();
void search_student();
void update_student();
void delete_student();

void ensure_credential_file() {
    FILE *fp = fopen(CREDENTIAL_FILE, "r");

    if (!fp) {
        // Create default credentials
        fp = fopen(CREDENTIAL_FILE, "w");
        fprintf(fp, "admin admin123 admin\n");
        fprintf(fp, "user user123 user\n");
        fclose(fp);

        printf("\n[INFO] credentials.txt was missing.\n");
        printf("[INFO] A new file has been created with default logins.\n\n");
    } else {
        fclose(fp);
    }
}

int main() {
    ensure_credential_file();

    if (loginsystem()) {
        mainmenu();
    } else {
        printf("Login failed! Exiting...\n");
    }
    return 0;
}

int loginsystem() {
    char username[20], password[20];
    char fileUser[20], filepass[20], filerole[20];

    printf("============== Login Screen ============\n");
    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("FATAL ERROR: Cannot open credentials file.\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filepass, filerole) != EOF) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filepass) == 0) {
            strcpy(currentUser, fileUser);
            strcpy(currentRole, filerole);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void mainmenu() {
    if (strcmp(currentRole, "admin") == 0)
        admin_menu();
    else
        user_menu();
}

void admin_menu() {
    int choice;

    do {
        printf("\n=== Admin Menu ===\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_student(); break;
            case 2: display_students(); break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

void user_menu() {
    int choice;

    do {
        printf("\n=== User Menu ===\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: display_students(); break;
            case 2: search_student(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

void add_student() {
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    printf("Enter roll number: ");
    scanf("%d", &st.roll);

    printf("Enter name: ");
    scanf(" %[^\n]", st.name);

    printf("Enter marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully.\n");
}

void display_students() {
    struct student st;
    char line[200];

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    printf("\n--- Student Records ---\n");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%f", &st.roll, st.name, &st.marks);
        printf("Roll: %d | Name: %s | Marks: %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

void search_student() {
    int roll;
    struct student st;
    char line[200];

    printf("Enter roll number to search: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening student file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            printf("Record Found!\n");
            printf("Roll: %d | Name: %s | Marks: %.2f\n", st.roll, st.name, st.marks);
            fclose(fp);
            return;
        }
    }

    printf("No such student found.\n");
    fclose(fp);
}

void update_student() {
    int roll, found = 0;
    struct student st;
    char line[200];

    printf("Enter roll number to update: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            found = 1;

            printf("Enter new name: ");
            scanf(" %[^\n]", st.name);

            printf("Enter new marks: ");
            scanf("%f", &st.marks);
        }

        fprintf(temp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record updated.\n");
    else printf("Student not found.\n");
}

void delete_student() {
    int roll, found = 0;
    struct student st;
    char line[200];

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%49[^|]|%f", &st.roll, st.name, &st.marks);

        if (st.roll == roll) {
            found = 1;
            continue;
        }

        fprintf(temp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record deleted successfully.\n");
    else printf("Student not found.\n");
}
