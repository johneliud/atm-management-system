#include "header.h"

const char *RECORDS = "./data/records.txt";

// Get next available record ID by reading the file and finding max ID
int getNextRecordId()
{
    FILE *fp;
    struct Record r;
    char name[50];
    int maxId = -1;

    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        // File doesn't exist yet, start with ID 0
        return 0;
    }

    while (fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF)
    {
        if (r.id > maxId)
        {
            maxId = r.id;
        }
    }

    fclose(fp);
    return maxId + 1;  // Return next available ID
}

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        exit(1);
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    // Reset file pointer to beginning for duplicate check
    rewind(pf);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }

    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    // Set record ID and user ID
    r.id = getNextRecordId();
    r.userId = u.id;
    strcpy(r.name, u.name);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        printf("No accounts found or file does not exist.\n");
        success(u);
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u)
{
    int accountNbr, choice, found = 0;
    char userName[100];
    struct Record records[1000];
    int recordCount = 0;
    
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }

    system("clear");
    printf("\t\t====== Update Account Information ======\n\n");
    printf("Enter the account number you want to update: ");
    scanf("%d", &accountNbr);

    // Read all records into memory
    while (getAccountFromFile(pf, userName, &records[recordCount]))
    {
        strcpy(records[recordCount].name, userName);
        recordCount++;
    }
    fclose(pf);

    // Find and update the specific account
    for (int i = 0; i < recordCount; i++)
    {
        if (records[i].accountNbr == accountNbr && records[i].userId == u.id)
        {
            found = 1;
            printf("\nAccount found! Current details:\n");
            printf("Country: %s\n", records[i].country);
            printf("Phone: %d\n", records[i].phone);
            
            printf("\nWhat would you like to update?\n");
            printf("1. Country\n");
            printf("2. Phone number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter new country: ");
                scanf("%s", records[i].country);
            }
            else if (choice == 2)
            {
                printf("Enter new phone number: ");
                scanf("%d", &records[i].phone);
            }
            else
            {
                printf("Invalid choice!\n");
                stayOrReturn(0, updateAccountInfo, u);
                return;
            }
            break;
        }
    }

    if (!found)
    {
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }

    // Write all records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        printf("Error! Could not open records file for writing\n");
        exit(1);
    }

    for (int i = 0; i < recordCount; i++)
    {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                records[i].id,
                records[i].userId,
                records[i].name,
                records[i].accountNbr,
                records[i].deposit.month,
                records[i].deposit.day,
                records[i].deposit.year,
                records[i].country,
                records[i].phone,
                records[i].amount,
                records[i].accountType);
    }
    fclose(pf);
    success(u);
}

void checkAccountDetails(struct User u)
{
    int accountNbr, found = 0;
    char userName[100];
    struct Record r;
    double monthlyInterest;

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        stayOrReturn(0, checkAccountDetails, u);
        return;
    }

    system("clear");
    printf("\t\t====== Account Details ======\n\n");
    printf("Enter the account number you want to check: ");
    scanf("%d", &accountNbr);

    while (getAccountFromFile(pf, userName, &r))
    {
        if (r.accountNbr == accountNbr && r.userId == u.id)
        {
            found = 1;
            printf("\n_____________________\n");
            printf("\nAccount Details:\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Account Holder: %s\n", r.name);
            printf("Country: %s\n", r.country);
            printf("Phone Number: %d\n", r.phone);
            printf("Account Type: %s\n", r.accountType);
            printf("Balance: $%.2f\n", r.amount);
            printf("Account Created: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);

            // Calculate and display interest information
            if (strcmp(r.accountType, "current") == 0)
            {
                printf("\nYou will not get interests because the account is of type current\n");
            }
            else if (strcmp(r.accountType, "savings") == 0 || strcmp(r.accountType, "saving") == 0)
            {
                monthlyInterest = (r.amount * 0.07) / 12;
                printf("\nYou will get $%.2f as interest on day %d of every month\n", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                monthlyInterest = (r.amount * 0.04) / 12;
                printf("\nYou will get $%.2f as interest on day %d of every month\n", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                monthlyInterest = (r.amount * 0.05) / 12;
                printf("\nYou will get $%.2f as interest on day %d of every month\n", 
                       monthlyInterest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                monthlyInterest = (r.amount * 0.08) / 12;
                printf("\nYou will get $%.2f as interest on day %d of every month\n", 
                       monthlyInterest, r.deposit.day);
            }
            break;
        }
    }

    fclose(pf);
    
    if (!found)
    {
        stayOrReturn(0, checkAccountDetails, u);
    }
    else
    {
        success(u);
    }
}

void makeTransaction(struct User u)
{
    int accountNbr, choice, found = 0;
    double amount;
    char userName[100];
    struct Record records[1000];
    int recordCount = 0;
    
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    system("clear");
    printf("\t\t====== Make Transaction ======\n\n");
    printf("Enter the account number: ");
    scanf("%d", &accountNbr);

    // Read all records into memory
    while (getAccountFromFile(pf, userName, &records[recordCount]))
    {
        strcpy(records[recordCount].name, userName);
        recordCount++;
    }
    fclose(pf);

    // Find the account
    for (int i = 0; i < recordCount; i++)
    {
        if (records[i].accountNbr == accountNbr && records[i].userId == u.id)
        {
            found = 1;
            
            // Check if account type allows transactions
            if (strcmp(records[i].accountType, "fixed01") == 0 || 
                strcmp(records[i].accountType, "fixed02") == 0 || 
                strcmp(records[i].accountType, "fixed03") == 0)
            {
                printf("\n✖ Error: Transactions are not allowed on fixed-term accounts\n");
                stayOrReturn(0, makeTransaction, u);
                return;
            }

            printf("\nCurrent balance: $%.2f\n", records[i].amount);
            printf("\nSelect transaction type:\n");
            printf("1. Deposit\n");
            printf("2. Withdraw\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter amount to deposit: $");
                scanf("%lf", &amount);
                if (amount > 0)
                {
                    records[i].amount += amount;
                    printf("\n✔ Deposit successful! New balance: $%.2f\n", records[i].amount);
                }
                else
                {
                    printf("\n✖ Invalid amount\n");
                    stayOrReturn(0, makeTransaction, u);
                    return;
                }
            }
            else if (choice == 2)
            {
                printf("Enter amount to withdraw: $");
                scanf("%lf", &amount);
                if (amount > 0 && amount <= records[i].amount)
                {
                    records[i].amount -= amount;
                    printf("\n✔ Withdrawal successful! New balance: $%.2f\n", records[i].amount);
                }
                else if (amount > records[i].amount)
                {
                    printf("\n✖ Insufficient balance\n");
                    stayOrReturn(0, makeTransaction, u);
                    return;
                }
                else
                {
                    printf("\n✖ Invalid amount\n");
                    stayOrReturn(0, makeTransaction, u);
                    return;
                }
            }
            else
            {
                printf("\n✖ Invalid choice\n");
                stayOrReturn(0, makeTransaction, u);
                return;
            }
            break;
        }
    }

    if (!found)
    {
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    // Write all records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        printf("Error! Could not open records file for writing\n");
        exit(1);
    }

    for (int i = 0; i < recordCount; i++)
    {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                records[i].id,
                records[i].userId,
                records[i].name,
                records[i].accountNbr,
                records[i].deposit.month,
                records[i].deposit.day,
                records[i].deposit.year,
                records[i].country,
                records[i].phone,
                records[i].amount,
                records[i].accountType);
    }
    fclose(pf);
    success(u);
}
void removeAccount(struct User u)
{
    int accountNbr, found = 0;
    char userName[100];
    struct Record records[1000];
    int recordCount = 0;
    
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("Error! Could not open records file\n");
        stayOrReturn(0, removeAccount, u);
        return;
    }

    system("clear");
    printf("\t\t====== Remove Account ======\n\n");
    printf("Enter the account number to remove: ");
    scanf("%d", &accountNbr);

    // Read all records into memory
    while (getAccountFromFile(pf, userName, &records[recordCount]))
    {
        strcpy(records[recordCount].name, userName);
        recordCount++;
    }
    fclose(pf);

    // Find and verify account ownership
    for (int i = 0; i < recordCount; i++)
    {
        if (records[i].accountNbr == accountNbr && records[i].userId == u.id)
        {
            found = 1;
            printf("\nAccount found: %s - Balance: $%.2f\n", records[i].accountType, records[i].amount);
            printf("Are you sure you want to remove this account? (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            
            if (confirm == 'y' || confirm == 'Y')
            {
                // Remove account by shifting remaining records
                for (int j = i; j < recordCount - 1; j++)
                {
                    records[j] = records[j + 1];
                }
                recordCount--;
                
                // Write remaining records back to file
                pf = fopen(RECORDS, "w");
                if (pf == NULL)
                {
                    printf("Error! Could not open records file for writing\n");
                    exit(1);
                }

                for (int k = 0; k < recordCount; k++)
                {
                    fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                            records[k].id,
                            records[k].userId,
                            records[k].name,
                            records[k].accountNbr,
                            records[k].deposit.month,
                            records[k].deposit.day,
                            records[k].deposit.year,
                            records[k].country,
                            records[k].phone,
                            records[k].amount,
                            records[k].accountType);
                }
                fclose(pf);
                
                printf("\n✔ Account removed successfully!\n");
                success(u);
                return;
            }
            else
            {
                printf("\nAccount removal cancelled.\n");
                success(u);
                return;
            }
        }
    }

    if (!found)
    {
        stayOrReturn(0, removeAccount, u);
    }
}
