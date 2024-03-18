//万年历初始代码
#include <stdio.h>
#include <SDL2/SDL.h>


//


void main_1();



//
#undef main
char *days[42];
char *day[40] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","","","","","","","","",""};


//判断闰年函数
int isLeapYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        return 1;
    } else
    {
        return 0;
    }
}

//计算每月第一天是星期几
int calculateDayOfWeek(int year, int month)
{
    if (month < 3)
    {
        month += 12;
        year--;
    }

    int q = 1;
    int m = month;
    int K = year % 100;
    int J = year / 100;

    int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 - 2 * J) % 7;

    // 调整h的值，使得0表示星期日，1表示星期一，以此类推
    if (h == 0)
    {
        h = 6;
    }
    else
    {
        h--;
    }

    if (h < 0)
    {
        h += 7;
    }

    return h;
}


//在days数组中输入日期数据
void printCalendar(int year, int month, int h)
{
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October",
                      "November", "December"};

    if (isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    // 代入第一天是星期几
    int dayOfWeek = h;
    for (int i = 1,j=0; i < daysInMonth[month - 1] + 1; i++,j++)
    {
        if (i == 1)
        {
            for (; j < dayOfWeek; j++)
            {
                days[j] = " ";
            }
        }
        days[j] = day[i-1];
//        if ((i + dayOfWeek) % 7 == 0)
//        {
//            printf("\n");
//        }
    }
    for (int i = daysInMonth[month - 1]  + h; i < 42; i++) {
        days[i] = " ";
    }
}




int year, month, h;
int main()
{


    printf("Enter year: ");
    scanf("%d", &year);

    if (year < 0)
    {
        printf("Invalid year\n");
        return 1;
    }

    printf("Enter month (1-12): ");
    scanf("%d", &month);

    if (month < 1 || month > 12)
    {
        printf("Invalid month\n");
        return 1;
    }
    h = calculateDayOfWeek(year, month);
    printCalendar(year, month, h);
    main_1();

    return 0;
}