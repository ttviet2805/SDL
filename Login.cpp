#include "Login.h"

#define accountFileName "Data/AllAccount.txt"

void loadAllAccountData(Account* &allAccount, string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not find account the file";
        Sleep(3000);
        return;
    }

    // load Data
    Account* Dummy = new Account;
    Account* curAccount = Dummy;

    string curName;
    while(!fin.eof() && getline(fin, curName)) {
        curAccount->Next = new Account;
        curAccount->Next->username = curName;
        getline(fin, curAccount->Next->password);
        fin >> curAccount->Next->type;
        fin.get();
        curAccount = curAccount->Next;
    }

    allAccount = Dummy->Next;

    Account* Del = Dummy;
    Dummy = Dummy->Next;
    delete Del;

    fin.close();
}

void saveAllAccountData(Account* allAccount, string filename) {
    ofstream fout;
    fout.open(filename);

    if(!fout) {
        cout << "Can not open account file";
        Sleep(3000);
        return;
    }

    Account* curAccount = allAccount;

    while(curAccount) {
        fout << curAccount->username << '\n' << curAccount->password << '\n' << curAccount->type << '\n';
        curAccount = curAccount->Next;
    }

    fout.close();
}

void showAllAccountData(Account* allAccount) {
    Account* tmp = allAccount;

    while(tmp) {
        cout << tmp->username << ' ' << tmp->password << ' ' << tmp->type << endl;
        tmp = tmp->Next;
    }
}

Account* findAccountByID(Account* allAccount, string ID) {
    Account* cur = allAccount;

    while(!cur) {
        if(cur->username == ID) return cur;
        cur = cur->Next;
    }

    return nullptr;
}

Account* loginProcess() {
    Account* allAccount = nullptr;
    loadAllAccountData(allAccount, accountFileName);

    while(1) {
        system("cls");
        cout << "Username: ";
        string username; getline(cin, username);
        cout << "Password: ";
        string password; getline(cin, password);

        Account* curAccount = allAccount;

        while(curAccount) {
            if(curAccount->username == username && curAccount->password == password) {
                cout << "Login successful";
                system("pause");
                return curAccount;
            }

            curAccount = curAccount->Next;
        }

        cout << "Fail to login" << '\n';
        system("pause");
    }

    return nullptr;
}

void Account::changePassword() {
    Account* allAccount = nullptr;
    loadAllAccountData(allAccount, accountFileName);

    while(1) {
        system("cls");

        cout << "Enter current password: ";
        string oldPass;
        getline(cin, oldPass);

        if(oldPass != password) {
            cout << "The password is not incorrect\n";
            system("pause");
            continue;
        }

        cout << "Enter your new password: ";
        string newPass;
        getline(cin, newPass);

        password = newPass;
        break;
    }

    saveAllAccountData(allAccount, accountFileName);
}

void addANewAccount(Account* newAccount) {
    Account* allAccount = nullptr;
    loadAllAccountData(allAccount, accountFileName);

    if(!allAccount) {
        allAccount = newAccount;
        saveAllAccountData(allAccount, accountFileName);
        return;
    }

    Account* curAccount = allAccount;

    while(curAccount->Next) {
        curAccount = curAccount->Next;
    }

    curAccount->Next = newAccount;

    saveAllAccountData(allAccount, accountFileName);
}

// -------------------------------------------------------------------------------- //

void loginWindow() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const string backgroundPath = "Data/Image/LoginBackground.jpg";

    if(!init(gWindow, gRenderer, "Login")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* background_Image = nullptr;

        loadImage(gRenderer, background_Image, backgroundPath);

        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderCopy(gRenderer, background_Image, NULL, NULL);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
}
