#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

char atoc(const char *const b)
{
    if (!b)
    {
        return -1;
    }

    char res = 0;
    if ('0' <= b[0] && b[0] <= '9')
    {
        res = b[0] - '0';
    }
    if ('0' <= b[1] && b[1] <= '9')
    {
        res *= 10;
        res += b[1] - '0';
    }
    if ('0' <= b[2] && b[2] <= '9')
    {
        res *= 10;
        res += b[2] - '0';
    }
    return res;
}

/**
** \returns 1 if the current computer is a laptop or a notebook, 0 otherwise
*/
char is_laptop()
{
    FILE *is_laptop_file = fopen("/sys/class/dmi/id/chassis_type", "r");
    if (!is_laptop_file)
    {
        return 0;
    }

    char buff[3] = { 0 };
    fread(buff, sizeof(char), 2, is_laptop_file);
    fclose(is_laptop_file);
    return buff[0] == '9' || (buff[0] == '1' && buff[1] == '0');
}

/**
** \returns Whether the battery is discharging or not
*/
char is_discharging()
{
    FILE *status_file = fopen("/sys/class/power_supply/BAT0/status", "r");
    if (!status_file)
    {
        return 0;
    }

    char status_str[12] = { 0 };
    fread(status_str, sizeof(char), 11, status_file);
    fclose(status_file);
    return status_str[0] == 'D' && status_str[1] == 'i' && status_str[2] == 's'
        && status_str[3] == 'c' && status_str[4] == 'h' && status_str[5] == 'a'
        && status_str[6] == 'r' && status_str[7] == 'g' && status_str[8] == 'i'
        && status_str[9] == 'n' && status_str[1] == 'g';
}

char get_battery_percentage()
{
    FILE *capacity_file = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (!capacity_file)
    {
        return -1;
    }

    char b[4] = { 0 };
    fread(b, sizeof(char), 3, capacity_file);
    fclose(capacity_file);
    return atoc(b);
}

void notify(unsigned char percentage)
{
    char b[79] = { 0 };
    snprintf(
        b, 78,
        "notify-send -a \"LowBatteryNotify\" -u CRITICAL "
        "-t 5000 -p \"Battery Low (%u%%)\"",
        percentage
    );
    system(b);
}

int main(void)
{
    if (!is_laptop())
    {
        return 1;
    }

    char has_notified_5  = 0;
    char has_notified_10 = 0;
    char has_notified_20 = 0;
    while (1)
    {
        if (!is_discharging())
        {
            has_notified_5 = has_notified_10 = has_notified_20 = 0;
        }
        else
        {
            char percentage = get_battery_percentage();
            if (percentage != -1)
            {
                if (percentage <= 5 && !has_notified_5)
                {
                    ++has_notified_5;
                    notify(percentage);
                }
                else if (percentage <= 10 && !has_notified_10)
                {
                    ++has_notified_10;
                    notify(percentage);
                }
                else if (percentage <= 20 && !has_notified_20)
                {
                    ++has_notified_20;
                    notify(percentage);
                }
            }
        }
        thrd_sleep(&(struct timespec){ .tv_sec = 60 }, NULL); // sleep 60 sec
    }
    return 0;
}
