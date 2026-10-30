#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define SUBJECTS 5
#define FILE_NAME "students.dat"

struct Student {
    int rollNo;
    char name[50];
    int marks[SUBJECTS];
    int total;
    float percentage;
    char grade;
    int pass;
};

struct Student students[MAX];
int count = 0;

void calculateResult(struct Student *s) {
    s->total = 0;
    s->pass = 1;

    for (int i = 0; i < SUBJECTS; i++) {
        s->total += s->marks[i];
        if (s->marks[i] < 35)
            s->pass = 0;
    }
    s->percentage = s->total / 5.0f;
    if (!s->pass)
        s->grade = 'F';
    else if (s->percentage >= 90)
        s->grade = 'A';
    else if (s->percentage >= 75)
        s->grade = 'B';
    else if (s->percentage >= 60)
        s->grade = 'C';
    else if (s->percentage >= 50)
        s->grade = 'D';
    else
        s->grade = 'E';
}
void saveData() {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(students, sizeof(struct Student), count, fp);
    fclose(fp);
}
void loadData() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL)
        return;
    fread(&count, sizeof(int), 1, fp);
    if (count < 0 || count > MAX) {
        count = 0;
        fclose(fp);
        return;
    }
    fread(students, sizeof(struct Student), count, fp);
    fclose(fp);
}
int findStudent(int rollNo) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo)
            return i;
    }
    return -1;
}
void addStudent() {
    if (count >= MAX) {
        printf("Student limit reached!\n");
        return;
    }
    struct Student s;
    printf("Enter Roll Number: ");
    if (scanf("%d", &s.rollNo) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    if (findStudent(s.rollNo) != -1) {
        printf("Roll number already exists!\n");
        return;
    }

    printf("Enter Student Name: ");
    scanf(" %49[^\n]", s.name);

    for (int i = 0; i < SUBJECTS; i++) {
        printf("Enter marks for Subject %d (0-100): ", i + 1);

        if (scanf("%d", &s.marks[i]) != 1 ||
            s.marks[i] < 0 || s.marks[i] > 100) {
            printf("Invalid marks! Please enter 0 to 100.\n");
            while (getchar() != '\n');
            return;
        }
    }

    calculateResult(&s);
    students[count++] = s;
    saveData();

    printf("Student added successfully!\n");
}

void displayOne(struct Student s) {
    printf("\nRoll Number : %d\n", s.rollNo);
    printf("Name        : %s\n", s.name);
    printf("Total Marks : %d / 500\n", s.total);
    printf("Percentage  : %.2f%%\n", s.percentage);
    printf("Grade       : %c\n", s.grade);
    printf("Result      : %s\n", s.pass ? "PASS" : "FAIL");
}

void displayStudents() {
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }

    printf("\n--- Student Records ---\n");

    for (int i = 0; i < count; i++) {
        displayOne(students[i]);
        printf("-----------------------\n");
    }
}

void searchStudent() {
    int rollNo;

    printf("Enter Roll Number to search: ");
    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findStudent(rollNo);

    if (index == -1)
        printf("Student not found!\n");
    else
        displayOne(students[index]);
}

void updateStudent() {
    int rollNo;

    printf("Enter Roll Number to update: ");
    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findStudent(rollNo);

    if (index == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("Enter new Student Name: ");
    scanf(" %49[^\n]", students[index].name);

    for (int i = 0; i < SUBJECTS; i++) {
        printf("Enter new marks for Subject %d (0-100): ", i + 1);

        if (scanf("%d", &students[index].marks[i]) != 1 ||
            students[index].marks[i] < 0 ||
            students[index].marks[i] > 100) {
            printf("Invalid marks! Update cancelled.\n");
            while (getchar() != '\n');
            return;
        }
    }

    calculateResult(&students[index]);
    saveData();

    printf("Student record updated successfully!\n");
}

void deleteStudent() {
    int rollNo;

    printf("Enter Roll Number to delete: ");
    if (scanf("%d", &rollNo) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }

    int index = findStudent(rollNo);

    if (index == -1) {
        printf("Student not found!\n");
        return;
    }

    for (int i = index; i < count - 1; i++)
        students[i] = students[i + 1];

    count--;
    saveData();

    printf("Student deleted successfully!\n");
}

void classStatistics() {
    if (count == 0) {
        printf("No records available!\n");
        return;
    }

    int totalMarks = 0;
    int passCount = 0;
    int topperIndex = 0;

    for (int i = 0; i < count; i++) {
        totalMarks += students[i].total;

        if (students[i].pass)
            passCount++;

        if (students[i].total > students[topperIndex].total)
            topperIndex = i;
    }

    printf("\n--- Class Statistics ---\n");
    printf("Total Students  : %d\n", count);
    printf("Class Average   : %.2f%%\n",
           (float)totalMarks / (count * 5));
    printf("Passed Students : %d\n", passCount);
    printf("Failed Students : %d\n", count - passCount);
    printf("Pass Percentage: %.2f%%\n",
           (float)passCount * 100 / count);

    printf("Topper          : %s\n",
           students[topperIndex].name);
    printf("Topper Marks    : %d / 500\n",
           students[topperIndex].total);
}

int main() {
    int choice;

    loadData();

    do {
        printf("\n===== STUDENT RESULT MANAGEMENT =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Class Statistics\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                classStatistics();
                break;
            case 7:
                saveData();
                printf("Program closed successfully!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}
