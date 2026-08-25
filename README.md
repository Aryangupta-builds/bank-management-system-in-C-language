# Bank Management System (C)

A simple console-based Bank Management System written in C, using file handling (`.txt` files) for persistent storage. Supports two roles — **Bank Manager** and **Normal User** — with separate menus and permissions.

## Features

### Manager
- First run auto-creates the bank: sets manager name, account number, and master password (saved to `manager.txt`).
- Logs in with account number + master password.
- **Add User** — creates a new customer with name, account number, initial balance, and PIN (appended to `user.txt`).
- **Search User** — looks up a customer by account number and displays their details.
- **Delete User** — looks up a customer by account number, shows their details, asks for `y/n` confirmation, then removes them from `user.txt`.

### Normal User
- Logs in with account number + PIN.
- **Deposit Money** — adds to balance and updates `user.txt`.
- **Withdraw Money** — subtracts from balance (blocks if insufficient funds).
- **Check Balance** — displays current balance.

## How It Works

- User records are stored in `user.txt` as plain text: name, account number, balance, and PIN, one field per line.
- Manager credentials are stored in `manager.txt`, created only once (on first run).
- `updatebalance()` handles balance changes by reading `user.txt`, writing the updated records to a temporary file (`tempuser.txt`), then deleting the original and renaming the temp file — a simple pattern for "editing" a line in a flat text file.
- `deleteuser()` uses the same read-and-rewrite pattern: it copies every record except the matching account number into `tempuser.txt`, then swaps it in for `user.txt`. Returns `1` if a matching user was found and removed, `0` otherwise.
- Colored terminal output (ANSI escape codes) is used to highlight success (green) and error (red) messages.

## Files Generated at Runtime

| File | Purpose |
|---|---|
| `manager.txt` | Stores the bank manager's name, account number, and master password |
| `user.txt` | Stores all customer records |
| `tempuser.txt` | Temporary file used during balance updates (auto-deleted) |

## Build & Run

```bash
gcc bank.c -o bank
./bank
```

On the first run, you'll be prompted to set up the bank manager. On subsequent runs, log in with either the manager's account number or a customer's account number.

## Sample Terminal Output

**Manager session — add a user and search for them:**

```
----NEW BANK-----
ENTER NAME OF BANK MANAGER: Aryan Gupta
ENTER YOUR ACCOUNT NO : 1001
SET YOUR MASTER PASSWIRD: admin123

Manager file created sucessfully
-------------BANK MANAGEMENT SYSTEM--------------
ENTER YOUR ACCOUNT NUMBER: 1001
ENTER YOUR MASTER PASSWORD: admin123

----------------------------------
WELCEOME Aryan Gupta
------------------------------------

[1] ADD USER
[2] SEARCH USER
[3] DELETE USER
[4] EXIT
SELECT YOUR USER : 1
ENTER NAME OF NEW USER: John Doe
ENTER ACCOUNT NO. OF NEW USER: 2001
ENTER THE INITIAL BALANCE OF ACCOUNT: 5000
SET PIN OF USER : 1234

User Added Sucessfully!!

[1] ADD USER
[2] SEARCH USER
[3] DELETE USER
[4] EXIT
SELECT YOUR USER : 2
ENTER ACCOUNT NO. OF USER: 2001

-------USER DETAILS-------
NAME      : John Doe
ACCOUNT No: 2001
BALANCE   : 5000.00

[1] ADD USER
[2] SEARCH USER
[3] DELETE USER
[4] EXIT
SELECT YOUR USER : 4
EXITING....
```

**Customer session — deposit and check balance:**

```
-------------BANK MANAGEMENT SYSTEM--------------
ENTER YOUR ACCOUNT NUMBER: 2001
ENTER YOUR PIN: 1234

------WELCOME John Doe-----

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] EXIT 
SELECT YOUR CHOICE : 1
ENTER THE AMOUNT YOU HAVE TO DEPOSIT : 1000
AMOUNT DEPOSIT SUCESSFULLY...
NEW BALANCE : 6000.00

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] EXIT 
SELECT YOUR CHOICE : 3
YOUR CURRENT BALANCE IS : 6000.00

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] EXIT 
SELECT YOUR CHOICE : 4
EXITING.....
```

*(In red/green in the real terminal — success messages show in green, errors in red, via ANSI codes. Output above was captured by compiling and running the code above.)*

## Known Limitations / To-Do

- No input validation for account numbers (e.g., duplicate account numbers aren't checked when adding a user).
- Master password and PINs are stored in plain text — not secure for real-world use.
- `scanf("%[^\n]s", ...)` for password/name input can behave inconsistently with buffered newlines; mixing `scanf("%d", ...)` and string reads occasionally requires an extra `getchar()` to clear the input buffer.
- No way to change PIN/password after creation.

## About This Project

This is my first project. It follows a Student Management System exercise as a self-made practice project, applying the same file-I/O patterns (struct-based records, read/update/rewrite via a temp file) to a bank account context.
