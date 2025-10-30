#include "header.h"

// Validate date components
int validateDate(int month, int day, int year)
{
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check for leap year
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
        daysInMonth[1] = 29;
    
    if (day < 1 || day > daysInMonth[month - 1]) return 0;
    return 1;
}

// Get valid integer within range
int getValidInt(const char *prompt, int min, int max)
{
    int value;
    char buffer[100];
    
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (sscanf(buffer, "%d", &value) == 1)
            {
                if (value >= min && value <= max)
                    return value;
                else
                    printf("✖ Please enter a value between %d and %d\n", min, max);
            }
            else
                printf("✖ Please enter a valid number\n");
        }
    }
}

// Get valid double within range
double getValidDouble(const char *prompt, double min, double max)
{
    double value;
    char buffer[100];
    
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (sscanf(buffer, "%lf", &value) == 1)
            {
                if (value >= min && value <= max)
                    return value;
                else
                    printf("✖ Please enter a value between %.2f and %.2f\n", min, max);
            }
            else
                printf("✖ Please enter a valid number\n");
        }
    }
}

// Get valid string input
void getValidString(const char *prompt, char *str, int maxLen)
{
    char buffer[200];
    
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            // Remove newline
            buffer[strcspn(buffer, "\n")] = 0;
            
            if (strlen(buffer) > 0 && strlen(buffer) < maxLen)
            {
                strcpy(str, buffer);
                return;
            }
            else
                printf("✖ Please enter 1-%d characters\n", maxLen - 1);
        }
    }
}

// Get valid date with validation
int getValidDate(int *month, int *day, int *year)
{
    while (1)
    {
        printf("Enter date (mm/dd/yyyy): ");
        char buffer[50];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            if (sscanf(buffer, "%d/%d/%d", month, day, year) == 3)
            {
                if (validateDate(*month, *day, *year))
                    return 1;
                else
                    printf("✖ Invalid date. Please check month (1-12), day, and year (1900-2100)\n");
            }
            else
                printf("✖ Please use format mm/dd/yyyy (e.g., 12/25/2023)\n");
        }
    }
}

// Validate account number (positive, reasonable range)
int validateAccountNumber(int accountNbr)
{
    return (accountNbr > 0 && accountNbr <= 999999999);
}

// Validate phone number (positive, reasonable length)
int validatePhoneNumber(int phone)
{
    return (phone > 0 && phone <= 999999999);
}

// Validate amount (positive, reasonable range)
int validateAmount(double amount)
{
    return (amount >= 0.01 && amount <= 1000000.00);
}
