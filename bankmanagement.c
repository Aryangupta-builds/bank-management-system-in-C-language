#include <stdio.h>
#include <string.h>
struct Bank
{
    // normal user
    char name[50];
    int Account_no;
    float balance;
    int pin;
};
struct Manager
{
    // manager
    char bankmanager_name[50];
    char masterpassword[100];
    int manageraccount;
};

void savereceipt(struct Bank *sender, struct Bank *receiver, float amount)
{
    FILE *fp = fopen("transactions.txt", "a");
    if (fp == NULL)
    {
        printf("\033[31m");
        printf("ERROR OPENING FILE\n");
        printf("\033[0m");
        return;
    }
    // printing recept in the file
    fprintf(fp, "==============================\n");
    fprintf(fp, "      TRANSACTION RECEIPT      \n");
    fprintf(fp, "==============================\n");
    fprintf(fp, "Sender Account   : %d\n", sender->Account_no);
    fprintf(fp, "Receiver Account : %d\n", receiver->Account_no);
    fprintf(fp, "Receiver Name    : %s\n", receiver->name);
    fprintf(fp, "Amount           : Rs %.2f\n", amount);
    fprintf(fp, "Status           : SUCCESSFUL\n");
    fprintf(fp, "==============================\n");
    // printing it in the console
    printf("==============================\n");
    printf("      TRANSACTION RECEIPT      \n");
    printf("==============================\n");
    printf("Sender Account   : %d\n", sender->Account_no);
    printf("Receiver Account : %d\n", receiver->Account_no);
    printf("Receiver Name    : %s\n", receiver->name);
    printf("Amount           : Rs %.2f\n", amount);
    printf("\033[32m");
    printf("Status           : SUCCESSFUL\n");
    printf("\033[0m");
    printf("==============================\n");
}

int deleteuser(int account_no)
{
    int delfound = 0;
    FILE *fp = fopen("user.txt", "r");
    FILE *temp = fopen("tempuser.txt", "w");
    struct Bank tempuser;
    if (fp == NULL || temp == NULL)
    {
        printf("\033[31m");
        printf("ERROR OPENING FILE\n");
        printf("\033[0m");
        return 0;
    }
    while (fscanf(fp, " %49[^\n]", tempuser.name) == 1)
    {
        fscanf(fp, "%d", &tempuser.Account_no);
        fscanf(fp, "%f", &tempuser.balance);
        fscanf(fp, "%d", &tempuser.pin);
        if (account_no == tempuser.Account_no)
        {
            // skiping the user found
            delfound = 1;
        }
        else
        {
            fprintf(temp, "%s\n%d\n%f\n%d\n",
                    tempuser.name,
                    tempuser.Account_no,
                    tempuser.balance,
                    tempuser.pin);
        }
    }
    fclose(fp);
    fclose(temp);
    if (delfound == 1)
    {
        remove("user.txt");
        rename("tempuser.txt", "user.txt");
    }
    else
    {
        remove("tempuser.txt");
    }
    return delfound;
}

int interbanktranaction(int senderaccount, int reciveraccount, float amount_transfer)
{
    int success = 0;
    int foundsender = 0;
    int failed = 0;
    int foundreciver = 0;
    FILE *fp = fopen("user.txt", "r");
    // FILE *user = fopen("user.txt", "r");
    // FILE *reciver = fopen("user.txt", "r");
    FILE *temp = fopen("tempuser.txt", "w");
    // FILE *temptransfer = fopen("tempuser.txt", "w");
    // FILE *tempreciver = fopen("tempuser.txt", "w");
    struct Bank tempuser;

    if (fp == NULL || temp == NULL)
    {
        printf("\033[31m");
        printf("ERROR OPENING FILE\n");
        printf("\033[0m");
        if (fp != NULL)
        {
            fclose(fp);
        }
        if (temp != NULL)
        {
            fclose(temp);
        }
        return 0;
    }
    // checking valid sender and reciver
    while (fscanf(fp, " %49[^\n]", tempuser.name) == 1)
    {
        fscanf(fp, "%d", &tempuser.Account_no);
        fscanf(fp, "%f", &tempuser.balance);
        fscanf(fp, "%d", &tempuser.pin);
        if (senderaccount == tempuser.Account_no)
        {
            foundsender = 1;
        }
        if (reciveraccount == tempuser.Account_no)
        {
            foundreciver = 1;
        }
    }
    // SENDER NOT FOUND
    if (!foundsender)
    {
        printf("\033[31m");
        printf("SENDER NOT FOUND!!\n");
        printf("\033[0m");
        failed = 1;
        fclose(fp);
        fclose(temp);
        remove("tempuser.txt");
        return 0;
    }
    // RECIVER NOT FOUND
    if (!foundreciver)
    {
        printf("\033[31m");
        printf("RECEIVER NOT FOUND!!\n");
        printf("\033[0m");
        failed = 1;
        fclose(fp);
        fclose(temp);
        remove("tempuser.txt");
        return 0;
    }
    // validate amount
    if (amount_transfer <= 0)
    {
        printf("\033[31m");
        printf("INVALID TRANSFER AMOUNT!!\n");
        printf("\033[0m");
        failed = 1;
        fclose(fp);
        fclose(temp);
        remove("tempuser.txt");
        return 0;
    }

    // cannot transfer to own account
    if (senderaccount == reciveraccount)
    {
        printf("\033[31m");
        printf("YOU CANNOT TRANSFER MONEY TO YOUR OWN ACCOUNT!!\n");
        printf("\033[0m");
        failed = 1;
        fclose(fp);
        fclose(temp);
        remove("tempuser.txt");
        return 0;
    }
    rewind(fp); // iss se phir se fp file ke starting pe aajayega
    if (foundreciver == 1 && foundsender == 1)
    {
        while (fscanf(fp, " %49[^\n]", tempuser.name) == 1)
        {
            fscanf(fp, "%d", &tempuser.Account_no);
            fscanf(fp, "%f", &tempuser.balance);
            fscanf(fp, "%d", &tempuser.pin);
            if (senderaccount == tempuser.Account_no)
            {
                if (tempuser.balance < amount_transfer)
                {
                    printf("\033[31m");
                    printf("INSUFFICENT BALANCE!!\n");
                    printf("\033[0m");
                    failed = 1;
                    fclose(fp);
                    fclose(temp);
                    remove("tempuser.txt");
                    break;
                }
                else if (tempuser.balance >= amount_transfer)
                {
                    tempuser.balance -= amount_transfer;
                }
            }
            else if (reciveraccount == tempuser.Account_no)
            {
                tempuser.balance += amount_transfer;
            }
            fprintf(temp, "%s\n%d\n%f\n%d\n", tempuser.name, tempuser.Account_no, tempuser.balance, tempuser.pin);
        }
    }

    if (failed == 0)
    {
        fclose(fp);
        fclose(temp);

        if (remove("user.txt") != 0)
        {
            printf("ERROR DELETING OLD FILE!!\n");
            remove("tempuser.txt");
            return 0;
        }

        if (rename("tempuser.txt", "user.txt") != 0)
        {
            printf("ERROR RENAMING TEMP FILE!!\n");
            return 0;
        }

        success = 1;
    }
    return success;
}

int finduser(int account, struct Bank *user)
{
    FILE *fp = fopen("user.txt", "r");

    if (fp == NULL)
    {
        return 0;
    }
    while (fscanf(fp, " %49[^\n]", user->name) == 1)
    {
        fscanf(fp, "%d", &user->Account_no);
        fscanf(fp, "%f", &user->balance);
        fscanf(fp, "%d", &user->pin);
        if (account == user->Account_no)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void updatebalance(int account_no, float new_balance)
{
    // iss function mai basically yek new file mai old file ka data update karke daal rahae hai
    // aur old file ko delete kar rahe hai and  new file ko rename kar rahe hai
    FILE *fp = fopen("user.txt", "r");
    FILE *temp = fopen("tempuser.txt", "w");
    struct Bank tempuser;
    if (fp == NULL || temp == NULL)
    {
        printf("\033[31m");
        printf("ERROR OPENING FILE\n");
        printf("\033[0m");
        return; // function void hai isliye no return value
    }
    while (fscanf(fp, " %49[^\n]", tempuser.name) == 1)
    {
        fscanf(fp, "%d", &tempuser.Account_no);
        fscanf(fp, "%f", &tempuser.balance);
        fscanf(fp, "%d", &tempuser.pin);
        if (account_no == tempuser.Account_no)
        {
            tempuser.balance = new_balance;
        }
        fprintf(temp, "%s\n%d\n%f\n%d\n", tempuser.name, tempuser.Account_no, tempuser.balance, tempuser.pin);
    }
    fclose(fp);
    fclose(temp);
    remove("user.txt");
    rename("tempuser.txt", "user.txt");
}

int main()
{
    struct Bank b;
    struct Manager m;
    int option;
    FILE *fp = fopen("manager.txt", "r");
    /*###########################MANAGER SETUP#######################*/
    if (fp == NULL)
    {
        fp = fopen("manager.txt", "w");
        if (fp == NULL)
        {
            printf("\033[31m");
            printf("ERROR IN CREATING MANAGER.TXT\n");
            printf("\033[0m");
            return 0;
        }
        // adding new manager
        printf("\n----NEW BANK-----\n");

        printf("ENTER NAME OF BANK MANAGER: ");
        scanf(" %49[^\n]", m.bankmanager_name);

        printf("ENTER YOUR ACCOUNT NO : ");
        scanf("%d", &m.manageraccount);
        getchar();

        printf("SET YOUR MASTER PASSWIRD: ");
        scanf("%[^\n]s", m.masterpassword);

        fprintf(fp, "%s\n%d\n%s\n", m.bankmanager_name, m.manageraccount, m.masterpassword);
        fclose(fp);
        printf("\033[32m");
        printf("\nManager file created sucessfully\n");
        printf("\033[0m");
    }
    else
    {
        // reading manager info
        fscanf(fp, " %49[^\n]s", m.bankmanager_name);
        fscanf(fp, "%d", &m.manageraccount);
        fscanf(fp, "%99s", m.masterpassword);
        fclose(fp);
    }

    // main login page

    printf("-------------BANK MANAGEMENT SYSTEM--------------\n");
    printf("ENTER YOUR ACCOUNT NUMBER: ");
    int check;
    int check_pass;
    int choice_normal;
    int choice_manager;
    char checkmanager[100];
    scanf("%d", &check);

    // user login
    if (check != m.manageraccount)
    {

        fp = fopen("user.txt", "r");
        if (fp == NULL)
        {
            printf("\033[31m");
            printf("NO user found\n");
            printf("\033[0m");
            return 0;
        }
        int found = 0;
        while (fscanf(fp, " %49[^\n]", b.name) == 1)
        {
            fscanf(fp, "%d", &b.Account_no);
            fscanf(fp, "%f", &b.balance);
            fscanf(fp, "%d", &b.pin);
            if (check == b.Account_no)
            {
                found = 1;
                break;
            }
        }
        fclose(fp);
        if (found == 0)
        {
            printf("\033[31m");
            printf("NO account found\n");
            printf("\033[0m");
        }
        else
        {
            printf("ENTER YOUR PIN: ");
            scanf("%d", &check_pass);
            if (check_pass == b.pin)
            {
                printf("\n------WELCOME %s-----\n", b.name);
                do
                {
                    printf("\n[1] DEPOSIT MONEY \n");
                    printf("[2] WITHDRAW MONEY \n");
                    printf("[3] CHECK BALANCE \n");
                    printf("[4] INTER-ACCOUNT TRANSFER \n");
                    printf("[5] EXIT \n");
                    printf("SELECT YOUR CHOICE : ");
                    scanf("%d", &choice_normal);
                    switch (choice_normal)
                    {
                    case 1:
                    {
                        // add money
                        printf("ENTER THE AMOUNT YOU HAVE TO DEPOSIT : ");
                        float amount;
                        scanf("%f", &amount);

                        if (amount <= 0)
                        {
                            printf("\033[31m");
                            printf("WRONG AMMOUNT ENTERED!!\n");
                            printf("\033[0m");
                            break;
                        }

                        else
                        {
                            // fscanf(fp, "%f", &b.balance);--> login ke time pe already ready kar chuke hai

                            b.balance += amount;
                            updatebalance(b.Account_no, b.balance);
                            printf("\033[32m");
                            printf("AMOUNT DEPOSIT SUCESSFULLY...\n");
                            printf("\033[0m");
                            printf("NEW BALANCE : %.2f\n", b.balance);

                            break;
                        }
                    }
                    case 2:
                    {
                        // withdraw money
                        printf("ENTER THE AMOUNT YOU HAVE TO WITHDRAW : ");
                        int withdraw;
                        scanf("%d", &withdraw);

                        if (withdraw <= 0)
                        {
                            printf("\033[31m");
                            printf("WRONG AMOUNT ENTERED!\n");
                            printf("\033[0m");
                        }
                        else if (b.balance < withdraw)
                        {
                            printf("\033[31m");
                            printf("INSUFFICIENT BALANCE!!\n");
                            printf("\033[0m");
                            break;
                        }
                        else
                        {
                            b.balance -= withdraw;
                            updatebalance(b.Account_no, b.balance);
                            printf("\033[32m");
                            printf("AMOUNT WITHDRAWED SUCESSFULLY...\n");
                            printf("\033[0m");
                            break;
                        }
                    }
                    case 3:
                    {
                        // check balance
                        fp = fopen("user.txt", "r");
                        float bal = 0.0f;

                        while (fscanf(fp, " %49[^\n]", b.name) == 1)
                        {
                            fscanf(fp, "%d", &b.Account_no);
                            fscanf(fp, "%f", &b.balance);
                            fscanf(fp, "%d", &b.pin);
                            if (check == b.Account_no)
                            {
                                bal = b.balance;
                                break;
                            }
                        }

                        fclose(fp);
                        printf("YOUR CURRENT BALANCE IS : %.2f\n", bal);
                        break;
                    }
                    case 4:
                    {
                        int receiver;
                        float amount;
                        struct Bank reciveuser;
                        printf("ENTER RECEIVER ACCOUNT NO : ");
                        scanf("%d", &receiver);
                        if (!finduser(receiver, &reciveuser))
                        {
                            printf("\033[31m");
                            printf("RECEIVER ACCOUNT NOT FOUND!!\n");
                            printf("\033[0m");
                            break;
                        }
                        if (receiver == check)
                        {
                            printf("\033[31m");
                            printf("CANNOT TRANSFER TO OWN ACCOUNT!!\n");
                            printf("\033[0m");
                            break;
                        }
                        printf("ENTER AMOUNT TO BE TRANSFER : ");
                        scanf("%f", &amount);
                        if (amount <= 0)
                        {
                            printf("\033[31m");
                            printf("INVALID TRANSFER AMOUNT!!\n");
                            printf("\033[0m");
                            break;
                        }

                        printf("ARE YOU SURE YOU WANT TO TRANSFER Rs%.2f TO %s ? (y/n): ", amount, reciveuser.name);
                        char confirm1;
                        getchar();
                        scanf("%c", &confirm1);
                        if (confirm1 == 'Y' || confirm1 == 'y')
                        {
                            if (interbanktranaction(check, receiver, amount) == 1)
                            {

                                printf("\033[32m");
                                printf("TRANSFER SUCCESSFUL!!\n");
                                printf("\033[0m");
                                savereceipt(&b, &reciveuser, amount);
                            }
                            else
                            {
                                printf("\033[31m");
                                printf("TRANSFER FAILED!!\n");
                                printf("\033[0m");
                            }
                        }
                        else if (confirm1 == 'N' || confirm1 == 'n')
                        {
                            printf("\033[32m");
                            printf("CANCELED THE TRANSFER..\n");
                            printf("\033[0m");
                        }
                        else
                        {
                            printf("\033[31m");
                            printf("WRONG INPUT!!\n");
                            printf("\033[0m");
                        }

                        break;
                    }

                    case 5:
                    {
                        printf("EXITING.....");
                        break;
                    }
                    default:
                    {
                        printf("\033[31m");
                        printf("!!!!!!!!WRONG OPTION OPT!!!!!!!!\n");
                        printf("\033[0m");
                    }
                    }

                } while (choice_normal != 5);
            }
            else
            {
                printf("\033[31m");
                printf("WRONG PIN!!");
                printf("\033[0m");
            }
        }
    }
    // manager login
    else if (check == m.manageraccount)
    {

        // YEH BEGINNING MAI DATA READ HO CHUKA HAI!!!
        // fp = fopen("manager.txt", "r");
        // fscanf(fp, "%s %d %99[^\n]", m.bankmanager_name, &m.manageraccount, m.masterpassword);

        printf("ENTER YOUR MASTER PASSWORD: ");
        scanf("%99s", checkmanager);
        // checking master password
        if (strcmp(checkmanager, m.masterpassword) == 0)
        {

            printf("\n----------------------------------\n");
            printf("WELCEOME %s\n", m.bankmanager_name);
            printf("------------------------------------\n");

            do
            { // manager menu
                printf("\n[1] ADD USER\n");
                printf("[2] SEARCH USER\n");
                printf("[3] DELETE USER\n");
                printf("[4] EXIT\n");

                printf("SELECT YOUR USER : ");
                scanf("%d", &choice_manager);

                switch (choice_manager)
                {
                case 1:
                {
                    // add user
                    FILE *user;
                    user = fopen("user.txt", "a");
                    if (user == NULL)
                    {
                        printf("\033[31m");
                        printf("ERROR OPENING FILE\n");
                        printf("\033[0m");
                        break;
                    }
                    printf("ENTER NAME OF NEW USER: ");
                    getchar();
                    scanf("%49[^\n]s", b.name);

                    printf("ENTER ACCOUNT NO. OF NEW USER: ");
                    scanf("%d", &b.Account_no);
                    if (b.Account_no == m.manageraccount)
                    {
                        printf("\033[31m");
                        printf("YOU HAVE ENTERED YOUR ACCOUNT NO. PLEASE CHECK YOUR DATA\n");
                        printf("\033[0m");
                        fclose(user);
                        break;
                    }
                    else if (finduser(b.Account_no, &b) == 1)
                    {
                        printf("\033[31m");
                        printf("USER ALREADY EXISTS!!\n");
                        printf("\033[0m");
                        fclose(user);
                        break;
                    }
                    else
                    {
                        printf("ENTER THE INITIAL BALANCE OF ACCOUNT: ");
                        scanf("%f", &b.balance);

                        printf("SET PIN OF USER : ");
                        getchar();
                        scanf("%d", &b.pin);

                        fprintf(user, "%s\n %d\n %.2f\n %d\n", b.name, b.Account_no, b.balance, b.pin);
                        fclose(user);

                        printf("\033[32m");
                        printf("\nUser Added Sucessfully!!\n");
                        printf("\033[0m");
                    }
                    break;
                }
                case 2:
                {
                    // search user
                    fp = fopen("user.txt", "r");
                    if (fp == NULL)
                    {
                        printf("\033[31m");
                        printf("NO USER FOUND!!\n");
                        printf("\033[0m");
                    }
                    else
                    {
                        int found = 0;
                        // searching on basis of account no.
                        printf("ENTER ACCOUNT NO. OF USER: ");
                        int check_accountno;
                        scanf("%d", &check_accountno);

                        // check
                        while (fscanf(fp, " %49[^\n]", b.name) == 1)
                        {
                            fscanf(fp, "%d", &b.Account_no);
                            fscanf(fp, "%f", &b.balance);
                            fscanf(fp, "%d", &b.pin);

                            if (check_accountno == b.Account_no)
                            {

                                found = 1;
                                break;
                            }
                        }

                        fclose(fp);
                        if (found == 0)
                        {
                            printf("\033[31m");
                            printf("USER NOT FOUND!!\n");
                            printf("\033[0m");
                        }
                        else if (found == 1)
                        {
                            printf("\033[32m");
                            printf("\n-------USER DETAILS-------\n");
                            printf("NAME      : %s\n", b.name);
                            printf("ACCOUNT No: %d\n", b.Account_no);
                            printf("BALANCE   : %.2f\n", b.balance);
                            printf("\033[0m");
                        }
                        break;
                    }
                }
                case 3:
                {
                    // delete user
                    // printf("\033[31m");
                    // printf("THIS FEATURE IS NOT AVAILABLE!!!\n");
                    // printf("\033[0m");
                    // break;

                    // printf("ENTER ACCOUNT NO. OF USER: ");
                    // int check_accountno;
                    // scanf("%d", &check_accountno);

                    // case 2 repeat
                    fp = fopen("user.txt", "r");
                    if (fp == NULL)
                    {
                        printf("\033[31m");
                        printf("NO USER FOUND!!\n");
                        printf("\033[0m");
                    }
                    else
                    {
                        int found = 0;
                        // searching on basis of account no.
                        printf("ENTER ACCOUNT NO. OF USER: ");
                        int check_accountno;
                        scanf("%d", &check_accountno);

                        // check
                        while (fscanf(fp, " %49[^\n]", b.name) == 1)
                        {
                            fscanf(fp, "%d", &b.Account_no);
                            fscanf(fp, "%f", &b.balance);
                            fscanf(fp, "%d", &b.pin);

                            if (check_accountno == b.Account_no)
                            {

                                found = 1;
                                break;
                            }
                        }

                        fclose(fp);
                        if (found == 0)
                        {
                            printf("\033[31m");
                            printf("USER NOT FOUND!!\n");
                            printf("\033[0m");
                        }
                        else if (found == 1)
                        {
                            printf("\033[32m");
                            printf("\n-------USER DETAILS-------\n");
                            printf("NAME      : %s\n", b.name);
                            printf("ACCOUNT No: %d\n", b.Account_no);
                            printf("BALANCE   : %.2f\n", b.balance);
                            printf("\033[0m");

                            printf("ARE YOU SURE YOU WANT TO DELETE THIS USER ? (y/n): ");
                            char confirm;
                            getchar();
                            scanf("%c", &confirm);
                            if (confirm == 'Y' || confirm == 'y')
                            {
                                if (deleteuser(check_accountno) == 1)
                                {
                                    printf("\033[32m");
                                    printf("USER DELETED SUCESSFULLY...");
                                    printf("\033[0m");
                                }
                                else
                                {
                                    printf("\033[31m");
                                    printf("USER NOT FOUND!!");
                                    printf("\033[0m");
                                }
                            }
                            else if (confirm == 'N' || confirm == 'n')
                            {
                                printf("DELETE CANCELLED\n");
                            }
                            else
                            {
                                printf("\033[31m");
                                printf("WRONG INPUT!!\n");
                                printf("\033[0m");
                            }
                        }
                        break;
                    }
                }
                case 4:
                {
                    printf("EXITING....");
                    break;
                }
                default:
                {
                    printf("\033[31m");
                    printf("INVALID OPTION OPT!!!!\n");
                    printf("\033[0m");
                }
                }
            } while (choice_manager != 4);
        }
        else
        {
            printf("\033[31m");
            printf("WRONG MASTER PASSWORD!!!");
            printf("\033[0m");
        }
    }

    return 0;
}
