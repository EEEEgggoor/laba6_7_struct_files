#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iomanip>
#include <cmath>
#include <time.h>
#define N 20

typedef struct {
    char* name;
    char* fam;
} Fio;

typedef struct {
    int number;
    int year;
    int mounth;
} ITB;

typedef struct {
    Fio fio;
    char* kaf;
    char* predmet;
    ITB itb;
} Teacher;

Teacher mas[N];


void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacter(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void setName(Teacher* struc) {
    char buff[25];
    printf("Name: ");
    scanf("%s", buff);

    struc->fio.name = (char*)malloc(strlen(buff) + 1);
    if (struc->fio.name == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(struc->fio.name, buff);
}

void setFam(Teacher* struc) {
    char buff[25];
    printf("Familiya: ");
    scanf("%s", buff);

    struc->fio.fam = (char*)malloc(strlen(buff) + 1);
    if (struc->fio.fam == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(struc->fio.fam, buff);
}

void setKaf(Teacher* struc) {
    char buff[25];
    printf("Kafedra: ");
    scanf("%s", buff);

    struc->kaf = (char*)malloc(strlen(buff) + 1);
    if (struc->kaf == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(struc->kaf, buff);
}

void setPredmet(Teacher* struc) {
    char buff[25];
    printf("Predmet: ");
    scanf("%s", buff);

    struc->predmet = (char*)malloc(strlen(buff) + 1);
    if (struc->predmet == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    strcpy(struc->predmet, buff);
}

void setITB(Teacher* struc) {
    int n;

    printf("Number: ");
    scanf("%d", &n);

    struc->itb.number = n;

    

    printf("Mounth: ");

    int m;
    scanf("%d", &m);
    struc->itb.mounth = m;


    printf("Year: ");

    int y;
    scanf("%d", &y);
    struc->itb.year = y;
}

void free_memory(Teacher* mas, int count) {
    for (int i = 0; i < count; i++) {
        free(mas[i].fio.name);
        free(mas[i].fio.fam);
        free(mas[i].kaf);
        free(mas[i].predmet);
    }
}


void write_teacher(FILE* fp, Teacher* t) {
    int len_name = strlen(t->fio.name) + 1;
    int len_fam = strlen(t->fio.fam) + 1;
    int len_kaf = strlen(t->kaf) + 1;
    int len_predmet = strlen(t->predmet) + 1;

    fwrite(&len_name, sizeof(int), 1, fp);
    fwrite(&len_fam, sizeof(int), 1, fp);
    fwrite(&len_kaf, sizeof(int), 1, fp);
    fwrite(&len_predmet, sizeof(int), 1, fp);

    fwrite(t->fio.name, sizeof(char), len_name, fp);
    fwrite(t->fio.fam, sizeof(char), len_fam, fp);
    fwrite(t->kaf, sizeof(char), len_kaf, fp);
    fwrite(t->predmet, sizeof(char), len_predmet, fp);

    fwrite(&t->itb.number, sizeof(int), 1, fp);
    fwrite(&t->itb.mounth, sizeof(int), 1, fp);
    fwrite(&t->itb.year, sizeof(int), 1, fp);
}
int read_teacher(FILE* fp, Teacher* t) {
    int len_name, len_fam, len_kaf, len_predmet;

    if (fread(&len_name, sizeof(int), 1, fp) != 1 ||
        fread(&len_fam, sizeof(int), 1, fp) != 1 ||
        fread(&len_kaf, sizeof(int), 1, fp) != 1 ||
        fread(&len_predmet, sizeof(int), 1, fp) != 1) {
        return 0;
    }

    if (len_name <= 0 || len_fam <= 0 || len_kaf <= 0 || len_predmet <= 0) {
        return 0;
    }

    t->fio.name = (char*)malloc(len_name);
    if (t->fio.name == NULL || fread(t->fio.name, sizeof(char), len_name, fp) != len_name) {
        return 0;
    }

    t->fio.fam = (char*)malloc(len_fam);
    if (t->fio.fam == NULL || fread(t->fio.fam, sizeof(char), len_fam, fp) != len_fam) {
        free(t->fio.name); 
        return 0;
    }

    t->kaf = (char*)malloc(len_kaf);
    if (t->kaf == NULL || fread(t->kaf, sizeof(char), len_kaf, fp) != len_kaf) {
        free(t->fio.name);
        free(t->fio.fam);
        return 0;
    }

    t->predmet = (char*)malloc(len_predmet);
    if (t->predmet == NULL || fread(t->predmet, sizeof(char), len_predmet, fp) != len_predmet) {
        free(t->fio.name);
        free(t->fio.fam);
        free(t->kaf);
        return 0;
    }

    if (fread(&t->itb.number, sizeof(int), 1, fp) != 1 ||
        fread(&t->itb.mounth, sizeof(int), 1, fp) != 1 ||
        fread(&t->itb.year, sizeof(int), 1, fp) != 1) {
        free(t->fio.name);
        free(t->fio.fam);
        free(t->kaf);
        free(t->predmet);
        return 0;
    }


    return 1;
}

void vivod_teachers(Teacher* mas, int count) {
    printf("Информация о преподавателях:\n\n");

    printf("ФИО\t\tКафедра\t\tПредмет\t\tНомер ИТБ\tМесяц\tГод\n");
    printf("---------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (mas[i].fio.name != NULL && mas[i].fio.fam != NULL && mas[i].kaf != NULL && mas[i].predmet != NULL && mas[i].itb.mounth != NULL) {
            printf("%s\t\t%s\t\t%s\t\t%d\t\t%d\t%d\n%s\n\n",
                mas[i].fio.name,
                mas[i].kaf,
                mas[i].predmet,
                mas[i].itb.number,
                mas[i].itb.mounth,
                mas[i].itb.year,
                mas[i].fio.fam
            );
        }
       
    }

    printf("---------------------------------------------------------------------------------------------------\n\n");
}


void find_teacher_ITB(Teacher* mas, int count) {
    struct tm input_tm = { 0 };
    time_t now;
    struct tm* now_tm;
    printf("Преподаватели, у которых с даты последнего ИТБ прошло более 15 месяцев:\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        input_tm.tm_year = mas[i].itb.year - 1990;
        input_tm.tm_mon = mas[i].itb.mounth - 1;
        input_tm.tm_mday = mas[i].itb.number;
        time(&now);
        now_tm = localtime(&now);
        int year_diff = now_tm->tm_year + 1900 - (input_tm.tm_year + 1900);
        int month_diff = now_tm->tm_mon - input_tm.tm_mon;

        int total_month_diff = year_diff * 12 + month_diff;


        if (total_month_diff>=15) {
            printf("%s ", mas[i].fio.name);
            printf("%s\n", mas[i].fio.fam);
        }
    }
    printf("-----------------------------------------\n");
}


void add_new_teacher(int count, Teacher* mas) {

    FILE* fp;
    fopen_s(&fp, "teachers.dat", "ab");
    if (fp == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    for (int i = 0; i < count; i++) {
        Teacher new_teacher;
        setName(&new_teacher);
        setFam(&new_teacher);
        setKaf(&new_teacher);
        setPredmet(&new_teacher);
        setITB(&new_teacher);
        mas[i] = new_teacher;


        write_teacher(fp, &new_teacher);
    }
    printf("-----------------------------------------\n\n");
}


void find_predmets(Teacher* mas, const char* name, const char* fam, int count) {
    int t = 0;
    printf("Предметы которые ведет этот преподаватель:\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(mas[i].fio.name, name) == 0 && strcmp(mas[i].fio.fam, fam) == 0) {
            printf("%s\n", mas[i].predmet);
            t++;

        }
    }
    if (t == 0) printf("Преподаватель не найден.");
    printf("-----------------------------------------\n");
}


void find_and_sort_by_kaf(Teacher* mas, const char* kaf_name, int count) {
    Teacher* maskaf = (Teacher*)malloc(N * sizeof(Teacher));
    int filtered_count = 0;

    for (int i = 0; i < count-1; i++) {
        if (strcmp(mas[i].kaf, kaf_name) == 0) {
            maskaf[filtered_count++] = mas[i];
        }
    }

    for (int i = 0; i < filtered_count - 1; i++) {
        for (int j = 0; j < filtered_count - i - 1; j++) {
            if (strcmp(maskaf[j].fio.name, maskaf[j + 1].fio.name) > 0 ||
                (strcmp(maskaf[j].fio.name, maskaf[j + 1].fio.name) == 0 &&
                    strcmp(maskaf[j].fio.fam, maskaf[j + 1].fio.fam) > 0)) {
                Teacher temp = maskaf[j];
                maskaf[j] = maskaf[j + 1];
                maskaf[j + 1] = temp;
            }
        }
    }

    printf("Преподаватели кафедры %s:\n", kaf_name);
    printf("-----------------------------------------\n");

    for (int i = 0; i < filtered_count; i++) {
        printf("%s %s\n", maskaf[i].fio.name, maskaf[i].fio.fam);
    }

    printf("-----------------------------------------\n");

    free(maskaf);
}

int init(Teacher* mas) {
    FILE* fp;
    fopen_s(&fp, "teachers.dat", "rb"); 
    int count = 0;

    if (fp != NULL) {
        while (!feof(fp) && count < N) {
            Teacher t;
            if (!read_teacher(fp, &t)) break;
            mas[count] = t; 
            count++;
        }
        fclose(fp);
    }
    int R;

    printf("\n");
    printf("\tБаза данных <Преподаватель>\n");

    printf("Добавить нового преподавателя - 1\n");
    printf("Распечатать информацию о преподавателях - 2\n");
    printf("Найти все предметы, которые ведет заданный преподаватель - 3\n");
    printf("Найти всех преподавателей заданной кафедры - 4\n");
    printf("Найти всех преподавателей, у которых с даты последнего ИТБ прошло более 15 месяцев - 5\n");
    printf("Выйти из программы - 6\n");

    printf("Выбрать функцию - ");

    scanf("%d", &R);

    getchar();

    return R;
}


int main() {

    setlocale(LC_ALL, "Rus");

    int count = 0, q = 0;
    int res;

    do {
        clear();

        res = init(mas);
        if (q < 1) {
            for (int i = 0; mas[i].fio.name != NULL && i < N; i++) {
                count++;
            }
            q = 2;
        }
        

        switch (res) {
        case 1: {
            if (count < N) {
                add_new_teacher(1, &mas[count]);
                count++;
            }
            else {
                printf("Достигнуто максимальное количество преподавателей.\n");
            }
            break;
        }
        case 2: { 
            vivod_teachers(mas, count);
            printf("Введите любую цифру для продолжения:");
            int p;
            scanf("%d", &p);
            break;
        }
        case 3: {
            char input[50];
            char name[25], fam[25];
            printf("Введите имя и фамилию преподавателя (через пробел): ");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%s %s", name, fam);

            find_predmets(mas, name, fam, count);
            printf("Введите любую цифру для продолжения:");
            int p;
            scanf("%d", &p);
            break;
        }
        case 4: {
            char input[25];

            printf("Введите кафедру преподавателя: ");
            fgets(input, sizeof(input), stdin);

            input[strcspn(input, "\n")] = '\0';

            find_and_sort_by_kaf(mas, input, count);
            printf("Введите любую цифру для продолжения:");
            int p;
            scanf("%d", &p);
            break;
        }

        case 5: {
            find_teacher_ITB(mas, count);
            printf("Введите любую цифру для продолжения:");
            int p;
            scanf("%d", &p);
            break;
        }

        case 6: { 
            printf("Выход из программы\n");
            free_memory(mas, count);
            break;
        }

        default: {
            printf("ОШИБКА. Неверный выбор.\n");
            break;
        }
        }

    } while (res != 6);

    return 0;
}