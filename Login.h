#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>
using namespace std;

#include "Const.h"
#include "ButtonClass.h"
#include "SDLProcess.h"

// Account is account to login
struct Account {
    string username = "", password = "1";
    // type = -1: not staff or student
    // type = 0: staff
    // type = 1: student
    int type = -1;

    Account* Next = nullptr;

    void changePassword();
};

//Back End

void loadAllAccountData(Account* &allAccount, string fileName);

void saveAllAccountData(Account* allAccount, string fileName);

void showAllAccountData(Account* allAccount);

Account* findAccountByID(Account* allAccount, string ID);

Account* loginProcess();

void addANewAccount(Account* newAccount);


//----------------------------------------------------------------------------//

//Front End

Account* loginWindow();




