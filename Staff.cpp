#include "Staff.h"

void loadAllStaffData(Staff* &allStaff, string fileName) {
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
    Staff* Del = Dummy;
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
        cout << curStaff->Info.ID << ' ' << StaffID << endl;
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

bool addStudentByCSV(string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not open student CSV file";
        fin.close();
//        system("pause");
        return false;
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

    return true;
}

//-----------------------------------------------------------------------------------------//

// Front End

void staffEditProfile(Staff* tmpStaff) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int textboxWidth = 300;
    const int textboxHeight = 40;
    const int startX = (SCREEN_WIDTH - textboxWidth) / 2;
    const int startY = 220;
    const int plusX = 240;
    const int plusY = 55;

    Staff* allStaff = nullptr;
    loadAllStaffData(allStaff, staffFileName);

    Staff* curStaff = findStaffByID(allStaff, tmpStaff->Info.ID);

    if(!curStaff) {
        cout << "NO";
        return;
    }

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Profile")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        TextOutput saveText = TextOutput(RED, 22);
        saveText.loadText(gRenderer, "You have save your data", FONTDIR);

        vector <Button> listTextbox(2);

        for(int i = 0; i < 2; i++) {
            string curText = "";

            switch (i) {
            case 0:
                curText = curStaff->Info.ID;
                break;

            case 1:
                curText = curStaff->Info.fullName;
                break;
            }

            listTextbox[i] = Button(startX, startY + (textboxHeight + plusY) * i, textboxWidth, textboxHeight, 2, BLACK, WHITE, WHITE, GREY, curText, 20);
        }

        vector <TextOutput> listText;

        for(int i = 0; i < 2; i++) {
            string curText = "";

            switch (i) {
            case 0:
                curText = "ID";
                break;

            case 1:
                curText = "Full name";
                break;
            }

            TextOutput tmp = TextOutput(BLACK, 22);
            tmp.loadText(gRenderer, curText, FONTDIR);

            listText.push_back(tmp);
        }

        bool isSave = false;

        Button saveButton = Button((SCREEN_WIDTH - 100) / 2, 550, 100, 30, 2, BLACK, LIGHTBLUE, RED, RED, "Save", 19);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        event.button.button = SDL_BUTTON_RIGHT;

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                for(int i = 0; i < 2; i++)
                    listTextbox[i].Display(gRenderer);

                for(int i = 0; i < 2; i++)
                    listText[i].Display(gRenderer, startX, startY + (textboxHeight + plusY) * i - 25);

                saveButton.Display(gRenderer);
                backButton.Display(gRenderer);

                for(int i = 1; i < 2; i++) {
                    if(listTextbox[i].isTextBox(gRenderer, &event)) {
                        event.button.button = SDL_BUTTON_RIGHT;
                    }
                }

                int saveButtonState = saveButton.isMouseClick(&event);
                if(saveButtonState == 1) {
                    curStaff->Info.ID = listTextbox[0].Text;
                    curStaff->Info.fullName = listTextbox[1].Text;

                    saveButton.FillCol = saveButton.PressCol;
                    saveAllStaffData(allStaff, staffFileName);
                    isSave = true;
                }
                else if(saveButtonState == 2) {
                    saveButton.FillCol = saveButton.HoverCol;
                }
                else {
                    saveButton.FillCol = saveButton.InitCol;
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
                    staffWindow(curStaff);
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                if(isSave) {
                    saveText.Display(gRenderer, (SCREEN_WIDTH - saveText.mWidth) / 2, 550 + saveButton.bRect.h + 10);
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
                    Width = 230;
                    curText = "First name";
                    curX += 150;
                    break;
                }

                case 2: {
                    Width = 150;
                    curText = "Last name";
                    curX += 230;
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
                    Width = 100;
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
                                Width = 230;
                                curX += 150;
                                break;
                            }

                            case 2: {
                                Width = 150;
                                curX += 230;
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
                                Width = 100;
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

void staffImportDataWindow(int Type) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    string Title = Type ? "Import list students from file" : "Import score board from file";

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, Title)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button directoryButton = Button((SCREEN_WIDTH - 600) / 2, 250, 600, 35, 2, BLACK, WHITE, WHITE, GREY, "", 20);
        directoryButton.TypeBox = 1;
        directoryButton.padding_hoz = 10;

        TextOutput directoryText = TextOutput(BLACK, 22);
        directoryText.loadText(gRenderer, "Directory", FONTDIR);

        TextOutput warningText = TextOutput(RED, 22);
        warningText.loadText(gRenderer, "Invalid directory!", FONTDIR);

        TextOutput successText = TextOutput(RED, 22);
        successText.loadText(gRenderer, "Successful loading!", FONTDIR);

        Button loadButton = Button((SCREEN_WIDTH - 100) / 2, 375, 100, 30, 2, BLACK, LIGHTBLUE, RED, RED, "LOAD", 19);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        string dir = "";

        bool quit = false;
        int isLoad = 0;
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

                int ButtonState = loadButton.isMouseClick(&event);
                if(ButtonState == 1) {
                    if(Type == 0) {
                        if(addStudentByCSV(directoryButton.Text)) {
                            isLoad = 2;
                        }
                        else isLoad = 1;
                    }
                    else {

                    }
                }
                else if(ButtonState == 2) {
                    loadButton.FillCol = loadButton.HoverCol;
                }
                else {
                    loadButton.FillCol = loadButton.InitCol;
                }

                // Display
                directoryButton.Display(gRenderer);
                loadButton.Display(gRenderer);
                backButton.Display(gRenderer);

                directoryText.Display(gRenderer, (SCREEN_WIDTH - 600) / 2, 222);

                if(isLoad == 1) {
                    warningText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, 345);
                }
                if(isLoad == 2) {
                    successText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, 345);
                }

                directoryButton.isTextBox(gRenderer, &event);
                dir = directoryButton.Text;

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

void staffViewStudentInASameYear(string schoolYear, int page) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;
    const int numStudentInAPage = 15;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "List Students In School Year")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button curYearButton = Button((SCREEN_WIDTH - 200) / 2 , 60, 200, 30, 2, BLACK, WHITE, WHITE, WHITE, "School year: " + schoolYear, 20);

        string preText = "<";
        Button preButton = Button(SCREEN_WIDTH / 2 - 40 - 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, preText, 20);

        string nextText = ">";
        Button nextButton = Button(SCREEN_WIDTH / 2 + 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, nextText, 20);

        vector <Button> listTitleButton;
        vector <Button> listContentButton;
        int listWidth[8] = {150, 230, 150, 100, 100, 100, 70, 100};
        string listTitle[8] = {"Student ID", "First name", "Last name", "Gender", "Date of birth", "Social ID", "Class", "School year"};

        int curX = startX, curY = startY;

        for(int i = 0; i < 8; i++) {
            int Width = listWidth[i], Height = 25;

            Button tmp = Button(curX, startY, Width, Height, 2, BLACK, WHITE, RED, RED, listTitle[i], 15);

            listTitleButton.push_back(tmp);
            curX += Width;
        }

        Student* curStudent = allStudent;

        int numStudent = 0;
        Student* tmpStu = curStudent;

        while(tmpStu) {
            if(tmpStu->Info.schoolyear == schoolYear) numStudent++;
            tmpStu = tmpStu->Next;
        }

        int numMaxPage = numStudent / numStudentInAPage;
        if(numStudent % numStudentInAPage != 0) numMaxPage++;

        curY = startY;

        int startStu = page * numStudentInAPage, cnt = 0;

        while(curStudent && cnt < startStu) {
            if(curStudent->Info.schoolyear == schoolYear) ++cnt;
            curStudent = curStudent->Next;
        }

        cnt = 0;

        while(curStudent && cnt < numStudentInAPage) {
            if(curStudent && curStudent->Info.schoolyear == schoolYear) {
                cnt++;
                string listContent[8] = {curStudent->Info.ID, curStudent->Info.firstName, curStudent->Info.lastName, curStudent->Info.Gender, curStudent->Info.Dob,  curStudent->Info.SocialID, curStudent->Info.Class, curStudent->Info.schoolyear};
                curX = startX;
                curY += 25;

                for(int i = 0; i < 8; i++) {
                    int Width = listWidth[i], Height = 25;

                    Button tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, listContent[i], 15);

                    listContentButton.push_back(tmp);
                    curX += Width;
                }
            }

            curStudent = curStudent->Next;
        }

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                for(auto it : listTitleButton) {
                    it.Display(gRenderer);
                }
                for(auto it : listContentButton) {
                    it.Display(gRenderer);
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
                    IMG_Quit();
                    SDL_Quit();

                    studentWindow(curStudent);
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                int preButtonState = preButton.isMouseClick(&event);
                bool isChange = false;
                if(preButtonState == 1) {
                    if(page > 0) {
                        page -= 1;
                        isChange = true;
                    }
                                    }
                else if(preButtonState == 2) {
                    preButton.FillCol = preButton.HoverCol;
                }
                else {
                    preButton.FillCol = preButton.InitCol;
                }

                int nextButtonState = nextButton.isMouseClick(&event);
                if(nextButtonState == 1) {
                    if(page < numMaxPage - 1) {
                        page += 1;
                        isChange = true;
                    }
                }
                else if(nextButtonState == 2) {
                    nextButton.FillCol = nextButton.HoverCol;
                }
                else {
                    nextButton.FillCol = nextButton.InitCol;
                }

                backButton.Display(gRenderer);
                curYearButton.Display(gRenderer);
                preButton.Display(gRenderer);
                nextButton.Display(gRenderer);

                SDL_RenderPresent(gRenderer);

                if(isChange) {
                    listContentButton.clear();
                    curStudent = allStudent;
                    curY = startY;
                    startStu = page * numStudentInAPage, cnt = 0;

                    while(curStudent && cnt < startStu) {
                        if(curStudent->Info.schoolyear == schoolYear) ++cnt;
                        curStudent = curStudent->Next;
                    }

                    cnt = 0;

                    while(curStudent && cnt < numStudentInAPage) {
                        if(curStudent && curStudent->Info.schoolyear == schoolYear) {
                            cnt++;
                            string listContent[8] = {curStudent->Info.ID, curStudent->Info.firstName, curStudent->Info.lastName, curStudent->Info.Gender, curStudent->Info.Dob,  curStudent->Info.SocialID, curStudent->Info.Class, curStudent->Info.schoolyear};
                            curX = startX;
                            curY += 25;

                            for(int i = 0; i < 8; i++) {
                                int Width = listWidth[i], Height = 25;

                                Button tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, listContent[i], 15);

                                listContentButton.push_back(tmp);
                                curX += Width;
                            }
                        }

                        curStudent = curStudent->Next;
                    }
                }
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
                                staffEditProfile(curStaff);
                                return;
                            }

                            case 1: {
                                staffaddStudentByManual();
                                return;
                            }

                            case 2: {
                                return;
                            }

                            case 3: {
                                return;
                            }

                            case 4: {
                                Account* allAccount = nullptr;
                                loadAllAccountData(allAccount, accountFileName);

                                Account* curAccount = findAccountByID(allAccount, curStaff->Info.ID);

                                curAccount->userChangePassword();
                                return;
                            }

                            case 5: {
                                systemProcess();
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
