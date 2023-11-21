#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>


#define STUDENT_SIZE 512;

/*NOTE:(donke) I do not know why LINE_MAX is not defined.
When I looked into header file for limits.s
I did not find it
But man pages say that it is
So I will define it as the man-pages suggest 
there is probably a better solution */
#define LINE_MAX 16384;

#define NAMESIZE   20
#define ROLLNOSIZE 20

typedef struct student
{
    char name[NAMESIZE];
    char rollno[ROLLNOSIZE];
    uint8_t marks;
    char grade;
} Student;

Student* read_csv(const char* filename);
char compute_grade(Student);
void write_file(Student);
