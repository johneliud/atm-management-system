# ATM Management System - Beginner's Guide

Welcome! This guide will help you understand the ATM Management System codebase. Since you're new to C, we'll explain everything step by step.

## Table of Contents
1. [Getting Started](#getting-started)
2. [Project Overview](#project-overview)
3. [How C Programs Work](#how-c-programs-work)
4. [File Structure](#file-structure)
5. [Understanding Data Structures](#understanding-data-structures)
6. [How the Program Flows](#how-the-program-flows)
7. [Understanding File I/O](#understanding-file-io)
8. [Common C Concepts Used](#common-c-concepts-used)
9. [Building and Running](#building-and-running)

---

## Getting Started

### Cloning the Repository

To get started with this project, you'll need to clone it from GitHub:

```bash
# Clone the repository
git clone https://github.com/johneliud/atm-management-system.git

# Navigate into the project directory
cd atm-management-system

# Check the contents
ls -la
```

**What these commands do:**
- `git clone` - Downloads the entire project from GitHub to your computer
- `cd` - Changes directory (navigates into the folder)
- `ls -la` - Lists all files including hidden ones

### Prerequisites

Before you can compile and run the project, make sure you have:

1. **C Compiler** - `gcc` or `cc`
   ```bash
   # Check if you have a compiler
   gcc --version
   # or
   cc --version
   ```

2. **Make** - Build automation tool
   ```bash
   # Check if you have make
   make --version
   ```

3. **Git** - Version control (for cloning)
   ```bash
   # Check if you have git
   git --version
   ```

**Installing on Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install build-essential git
```

**Installing on macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

---

## Project Overview

This is a bank ATM management system that lets users:
- Register and login
- Create bank accounts
- View their accounts
- Update account information
- Make deposits and withdrawals
- Transfer accounts to other users
- Delete accounts

All data is stored in simple text files (no database needed).

---

## How C Programs Work

### The Basics

C is a **compiled language**, which means:
1. You write code in `.c` and `.h` files (human-readable)
2. A **compiler** (like `gcc` or `cc`) translates it to machine code
3. The result is an **executable** file that your computer can run

```
source code (.c) → compiler → executable (./atm)
```

### The Entry Point

Every C program starts at the `main()` function:

```c
int main()
{
    // Your program starts here
    return 0;  // 0 means success
}
```

---

## File Structure

```
atm-management-system/
├── data/                    # Data storage (text files)
│   ├── users.txt           # User credentials
│   └── records.txt         # Account records
├── src/                    # Source code
│   ├── header.h           # Declarations (structures, function prototypes)
│   ├── main.c             # Entry point and menus
│   ├── auth.c             # Login/registration logic
│   └── system.c           # Account management logic
├── guide/
│   ├── question.md        # Project requirements
│   └── plan.md            # Implementation plan
├── docs/                  # Documentation (this folder!)
│   ├── README.md          # This file
│   └── justifications.md  # Detailed code explanations
└── Makefile              # Build automation
```

### Why Split Into Multiple Files?

- **Organization**: Each file has a specific purpose
- **Reusability**: Functions can be used across files
- **Maintainability**: Easier to find and fix bugs
- **Collaboration**: Multiple people can work on different files

---

## Understanding Data Structures

### What is a Structure?

A **struct** (structure) is a way to group related data together. Think of it like a form or template.

#### Example: The User Structure

```c
struct User
{
    int id;           // User's unique ID number
    char name[50];    // Username (up to 50 characters)
    char password[50]; // Password (up to 50 characters)
};
```

**In plain English**: "A User has an ID, a name, and a password."

#### Creating a User

```c
struct User u;              // Declare a user variable
u.id = 1;                   // Set the ID
strcpy(u.name, "Alice");    // Set the name (can't use = with strings!)
strcpy(u.password, "1234"); // Set the password
```

### The Record Structure

A **Record** represents a bank account:

```c
struct Record
{
    int id;                  // Record ID
    int userId;              // Which user owns this account
    char name[100];          // Owner's name
    char country[100];       // Country
    int phone;               // Phone number
    char accountType[10];    // Type: savings, current, fixed01, etc.
    int accountNbr;          // Account number
    double amount;           // Balance (money in account)
    struct Date deposit;     // When account was created
    struct Date withdraw;    // When last withdrawal was made
};
```

### The Date Structure

```c
struct Date
{
    int month;  // 1-12
    int day;    // 1-31
    int year;   // e.g., 2025
};
```

---

## How the Program Flows

### 1. Program Starts (main.c)

```c
int main()
{
    struct User u;        // Create a user variable

    initMenu(&u);        // Show login/register menu
    mainMenu(u);         // Show main menu after login
    return 0;
}
```

### 2. Initial Menu (initMenu)

User chooses:
- **Option 1**: Login → Check credentials
- **Option 2**: Register → Create new user
- **Option 3**: Exit → Quit program

### 3. Main Menu (mainMenu)

After successful login, user sees:
1. Create a new account
2. Update account information
3. Check accounts
4. Check list of owned accounts
5. Make transaction
6. Remove existing account
7. Transfer ownership
8. Exit

### 4. Action Functions

Each menu option calls a different function:
- `createNewAcc()` - Creates new bank account
- `checkAllAccounts()` - Lists all user's accounts
- `updateAccountInfo()` - Modifies account details
- etc.

---

## Understanding File I/O

### Why Use Files?

Files let us **persist data** - when you close the program and reopen it, your data is still there.

### File Operations in C

#### 1. Opening a File

```c
FILE *fp;
fp = fopen("data/users.txt", "r");  // "r" = read mode
```

**Modes:**
- `"r"` - Read (file must exist)
- `"w"` - Write (creates new file, overwrites existing)
- `"a"` - Append (adds to end of file)
- `"r+"` - Read and write
- `"a+"` - Read and append

#### 2. Reading from a File

```c
char name[50];
char password[50];

// Read formatted data (like scanf, but from file)
fscanf(fp, "%s %s", name, password);
```

#### 3. Writing to a File

```c
// Write formatted data
fprintf(fp, "%d %s %s\n", userId, name, password);
```

#### 4. Closing a File

```c
fclose(fp);  // ALWAYS close files when done!
```

### File Format

#### users.txt
```
0 Alice 1234password
1 Michel password1234
```
Format: `id name password`

#### records.txt
```
0 0 Alice 0 10/02/2020 german 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 portugal 914134431 1920.42 savings
```
Format: `id userId name accountNbr date country phone balance type`

---

## Common C Concepts Used

### 1. Pointers

A **pointer** stores the memory address of a variable.

```c
int x = 10;
int *ptr = &x;  // ptr points to x's address

printf("%d", *ptr);  // Print value at address (10)
```

**Why use pointers?**
- Pass large data to functions efficiently
- Modify variables inside functions
- Dynamic memory allocation

#### Example: Passing by Reference

```c
void modifyUser(struct User *u)  // Pointer parameter
{
    u->id = 5;  // Arrow operator for pointer to struct
}

int main()
{
    struct User user;
    modifyUser(&user);  // Pass address of user
    printf("%d", user.id);  // Prints 5
}
```

### 2. Arrays and Strings

In C, strings are arrays of characters:

```c
char name[50];  // Array of 50 characters
```

**Important:**
- Arrays are fixed size
- Strings end with `\0` (null terminator)
- Use `strcpy()` to copy strings, not `=`

```c
char name[50];
strcpy(name, "Alice");     // ✓ Correct
// name = "Alice";         // ✗ Wrong! Can't assign to array
```

### 3. String Functions

```c
#include <string.h>

strcmp(str1, str2)   // Compare strings (returns 0 if equal)
strcpy(dest, src)    // Copy string
strlen(str)          // Get string length
```

### 4. Input/Output

```c
#include <stdio.h>

printf("Hello %s\n", name);        // Print to screen
scanf("%s", name);                  // Read from keyboard
fprintf(fp, "data", ...);          // Write to file
fscanf(fp, "format", ...);         // Read from file
```

### 5. Control Structures

#### If Statements
```c
if (amount > 0)
{
    printf("Positive");
}
else if (amount < 0)
{
    printf("Negative");
}
else
{
    printf("Zero");
}
```

#### While Loop
```c
while (condition)
{
    // Repeat while condition is true
}
```

#### For Loop
```c
for (int i = 0; i < 10; i++)
{
    // Repeat 10 times
}
```

#### Switch Statement
```c
switch (option)
{
    case 1:
        doSomething();
        break;
    case 2:
        doOtherThing();
        break;
    default:
        handleInvalid();
}
```

---

## Building and Running

### Using the Makefile

A **Makefile** automates compilation:

```bash
# Compile the program
make

# Run the program
./atm

# Clean up compiled files
make clean
```

### What Make Does

```makefile
atm : src/main.o src/system.o src/auth.o
    cc -o atm src/main.o src/system.o src/auth.o
```

This means:
1. Compile `main.c` → `main.o`
2. Compile `system.c` → `system.o`
3. Compile `auth.c` → `auth.o`
4. Link them together → `atm` executable

### Manual Compilation (if needed)

```bash
# Compile each source file
gcc -c src/main.c -o src/main.o
gcc -c src/auth.c -o src/auth.o
gcc -c src/system.c -o src/system.o

# Link them together
gcc src/main.o src/auth.o src/system.o -o atm

# Run
./atm
```

---

## Key Takeaways for Beginners

1. **Start with main()**: Find `main()` to understand program flow
2. **Follow function calls**: See what each function does
3. **Structures organize data**: Like forms or templates
4. **Pointers are addresses**: Use `&` to get address, `*` to get value
5. **Files persist data**: Use `fopen`, `fscanf`, `fprintf`, `fclose`
6. **Always check errors**: File operations can fail!
7. **Read compiler errors carefully**: They tell you exactly what's wrong

---

## Next Steps

1. Read `justifications.md` for detailed code explanations
2. Review `guide/plan.md` for implementation tasks
3. Try compiling and running the existing code
4. Experiment with small changes
5. Implement one feature at a time
