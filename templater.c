#include "templater.h"

int main(int argc, char** argv)
{
    Student* students = read_csv("students.csv");

    if (students == NULL)
    {
        fprintf(stderr, "Failed to read student data");
        return 1;
    }

    for (size_t i = 0; i < sizeof(students); i++)
    {
        /*NOTE:(donke) This is weird and necessary becauase
        // we do not have a custom allocator...
        // resizing with realloc gives me a min of 8
        // on my Windows based machine
        // I guess its related to memory alignment
        // Won' be a problem if we have a significant amount
        // of entries
        */
        if (students[i].grade == '\0')
        {
            continue;
        }

        // printf("The size of the returned table is %lld\n", sizeof(students));
        // printf("Name of %lld: %s\n",i, students[i].name);
        // printf("RollNo of %lld: %s\n",i, students[i].rollno);
        // printf("Name of %lld: %d\n",i, students[i].marks);
        // printf("Name of %lld: %c\n\n\n\n",i, students[i].grade);
    }

    return 0;
}

Student* read_csv(const char* filename)
{
    //NOTE:(donke) Look into this, Windows has nothing of sorts
    uint32_t line_max = LINE_MAX;

    FILE* csvfile;
    if ((csvfile = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Could not read CSV file %s", filename);
        return NULL;
    }

    char* student_data = (char*)malloc(line_max + 1);
    if (student_data == NULL)
    {
        fprintf(stderr, 
                "Failed to allocate memory while reading CSV file %s",
                filename);
        fclose(csvfile);
        return NULL;
    }

    size_t student_size = STUDENT_SIZE;
    // Using calloc because of annoying behaviour of system allocators 
    Student* students = calloc(student_size, sizeof(Student));
    if (students == NULL)
    {
        fprintf(stderr, 
                "Failed to allocate memory for student array while parsing CSV file %s",
                filename);
        free(student_data);
        fclose(csvfile);
    }
    
    size_t count = 0;
    while (fgets(student_data, line_max + 1, csvfile) != NULL)
    {
        if(ferror(csvfile))
        {
            fprintf(stderr, 
                    "Error enountered while parsing CSV file %s",
                    filename);
            fclose(csvfile);
            return NULL;
        }

        char* row = student_data;

        char* name  = strtok(row, ",");
        char* roll  = strtok(NULL, ",");
        char* marks = strtok(NULL, ",");

        // printf("%s\n%s\n%s\n", name, roll, marks);

        strncpy(students[count].name, name, NAMESIZE - 1);
        students[count].name[NAMESIZE - 1] = '\0';

        strncpy(students[count].rollno, roll, ROLLNOSIZE - 1);
        students[count].rollno[NAMESIZE - 1] = '\0';

        students[count].marks = atoi(marks);
        students[count].grade = compute_grade(students[count]);

        count++;
    }

    Student* resized = realloc(students, count * sizeof(Student));
    if (resized == NULL)
    {
        printf("(Warning) Failed to resize internal student array\n");
        printf("(Warning) This array will probably have garbage content\n");
        free(student_data);
        fclose(csvfile);
        return students;
    }
    
    free(student_data);
    fclose(csvfile);
    return resized;
}

char compute_grade(Student student)
{
    if (student.marks >= 90)
    {
        return 'S';
    }
    else if (student.marks < 90 && student.marks >= 80)
    {
        return 'A';
    }
    else if (student.marks < 80 && student.marks >= 70)
    {
        return 'B';
    }
    else if (student.marks < 70 && student.marks >= 60)
    {
        return 'C';
    }
    else if (student.marks < 60 && student.marks >= 50)
    {
        return 'D';
    }
    else if (student.marks < 50 && student.marks >= 40)
    {
        return 'E';
    }
    else
    {
        return 'U';
    }
}

void write_file(Student student)
{

}
