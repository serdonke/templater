#include "templater.h"

int main(void)
{
    Student* students = read_csv("students.csv");

    if (students == NULL) 
    { 
        return 1; 
    }

    for (size_t i = 0; i < sizeof(students); i++)
    {
        /* NOTE:(donke) This is weird and necessary becauase
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
        write_file(students[i]);
    }

    // I should free the students pointer but since this is the end...
    // I do not have to, as the OS will take care of it
    // Regardless for the sake of "gOoD pRaCtiCeS"
    free(students);
    return 0;
}

Student* read_csv(const char* filename)
{
    // NOTE:(donke) Look into this, Windows has nothing of sorts
    uint32_t line_max = MY_LINE_MAX;

    FILE* csvfile;
    if ((csvfile = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Could not read CSV file %s\n", filename);
        return NULL;
    }

    char* student_data = (char*)malloc(line_max + 1);
    if (student_data == NULL)
    {
        fprintf(stderr, 
                "Failed to allocate memory while reading CSV file %s\n",
                filename);
        fclose(csvfile);
        return NULL;
    }

    size_t student_size = STUDENT_SIZE;
    // Using calloc because of annoying behaviour of system allocators
    // and to make it easy to deal with garbage stuff
    Student* students = calloc(student_size, sizeof(Student));
    if (students == NULL)
    {
        fprintf(stderr, 
                "Failed to allocate memory for student array while parsing CSV file %s\n",
                filename);
        free(student_data);
        fclose(csvfile);
        return NULL;
    }
    
    size_t count = 0;
    while (fgets(student_data, line_max + 1, csvfile) != NULL)
    {
        if(ferror(csvfile))
        {
            fprintf(stderr, 
                    "Error enountered while parsing CSV file %s\n",
                    filename);
            free(student_data);
            free(students);
            fclose(csvfile);
            return NULL;
        }

        char* row = student_data;

        char* name  = strtok(row, ",");
        char* roll  = strtok(NULL, ",");
        char* marks = strtok(NULL, ",");

        strncpy(students[count].name, name, NAMESIZE - 1);
        students[count].name[NAMESIZE - 1] = '\0';

        strncpy(students[count].rollno, roll, ROLLNOSIZE - 1);
        students[count].rollno[NAMESIZE - 1] = '\0';

        students[count].marks = atoi(marks);
        students[count].grade = compute_grade(students[count]);

        count++;
    }

    Student* resized_students = realloc(students, count * sizeof(Student));
    if (resized_students == NULL)
    {
        printf("(Warning) Failed to resize internal student array\n");
        printf("(Warning) This array will probably have garbage content\n");
        free(student_data);
        fclose(csvfile);
        // This is intentional 
        return students;
    }
    
    free(student_data);
    fclose(csvfile);
    /* NOTE:(donke) Freeing students array gives some unexpected behaviour
    // Since in testing the size was 3 and I am allocating 512
    // realloc just frees excess but the pointer remains same 
    // and freeing it gives unexpected behaviour since students
    // and resized both point to the same memory location
    */
    return resized_students;
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
    FILE* template = fopen("templates.txt", "r");
    if (template == NULL)
    {
        fprintf(stderr, "Failed to open template file\n");
        exit(1);
    }
    
    fseek(template, 0, SEEK_END);
    size_t template_size = ftell(template);
    fseek(template, 0, SEEK_SET);

    char* template_string = (char*)calloc(template_size + 1, sizeof(char));

    fread(template_string, sizeof(char), template_size, template);
    template_string[template_size] = '\0';
    
    for (size_t i = 0; i < 4; i++)
    {
        switch (i)
        {
            case 0:
                template_string = replace_substr(template_string, g_str_templates[i], student.name);
                break;
            case 1:
                template_string = replace_substr(template_string, g_str_templates[i], student.rollno);
                break;
            // NOTE:(donke) TIL C's grammar does not allow for 
            // declarations after case...
            // so I need an empty statement
            case 2: ;
                char* marks = calloc(4, sizeof(char));
                sprintf(marks, "%d", student.marks);
                template_string = replace_substr(template_string, g_str_templates[i], marks);
                free(marks);
                break;
            case 3: ;
                char grade[2];
                strncpy(grade, &student.grade, sizeof(char));
                grade[1] = '\0';
                template_string = replace_substr(template_string, g_str_templates[i], grade);
                break;
            default:
                break;
        }
    }

    char outputfilename[32];
    sprintf(outputfilename, "output/%s.txt", student.rollno);
    FILE* outfile  = fopen(outputfilename, "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "Failed to open the output file");
        fclose(template);
        exit(2);
    }

    fprintf(outfile, "%s", template_string);

    free(template_string);
    fclose(template);
    fclose(outfile);
}

char* replace_substr(char* template_string, const char* substr, const char* substr_replace)
{
    char* new_str; 
    char* found;
    while ((found = strstr(template_string, substr)) != NULL)
    {
        size_t new_str_size = 0;
        
        size_t prefix_size = found - template_string;
        size_t replacement_size = strlen(substr_replace) + 1;
        size_t suffix_size = strlen(template_string) - (prefix_size + strlen(substr));

        new_str_size += prefix_size + replacement_size + suffix_size;

        new_str = (char*)calloc(new_str_size, sizeof(char));
        if (new_str == NULL)
        {
          fprintf(stderr, "Failed to allocate memory during template expansion\n");
          exit(1);
        }

        strncat(new_str, template_string, prefix_size);
        strncat(new_str, substr_replace, strlen(substr_replace));
        strncat(new_str, &template_string[prefix_size + strlen(substr)], suffix_size);

        strncpy(template_string, new_str, new_str_size * sizeof(char));
    }
    return new_str;
}
