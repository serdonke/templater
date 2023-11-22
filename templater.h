#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Minimum size of an array of Students before resizing
#define STUDENT_SIZE 512

/*NOTE:(donke) I do not know why LINE_MAX is not defined.
When I looked into header file for limits.s I saw it not!! 
but I am working on a Windows based machine and the header 
was from MS asw... I did not find it. But man pages say 
that it is So I will define it as the man-pages suggest 
there is probably a better solution. There is nothing 
really special with that number... The spec says that 
we only need 20 bytes for each string in struct and since 
marks won't exceed 100 or go below 0 it will be uint8_t 
*/
#define MY_LINE_MAX 16384

#define NAMESIZE   20
#define ROLLNOSIZE 20

typedef struct student
{
    char name[NAMESIZE];
    char rollno[ROLLNOSIZE];
    uint8_t marks;
    char grade;
} Student;

const char* g_str_templates[] = {"{name}", "{rollno}", "{marks}", "{grade}"};

Student* read_csv(const char* filename);
char compute_grade(Student);
void write_file(Student);
// Replace the substring 'replace' with 'replace_wth' in place fore template_string
char* replace_substr(char* template_string, const char* substr, const char* substr_replace);
