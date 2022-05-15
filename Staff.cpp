#include "Staff.h"

void loadAllStaffData(Staff* & allStaff, string fileName) {
    ifstream fin;
    fin.open(fileName);

    Staff* Dummy = new Staff;
    Staff* curStaff = Dummy;

    string tmpID;
    while(!fin.eof() && getline(fin, tmpID)) {
        curStaff->Next = new Staff;
        curStaff->Next->Info.ID = tmpID;
        getline(fin, curStaff->Next->Info.fullName);

        curStaff = curStaff->Next;
    }

    allStaff = Dummy->Next;
    Staff* Del = Dummy->Next;
    Dummy = Dummy->Next;

    delete Del;

    fin.close();
}

void saveAllStaffData(Staff* allStaff, string fileName) {
    ofstream fout;
    fout.open(fileName);

    Staff* curStaff = allStaff;

    while(curStaff) {
        fout << curStaff->Info.ID << '\n' << curStaff->Info.fullName << '\n';
        curStaff = curStaff->Next;
    }

    fout.close();
}

Staff* findStaffByID(Staff* allStaff, string StaffID) {
    Staff* curStaff = allStaff;

    while(curStaff) {
        if(curStaff->Info.ID == StaffID)
            return curStaff;

        curStaff = curStaff->Next;
    }

    return nullptr;
}

void showAllStaffData(Staff* allStaff) {
    Staff* curStaff = allStaff;

    while(curStaff) {
        cout << curStaff->Info.ID << ' ' << curStaff->Info.fullName << '\n';
        curStaff = curStaff->Next;
    }
}

void addANewStudent(Student* &allStudent, Student* newStudent) {
    if(!allStudent) {
        allStudent = newStudent;
        return;
    }

    Student* curStudent = allStudent;

    while(curStudent->Next) curStudent = curStudent->Next;

    curStudent->Next = newStudent;
}

void deleteAStudent(Student* &allStudent, Student* delStudent) {
    Student* curStudent = allStudent;

    if(allStudent == delStudent) {
        Student* Del = allStudent;
        allStudent = allStudent->Next;

        delete Del;
        return;
    }

    while(curStudent->Next && curStudent->Next != delStudent) {
        curStudent = curStudent->Next;
    }

    if(!curStudent->Next) return;

    Student* Del = curStudent->Next;

    curStudent->Next = Del->Next;

    delete Del;
}

void viewStudentInAClass(Class* curClass) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    StudentInClass* curStudentInClass = curClass->studentHead;

    while(curStudentInClass) {
        Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);

        curStudent->viewProfile();

        curStudentInClass = curStudentInClass->Next;
    }
}

void exportStudentInAClass(Class* curClass, string fileName) {
    ofstream fout;
    fout.open(fileName);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    StudentInClass* curStudentInClass = curClass->studentHead;

    while(curStudentInClass) {
        Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);
        StudentInfo Info = curStudent->Info;

        fout << Info.ID << '\n' << Info.firstName << '\n' << Info.lastName << '\n' << Info.Gender << '\n';
        fout << Info.Dob << '\n' << Info.SocialID << '\n' << Info.Class << '\n';
        fout << Info.schoolyear << '\n';

        curStudentInClass = curStudentInClass->Next;
    }

    fout.close();
}

void viewStudentInASameYear(string schoolYear) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Student* curStudent = allStudent;

    while(curStudent) {
        if(curStudent->Info.schoolyear == schoolYear) {
            curStudent->viewProfile();
        }

        curStudent = curStudent->Next;
    }
}

void exportStudentInASameYear(string schoolYear, string fileName) {
    ofstream fout;
    fout.open(fileName);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Student* curStudent = allStudent;

    while(curStudent) {
        if(curStudent->Info.schoolyear == schoolYear) {
            StudentInfo Info = curStudent->Info;

            fout << Info.ID << '\n' << Info.firstName << '\n' << Info.lastName << '\n' << Info.Gender << '\n';
            fout << Info.Dob << '\n' << Info.SocialID << '\n' << Info.Class << '\n';
            fout << Info.schoolyear << '\n';
        }

        curStudent = curStudent->Next;
    }

    fout.close();
}

void addStudentByCSV(string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not open student CSV file";
        system("pause");
        return;
    }

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    string ID;
    while(!fin.eof() && getline(fin, ID, ',')) {
        Student* newStudent = new Student;
        newStudent->Info.ID = ID;
        getline(fin, newStudent->Info.firstName, ',');
        getline(fin, newStudent->Info.lastName, ',');
        getline(fin, newStudent->Info.Gender, ',');
        getline(fin, newStudent->Info.Dob, ',');
        getline(fin, newStudent->Info.SocialID, ',');
        getline(fin, newStudent->Info.Class, ',');
        getline(fin, newStudent->Info.schoolyear);

        addANewStudent(allStudent, newStudent);

        newStudent->viewProfile();

        Account* newAccount = new Account;
        newAccount->username = newStudent->Info.ID;
        newAccount->type = 1;
        addANewAccount(newAccount);
    }

    saveAllStudentData(allStudent, studentFileName);

    showAllStudentInfo(allStudent);

    fin.close();
}

//-----------------------------------------------------------------------------------------//

// Front End

void staffaddStudentByManual() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;
    const int buttonHeight = 25;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Add New Students")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button saveButton = Button(SCREEN_WIDTH - 120, 60, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Save", 20);

        Button addButton = Button(SCREEN_WIDTH - 120, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Add", 20);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        TextOutput warningText = TextOutput(RED, 22);
        warningText.loadText(gRenderer, "You have save your data", FONTDIR);

        vector <Button> listButton[10];

        int curX = startX, curY = startY;

        for(int i = 0; i < 8; i++) {
            int Width = 0, Height = 25;
            string curText = "";

            switch (i) {
                case 0: {
                    Width = 150;
                    curText = "Student ID";
                    break;
                }

                case 1: {
                    Width = 250;
                    curText = "First name";
                    curX += 150;
                    break;
                }

                case 2: {
                    Width = 150;
                    curText = "Last name";
                    curX += 250;
                    break;
                }

                case 3: {
                    Width = 100;
                    curText = "Gender";
                    curX += 150;
                    break;
                }

                case 4: {
                    Width = 100;
                    curText = "Date of birth";
                    curX += 100;
                    break;
                }

                case 5: {
                    Width = 100;
                    curText = "Social ID";
                    curX += 100;
                    break;
                }

                case 6: {
                    Width = 70;
                    curText = "Class";
                    curX += 100;
                    break;
                }

                case 7: {
                    Width = 80;
                    curText = "School year";
                    curX += 70;
                    break;
                }
            }

            Button tmp = Button(curX, startY, Width, Height, 2, BLACK, WHITE, RED, RED, curText, 15);

            listButton[i].push_back(tmp);
        }

        bool isSave = false;

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                int backButtonState = backButton.isMouseClick(&event);
                if(backButtonState == 1) {
                    backButton.FillCol = backButton.PressCol;
                    SDL_DestroyTexture(backgroundImage);
                    backgroundImage = NULL;

                    //Destroy window
                    SDL_DestroyRenderer( gRenderer );
                    SDL_DestroyWindow( gWindow );
                    gWindow = NULL;
                    gRenderer = NULL;

                    // Quit
                    TTF_Quit();
                    IMG_Quit();
                    SDL_Quit();
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                int addButtonState = addButton.isMouseClick(&event);
                if(addButtonState == 1) {
                    addButton.FillCol = addButton.PressCol;

                    curY += buttonHeight;
                    curX = startX;

                    for(int i = 0; i < 8; i++) {
                        int Width = 0, Height = 25;
                        string curText = "";

                        switch (i) {
                            case 0: {
                                Width = 150;
                                break;
                            }

                            case 1: {
                                Width = 250;
                                curX += 150;
                                break;
                            }

                            case 2: {
                                Width = 150;
                                curX += 250;
                                break;
                            }

                            case 3: {
                                Width = 100;
                                curX += 150;
                                break;
                            }

                            case 4: {
                                Width = 100;
                                curX += 100;
                                break;
                            }

                            case 5: {
                                Width = 100;
                                curX += 100;
                                break;
                            }

                            case 6: {
                                Width = 70;
                                curX += 100;
                                break;
                            }

                            case 7: {
                                Width = 80;
                                curX += 70;
                                break;
                            }
                        }

                        Button tmp = Button(curX, curY, Width, Height, 2, BLACK, WHITE, RED, RED, curText, 15);

                        listButton[i].push_back(tmp);
                    }
                }
                else if(addButtonState == 2) {
                    addButton.FillCol = addButton.HoverCol;
                }
                else {
                    addButton.FillCol = addButton.InitCol;
                }

                backButton.Display(gRenderer);
                saveButton.Display(gRenderer);
                addButton.Display(gRenderer);

                for(int i = 0; i < 8; i++) {
                    for(auto it : listButton[i]) {
                        it.Display(gRenderer);
                    }
                }

                for(int i = 0; i < 8; i++) {
                    for(int j = 1; j < listButton[i].size(); j++) {
                        int kq = listButton[i][j].isTextBox(gRenderer, &event);

                        if(kq) event.button.button = SDL_BUTTON_RIGHT;
                    }
                }

                int saveButtonState = saveButton.isMouseClick(&event);
                if(saveButtonState == 1) {
                    saveButton.FillCol = saveButton.PressCol;

                    isSave = true;

                    Student* allStudent = nullptr;
                    loadAllStudentData(allStudent, studentFileName);

                    for(int i = 1; i < listButton[0].size(); i++) {
                        Student* newStudent = new Student;
                        newStudent->Info.ID = listButton[0][i].Text;
                        newStudent->Info.firstName = listButton[1][i].Text;
                        newStudent->Info.lastName = listButton[2][i].Text;
                        newStudent->Info.Gender = listButton[3][i].Text;
                        newStudent->Info.Dob = listButton[4][i].Text;
                        newStudent->Info.SocialID = listButton[5][i].Text;
                        newStudent->Info.Class = listButton[6][i].Text;
                        newStudent->Info.schoolyear = listButton[7][i].Text;

                        addANewStudent(allStudent, newStudent);

                        Account* newAccount = new Account;
                        newAccount->username = newStudent->Info.ID;
                        newAccount->type = 1;
                        addANewAccount(newAccount);
                    }

                    saveAllStudentData(allStudent, studentFileName);
                }
                else if(saveButtonState == 2) {
                    saveButton.FillCol = saveButton.HoverCol;
                }
                else {
                    saveButton.FillCol = saveButton.InitCol;
                }

                if(isSave) {
                    warningText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, 40);
                }

                SDL_RenderPresent(gRenderer);
            }
        }

        SDL_DestroyTexture(backgroundImage);
        backgroundImage = NULL;

        //Destroy window
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        // Quit
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
}

void staffViewStudentInAClass(Class* tmpClass) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Class* allClass = nullptr;
    loadAllClassData(allClass, classFileName);

    Class* curClass = findClassByID(allClass, tmpClass->className);

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "List Students In A Class")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button curClassButton = Button((SCREEN_WIDTH - 150) / 2 , 60, 150, 30, 2, BLACK, WHITE, WHITE, WHITE, "Class: " + curClass->className, 20);

        Button exportCSVButton = Button(SCREEN_WIDTH - 350, 50, 300, 30, 2, BLACK, GREY, RED, LIGHTBLUE, "Export to CSV file", 20);

        vector <Button> listButton[10];

        int curX = startX, curY = startY;

        for(int i = 0; i < 8; i++) {
            int Width = 0, Height = 25;
            string curText = "";

            switch (i) {
                case 0: {
                    Width = 150;
                    curText = "Student ID";
                    break;
                }

                case 1: {
                    Width = 250;
                    curText = "First name";
                    curX += 150;
                    break;
                }

                case 2: {
                    Width = 150;
                    curText = "Last name";
                    curX += 250;
                    break;
                }

                case 3: {
                    Width = 100;
                    curText = "Gender";
                    curX += 150;
                    break;
                }

                case 4: {
                    Width = 100;
                    curText = "Date of birth";
                    curX += 100;
                    break;
                }

                case 5: {
                    Width = 100;
                    curText = "Social ID";
                    curX += 100;
                    break;
                }

                case 6: {
                    Width = 70;
                    curText = "Class";
                    curX += 100;
                    break;
                }

                case 7: {
                    Width = 80;
                    curText = "School year";
                    curX += 70;
                    break;
                }
            }

            Button tmp = Button(curX, startY, Width, Height, 2, BLACK, WHITE, RED, RED, curText, 15);

            listButton[i].push_back(tmp);
        }

        StudentInClass* curStudentInClass = curClass->studentHead;

        curY = startY;

        while(curStudentInClass) {
            Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);

            if(curStudent) {
                curX = startX;
                curY += 25;

                for(int i = 0; i < 8; i++) {
                    int Width = 0, Height = 25;
                    string curText = "";

                    switch (i) {
                        case 0: {
                            Width = 150;
                            curText = curStudent->Info.ID;
                            break;
                        }

                        case 1: {
                            Width = 250;
                            curText = curStudent->Info.firstName;
                            curX += 150;
                            break;
                        }

                        case 2: {
                            Width = 150;
                            curText = curStudent->Info.lastName;
                            curX += 250;
                            break;
                        }

                        case 3: {
                            Width = 100;
                            curText = curStudent->Info.Gender;
                            curX += 150;
                            break;
                        }

                        case 4: {
                            Width = 100;
                            curText = curStudent->Info.Dob;
                            curX += 100;
                            break;
                        }

                        case 5: {
                            Width = 100;
                            curText = curStudent->Info.SocialID;
                            curX += 100;
                            break;
                        }

                        case 6: {
                            Width = 70;
                            curText = curStudent->Info.Class;
                            curX += 100;
                            break;
                        }

                        case 7: {
                            Width = 80;
                            curText = curStudent->Info.schoolyear;
                            curX += 70;
                            break;
                        }
                    }

                    Button tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, curText, 15);

                    listButton[i].push_back(tmp);
                }
            }

            curStudentInClass = curStudentInClass->Next;
        }

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                for(int i = 0; i < 8; i++) {
                    for(auto it : listButton[i]) {
                        it.Display(gRenderer);
                    }
                }

                int backButtonState = backButton.isMouseClick(&event);
                if(backButtonState == 1) {
                    backButton.FillCol = backButton.PressCol;
                    SDL_DestroyTexture(backgroundImage);
                    backgroundImage = NULL;

                    //Destroy window
                    SDL_DestroyRenderer( gRenderer );
                    SDL_DestroyWindow( gWindow );
                    gWindow = NULL;
                    gRenderer = NULL;

                    // Quit
                    TTF_Quit();
                    IMG_Quit();
                    SDL_Quit();
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                backButton.Display(gRenderer);
                curClassButton.Display(gRenderer);
                exportCSVButton.Display(gRenderer);

                SDL_RenderPresent(gRenderer);
            }
        }

        SDL_DestroyTexture(backgroundImage);
        backgroundImage = NULL;

        //Destroy window
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        // Quit
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
}

void staffWindow(Staff* curStaff) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 270;
    const int buttonHeight = 140;
    const int startX = 45;
    const int startY = 100;
    const int plusX = 90;
    const int plusY = 60;
    const int buttonTextSize = 27;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Staff")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        TextOutput welcomeText = TextOutput(RED, 22);

        string staffName = curStaff->Info.fullName;

        welcomeText.loadText(gRenderer, "Welcome " + staffName,FONTDIR);


        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        vector <Button> listButton(6);

        listButton[0] = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "Profile", buttonTextSize);

        listButton[1] = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "Add Student", buttonTextSize);

        listButton[2] = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "Add Score", buttonTextSize);

        listButton[3] = Button(startX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "View Student", buttonTextSize);

        listButton[4] = Button(startX + buttonWidth + plusX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "Change Password", buttonTextSize);

        listButton[5] = Button(startX + 2 * (buttonWidth + plusX), startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "Log Out", buttonTextSize);

//        system("pause");

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                welcomeText.Display(gRenderer, SCREEN_WIDTH - welcomeText.mWidth - 20, 10);

                for(int i = 0; i < 6; i++) {
                    listButton[i].Display(gRenderer);
                }

                for(int i = 0; i < 6; i++) {
                    int buttonState = listButton[i].isMouseClick(&event);
                    //listButton[i].FillCol = listButton[i].InitCol;
                    if(buttonState == 1) {
                        listButton[i].FillCol = listButton[i].PressCol;

                        SDL_DestroyTexture(backgroundImage);
                        backgroundImage = NULL;

                        //Destroy window
                        SDL_DestroyRenderer( gRenderer );
                        SDL_DestroyWindow( gWindow );
                        gWindow = NULL;
                        gRenderer = NULL;

                        // Quit
                        TTF_Quit();
                        IMG_Quit();
                        SDL_Quit();

                        switch (i) {
                            case 0: {
                                return;
                            }

                            case 1: {
                                return;
                            }

                            case 2: {
                                return;
                            }

                            case 3: {
                                return;
                            }

                            case 4: {
                                return;
                            }

                            case 5: {
                                return;
                            }
                        }
                    }
                    else if(buttonState == 2) {
                        listButton[i].FillCol = listButton[i].HoverCol;
                    }
                    else {
                        listButton[i].FillCol = listButton[i].InitCol;
                    }
                }

                SDL_RenderPresent(gRenderer);
            }
        }

        SDL_DestroyTexture(backgroundImage);
        backgroundImage = NULL;

        //Destroy window
        SDL_DestroyRenderer( gRenderer );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        gRenderer = NULL;

        // Quit
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }
}
