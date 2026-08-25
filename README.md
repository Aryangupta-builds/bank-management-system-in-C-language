# Bank Management System (C)

A simple console-based Bank Management System written in C, using file handling (`.txt` files) for persistent storage. Supports two roles — **Bank Manager** and **Normal User** — with separate menus and permissions.

## Features

### Manager
- First run auto-creates the bank: sets manager name, account number, and master password (saved to `manager.txt`).
- Logs in with account number + master password.
- **Add User** — creates a new customer with name, account number, initial balance, and PIN (appended to `user.txt`).
- **Search User** — looks up a customer by account number and displays their details.
- **Delete User** — placeholder, not yet implemented.

### Normal User
- Logs in with account number + PIN.
- **Deposit Money** — adds to balance and updates `user.txt`.
- **Withdraw Money** — subtracts from balance (blocks if insufficient funds).
- **Check Balance** — displays current balance.

## How It Works

- User records are stored in `user.txt` as plain text: name, account number, balance, and PIN, one field per line.
- Manager credentials are stored in `manager.txt`, created only once (on first run).
- `updatebalance()` handles balance changes by reading `user.txt`, writing the updated records to a temporary file (`tempuser.txt`), then deleting the original and renaming the temp file — a simple pattern for "editing" a line in a flat text file.
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

## Known Limitations / To-Do

- **Delete User** is not implemented yet.
- No input validation for account numbers (e.g., duplicate account numbers aren't checked when adding a user).
- Master password and PINs are stored in plain text — not secure for real-world use.
- `scanf("%[^\n]s", ...)` for password/name input can behave inconsistently with buffered newlines; mixing `scanf("%d", ...)` and string reads occasionally requires an extra `getchar()` to clear the input buffer.
- No way to change PIN/password after creation.

## About This Project

This project follows a Student Management System exercise as a self-made practice project, applying the same file-I/O patterns (struct-based records, read/update/rewrite via a temp file) to a bank account context.
