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
- **Inter-Account Transfer** — transfers money to another customer's account by account number, with checks for a valid receiver, sufficient balance, non-zero amount, and no self-transfers, plus a `y/n` confirmation before the transfer happens.

## How It Works

- User records are stored in `user.txt` as plain text: name, account number, balance, and PIN, one field per line.
- Manager credentials are stored in `manager.txt`, created only once (on first run).
- `updatebalance()` handles balance changes by reading `user.txt`, writing the updated records to a temporary file (`tempuser.txt`), then deleting the original and renaming the temp file — a simple pattern for "editing" a line in a flat text file.
- `deleteuser()` uses the same read-and-rewrite pattern: it copies every record except the matching account number into `tempuser.txt`, then swaps it in for `user.txt`. Returns `1` if a matching user was found and removed, `0` otherwise.
- `finduser()` looks up an account by number and fills a `struct Bank` with that user's details — used to validate the receiver before a transfer.
- `interbanktranaction()` handles money transfers between two customer accounts: it validates that both the sender and receiver exist, the amount is positive, the accounts aren't the same, and the sender has enough balance — then rewrites `user.txt` with the sender's balance decreased and the receiver's increased, using the same temp-file swap pattern.
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

**Customer session — deposit, check balance, and transfer money:**

```
-------------BANK MANAGEMENT SYSTEM--------------
ENTER YOUR ACCOUNT NUMBER: 2001
ENTER YOUR PIN: 1234

------WELCOME John Doe-----

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] INTER-ACCOUNT TRANSFER 
[5] EXIT 
SELECT YOUR CHOICE : 4
ENTER RECEIVER ACCOUNT NO : 2002
ENTER AMOUNT TO BE TRANSFER : 1000
ARE YOU SURE YOU WANT TO TRANSFER Rs1000.00 TO Jane Smith ? (y/n): y
TRANSFER SUCCESSFUL!!

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] INTER-ACCOUNT TRANSFER 
[5] EXIT 
SELECT YOUR CHOICE : 3
YOUR CURRENT BALANCE IS : 4000.00

[1] DEPOSIT MONEY 
[2] WITHDRAW MONEY 
[3] CHECK BALANCE 
[4] INTER-ACCOUNT TRANSFER 
[5] EXIT 
SELECT YOUR CHOICE : 5
EXITING.....
```

*(Sender's account started at 5000, sent Rs1000 to receiver 2002, leaving a balance of 4000.)*

*(In red/green in the real terminal — success messages show in green, errors in red, via ANSI codes. Output above was captured by compiling and running the current version of the code, including the transfer feature.)*

## Known Limitations / To-Do

- No input validation for account numbers (e.g., duplicate account numbers aren't checked when adding a user).
- Master password and PINs are stored in plain text — not secure for real-world use.
- `scanf("%[^\n]s", ...)` for password/name input can behave inconsistently with buffered newlines; mixing `scanf("%d", ...)` and string reads occasionally requires an extra `getchar()` to clear the input buffer.
- No way to change PIN/password after creation.
- Inter-account transfer rewrites `user.txt` via delete-and-rename, so an interruption mid-transfer (e.g. program crash) could leave the file in an inconsistent state — there's no rollback.

## About This Project

This is my first project. It follows a Student Management System exercise as a self-made practice project, applying the same file-I/O patterns (struct-based records, read/update/rewrite via a temp file) to a bank account context.
