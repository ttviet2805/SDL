#include "Login.h"

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

Account* loginWindow() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    Account* allAccount = nullptr;
    loadAllAccountData(allAccount, accountFileName);

    const string backgroundPath = "Data/Image/LoginBackground.jpg";

    if(!init(gWindow, gRenderer, "Login")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* background_Image = nullptr;
        loadImage(gRenderer, background_Image, backgroundPath);

        Button userNameButton = Button((SCREEN_WIDTH - 300) / 2, 250, 300, 35, 2, BLACK, WHITE, WHITE, GREY, "", 20);
        userNameButton.TypeBox = 1;
        userNameButton.padding_hoz = 10;

        Button passwordButton = Button((SCREEN_WIDTH - 300) / 2, 350, 300, 35, 2, BLACK, WHITE, WHITE, GREY, "", 20);
        passwordButton.TypeBox = 1;
        passwordButton.padding_hoz = 10;

        TextOutput userNameText = TextOutput(BLACK, 22);
        userNameText.loadText(gRenderer, "Username", FONTDIR);

        TextOutput passwordText = TextOutput(BLACK, 22);
        passwordText.loadText(gRenderer, "Password", FONTDIR);

        TextOutput warningText = TextOutput(RED, 22);
        warningText.loadText(gRenderer, "Incorrect username or password!", FONTDIR);

        Button loginButton = Button((SCREEN_WIDTH - 100) / 2, 450, 100, 30, 2, BLACK, LIGHTBLUE, RED, RED, "LOGIN", 19);

        string curUsername = "", curPassword = "";

        bool isLogin = false;

        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, background_Image, NULL, NULL);

                int loginButtonState = loginButton.isMouseClick(&event);
                if(loginButtonState == 1) {
                    loginButton.FillCol = loginButton.PressCol;

                    Account* curAccount = allAccount;

                    while(curAccount) {
                        if(curAccount->username == curUsername && curAccount->password == curPassword) {
                            cout << "Login successful";

                            SDL_DestroyTexture(background_Image);
                            background_Image = NULL;

                            //Destroy window
                            SDL_DestroyRenderer( gRenderer );
                            SDL_DestroyWindow( gWindow );
                            gWindow = NULL;
                            gRenderer = NULL;

                            // Quit
                            IMG_Quit();
                            SDL_Quit();

                            return curAccount;
                        }

                        curAccount = curAccount->Next;
                    }

                    userNameButton.Text = "";
                    passwordButton.Text = "";

                    cout << "Fail to login" << '\n';
                    isLogin = true;
                }
                else if(loginButtonState == 2) {
                    loginButton.FillCol = loginButton.HoverCol;
                }
                else {
                    loginButton.FillCol = loginButton.InitCol;
                }

                // Display
                userNameButton.Display(gRenderer);
                passwordButton.Display(gRenderer);
                loginButton.Display(gRenderer);

                userNameText.Display(gRenderer, (SCREEN_WIDTH - 300) / 2, 222);
                passwordText.Display(gRenderer, (SCREEN_WIDTH - 300) / 2, 322);

                if(isLogin) {
                    warningText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, 485);
                }

                userNameButton.isTextBox(gRenderer, &event);
                curUsername = userNameButton.Text;

                passwordButton.isTextBox(gRenderer, &event);
                curPassword = passwordButton.Text;

                SDL_RenderPresent(gRenderer);
            }
        }

        SDL_DestroyTexture(background_Image);
        background_Image = NULL;

        //Destroy window
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        // Quit
        IMG_Quit();
        SDL_Quit();
    }

    return nullptr;
}
