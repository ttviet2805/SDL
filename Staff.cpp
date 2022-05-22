#include "Staff.h"

int convertStringToInt(string S) {
    int res = 0;

    for(int i = 0; i < S.size(); i++) res = res * 10 + S[i] - '0';

    return res;
}

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

        fout << Info.ID << ',' << Info.firstName << ',' << Info.lastName << ',' << Info.Gender << ',';
        fout << Info.Dob << ',' << Info.SocialID << ',' << Info.Class << ',';
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

            fout << Info.ID << ',' << Info.firstName << ',' << Info.lastName << ',' << Info.Gender << ',';
            fout << Info.Dob << ',' << Info.SocialID << ',' << Info.Class << ',';
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
                        isSave = false;
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

        Button removeButton = Button(SCREEN_WIDTH - 220, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Remove", 20);

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
                    Width = 220;
                    curText = "First name";
                    curX += 150;
                    break;
                }

                case 2: {
                    Width = 140;
                    curText = "Last name";
                    curX += 220;
                    break;
                }

                case 3: {
                    Width = 100;
                    curText = "Gender";
                    curX += 140;
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
                    Width = 90;
                    curText = "Class";
                    curX += 100;
                    break;
                }

                case 7: {
                    Width = 100;
                    curText = "School year";
                    curX += 90;
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
                    exit(0);
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

                    staffChooseOptionWindow(0, "");
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
                                Width = 220;
                                curX += 150;
                                break;
                            }

                            case 2: {
                                Width = 140;
                                curX += 220;
                                break;
                            }

                            case 3: {
                                Width = 100;
                                curX += 140;
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
                                Width = 90;
                                curX += 100;
                                break;
                            }

                            case 7: {
                                Width = 100;
                                curX += 90;
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
                removeButton.Display(gRenderer);

                for(int i = 0; i < 8; i++) {
                    for(auto it : listButton[i]) {
                        it.Display(gRenderer);
                    }
                }

                for(int i = 0; i < 8; i++) {
                    for(int j = 1; j < listButton[i].size(); j++) {
                        int kq = listButton[i][j].isTextBox(gRenderer, &event);

                        if(kq) event.button.button = SDL_BUTTON_RIGHT;
                        isSave = false;
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

                int removeButtonState = removeButton.isMouseClick(&event);
                if(removeButtonState == 1) {
                    removeButton.FillCol = removeButton.PressCol;

                    if(listButton[0].size() > 1) {
                        for(int i = 0; i < 8; i++) {
                            listButton[i].pop_back();
                        }

                        curY -= buttonHeight;
                    }
                }
                else if(removeButtonState == 2) {
                    removeButton.FillCol = removeButton.HoverCol;
                }
                else {
                    removeButton.FillCol = removeButton.InitCol;
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

void staffaddScoreByManual(Course* curCourse) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 265, startY = 150;
    const int buttonHeight = 25;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Add score")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button saveButton = Button(SCREEN_WIDTH - 120, 60, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Save", 20);

        Button addButton = Button(SCREEN_WIDTH - 120, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Add", 20);

        string courseIDText = "Course ID: "  + curCourse->Info.courseID;
        Button courseIDButton = Button((SCREEN_WIDTH - 350) / 2, 25, 350, 30, 2, BLACK, WHITE, RED, RED, courseIDText,20);

        string courseNameText = "Course name: "  + curCourse->Info.courseName;
        Button courseNameButton = Button((SCREEN_WIDTH - 600) / 2, 65, 600, 30, 2, BLACK, WHITE, RED, RED, courseNameText,20);

        TextOutput warningText = TextOutput(RED, 22);
        warningText.loadText(gRenderer, "You have save your data", FONTDIR);

        Button removeButton = Button(SCREEN_WIDTH - 220, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, RED, "Remove", 20);

        bool isSave = false;

        vector <Button> listButton[10];

        int curX = startX, curY = startY;

        string curText[] = {"Student ID", "Midterm", "Final", "Other"};
        const int Width[] = {250, 100, 100, 100};

        for(int i = 0; i < 4; i++) {
            if(i > 0) {
                curX += Width[i - 1];
            }

            Button tmp = Button(curX, startY, Width[i], buttonHeight, 2, BLACK, WHITE, RED, RED, curText[i], 15);

            listButton[i].push_back(tmp);
        }

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                for(int i = 0; i < 4; i++) {
                    for(auto it : listButton[i]) {
                        it.Display(gRenderer);
                    }
                }

                backButton.Display(gRenderer);
                saveButton.Display(gRenderer);
                addButton.Display(gRenderer);
                courseIDButton.Display(gRenderer);
                courseNameButton.Display(gRenderer);
                removeButton.Display(gRenderer);

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

                    staffChooseOptionWindow(1, curCourse->Info.courseID);
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

                    for(int i = 0; i < 7; i++) {
                        if(i > 0) curX += Width[i - 1];

                        Button tmp = Button(curX, curY, Width[i], buttonHeight, 2, BLACK, WHITE, RED, RED, "", 15);
                        listButton[i].push_back(tmp);
                    }
                }
                else if(addButtonState == 2) {
                    addButton.FillCol = addButton.HoverCol;
                }
                else {
                    addButton.FillCol = addButton.InitCol;
                }

                for(int i = 0; i < 4; i++) {
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

                    Course* allCourse = nullptr;
                    loadAllCourseData(allCourse, courseFileName, allStudent);

                    Course* nowCourse = findCourseByID(allCourse, curCourse->Info.courseID);

                    for(int i = 1; i < listButton[0].size(); i++) {
                        int Midterm = convertStringToInt(listButton[1][i].Text);
                        int Final = convertStringToInt(listButton[2][i].Text);
                        int Other = convertStringToInt(listButton[3][i].Text);
                        CourseScore* newCourseScore = createACourseScore(listButton[0][i].Text, Midterm, Final, Other);

                        nowCourse->addCourseScore(newCourseScore);
                    }

                    saveAllCourseData(allCourse, courseFileName);
                }
                else if(saveButtonState == 2) {
                    saveButton.FillCol = saveButton.HoverCol;
                }
                else {
                    saveButton.FillCol = saveButton.InitCol;
                }

                int removeButtonState = removeButton.isMouseClick(&event);
                if(removeButtonState == 1) {
                    removeButton.FillCol = removeButton.PressCol;

                    if(listButton[0].size() > 1) {
                        for(int i = 0; i < 4; i++) {
                            listButton[i].pop_back();
                        }

                        curY -= buttonHeight;
                    }
                }
                else if(removeButtonState == 2) {
                    removeButton.FillCol = removeButton.HoverCol;
                }
                else {
                    removeButton.FillCol = removeButton.InitCol;
                }

                if(isSave) {
                    warningText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, 110);
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

void staffImportDataWindow(int Type, string courseID) {
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

        Student* allStudent = nullptr;
        loadAllStudentData(allStudent, studentFileName);

        Course* allCourse = nullptr;
        loadAllCourseData(allCourse, courseFileName, allStudent);

        Course* curCourse;
        Button courseIDButton, courseNameButton;

        if(Type == 1) {
            curCourse = findCourseByID(allCourse, courseID);

            string courseIDText = "Course ID: "  + curCourse->Info.courseID;
            Button courseIDButton = Button((SCREEN_WIDTH - 200) / 2, 20, 200, 30, 2, BLACK, WHITE, RED, RED, courseIDText,20);

            string courseNameText = "Course name: "  + curCourse->Info.courseName;
            Button courseNameButton = Button((SCREEN_WIDTH - 600) / 2, 65, 600, 30, 2, BLACK, WHITE, RED, RED, courseNameText,20);
        }

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

                    if(Type == 0) staffChooseOptionWindow(0, "");
                    else staffChooseOptionWindow(1, courseID);
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
                        if(addCourseScoreByCSV(courseID, directoryButton.Text)) {
                            isLoad = 2;
                        }
                        else isLoad = 1;
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

                if(Type == 1) {
                    courseIDButton.Display(gRenderer);
                    courseNameButton.Display(gRenderer);
                }

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

void staffViewStudentInAClass(Class* curClass, int page) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;
    const int numStudentInAPage = 15;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "List Students In A Class")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button exportFileButton = Button(SCREEN_WIDTH - 350, 50, 300, 30, 2, BLACK, GREY, RED, LIGHTBLUE, "Export to CSV file", 20);

        TextOutput exportOKButton = TextOutput(RED, 22);
        exportOKButton.loadText(gRenderer, "You have export this students to file", FONTDIR);

        bool isExport = false;

        string curClassName = "Class: " + curClass->className;
        Button curClassButton = Button((SCREEN_WIDTH - 200) / 2 , 60, 200, 30, 2, BLACK, WHITE, WHITE, WHITE, curClassName, 20);

        string preText = "<";
        Button preButton = Button(SCREEN_WIDTH / 2 - 40 - 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, preText, 20);

        string nextText = ">";
        Button nextButton = Button(SCREEN_WIDTH / 2 + 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, nextText, 20);

        vector <Button> listTitleButton;
        vector <Button> listContentButton[10];
        int listWidth[9] = {150, 220, 140, 100, 100, 100, 90, 100, 25};
        string listTitle[9] = {"Student ID", "First name", "Last name", "Gender", "Date of birth", "Social ID", "Class", "School year", ""};

        int curX = startX, curY = startY;

        for(int i = 0; i < 9; i++) {
            int Width = listWidth[i], Height = 25;

            Button tmp = Button(curX, startY, Width, Height, 2, BLACK, WHITE, RED, RED, listTitle[i], 15);

            listTitleButton.push_back(tmp);
            curX += Width;
        }

        Student* curStudent = allStudent;

        int numStudent = 0;
        StudentInClass* curStudentInClass = curClass->studentHead;

        while(curStudentInClass) {
            curStudentInClass = curStudentInClass->Next;
            numStudent++;
        }

        int numMaxPage = numStudent / numStudentInAPage;
        if(numStudent % numStudentInAPage != 0) numMaxPage++;

        curY = startY;

        int startStu = page * numStudentInAPage, cnt = 0;

        curStudentInClass = curClass->studentHead;

        while(curStudentInClass && cnt < numStudentInAPage) {
            Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);

            if(curStudent) {
                cnt++;
                string listContent[] = {curStudent->Info.ID, curStudent->Info.firstName, curStudent->Info.lastName, curStudent->Info.Gender, curStudent->Info.Dob,  curStudent->Info.SocialID, curStudent->Info.Class, curStudent->Info.schoolyear, ""};
                curX = startX;
                curY += 25;

                for(int i = 0; i < 9; i++) {
                    int Width = listWidth[i];
                    Button tmp;
                    if(i != 8)
                        tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, listContent[i], 15);
                    else
                        tmp = Button(curX, curY, Width, 25, 2, BLACK, RED, LIGHTBLUE, RED, listContent[i], 15);

                    listContentButton[i].push_back(tmp);
                    curX += Width;
                }
            }

            curStudentInClass = curStudentInClass->Next;
        }

        SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
        for(auto it : listTitleButton) {
            it.Display(gRenderer);
        }

        for(int i = 0; i < 9; i++) {
            for(auto it : listContentButton[i]) {
                it.Display(gRenderer);
            }
        }

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
                    break;
                }

                //SDL_RenderClear(gRenderer);

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
                    TTF_Quit();

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
                curClassButton.Display(gRenderer);
                preButton.Display(gRenderer);
                nextButton.Display(gRenderer);
                exportFileButton.Display(gRenderer);

                int exportState = exportFileButton.isMouseClick(&event);
                if(exportState == 1) {
                    exportFileButton.FillCol = exportFileButton.PressCol;
                    isExport = true;

                    exportStudentInAClass(curClass, exportClassFilename + curClass->className + ".csv");
                }
                else if(exportState == 2) {
                    exportFileButton.FillCol = exportFileButton.HoverCol;
                }
                else {
                    exportFileButton.FillCol = exportFileButton.InitCol;
                }

                if(isExport) {
                    exportOKButton.Display(gRenderer, (SCREEN_WIDTH - exportOKButton.mWidth) / 2, 30);
                }

                for(int i = 0; i < listContentButton[0].size(); i++) {
                    int okState = listContentButton[8][i].isMouseClick(&event);
                    if(okState == 1) {
                        listContentButton[8][i].FillCol = listContentButton[8][i].PressCol;
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
                        TTF_Quit();
                        string studentID = listContentButton[0][i].Text;

                        Student* curStudent = findStudentByID(allStudent, studentID);

                        studentViewScore(curStudent);
                        staffViewStudentInAClass(curClass, 0);
                        return;
                    }
                    else if(okState == 2) {
                        listContentButton[8][i].FillCol = listContentButton[8][i].HoverCol;
                    }
                    else {
                        listContentButton[8][i].FillCol = listContentButton[8][i].InitCol;
                    }
                }

                SDL_RenderPresent(gRenderer);

                if(isChange) {
                    for(int i = 0; i < 9; i++)
                        listContentButton[i].clear();

                    curStudentInClass = curClass->studentHead;
                    curY = startY;
                    startStu = page * numStudentInAPage, cnt = 0;

                    while(curStudentInClass && cnt < startStu) {
                        ++cnt;
                        curStudentInClass = curStudentInClass->Next;
                    }

                    cnt = 0;

                    while(curStudentInClass && cnt < numStudentInAPage) {
                        Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);

                        if(curStudent) {
                            cnt++;
                            string listContent[8] = {curStudent->Info.ID, curStudent->Info.firstName, curStudent->Info.lastName, curStudent->Info.Gender, curStudent->Info.Dob,  curStudent->Info.SocialID, curStudent->Info.Class, curStudent->Info.schoolyear};
                            curX = startX;
                            curY += 25;

                            for(int i = 0; i < 9; i++) {
                                int Width = listWidth[i];
                                Button tmp;
                                if(i != 8)
                                    tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, listContent[i], 15);
                                else
                                    tmp = Button(curX, curY, Width, 25, 2, BLACK, RED, LIGHTBLUE, RED, listContent[i], 15);

                                listContentButton[i].push_back(tmp);
                                curX += Width;
                            }
                        }

                        curStudentInClass = curStudentInClass->Next;
                    }

                    SDL_RenderClear(gRenderer);
                    SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
                    for(auto it : listTitleButton) {
                        it.Display(gRenderer);
                    }

                    for(int i = 0; i < 9; i++) {
                        for(auto it : listContentButton[i]) {
                            it.Display(gRenderer);
                        }
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

        Button exportFileButton = Button(SCREEN_WIDTH - 350, 50, 300, 30, 2, BLACK, GREY, RED, LIGHTBLUE, "Export to CSV file", 20);

        TextOutput exportOKButton = TextOutput(RED, 22);
        exportOKButton.loadText(gRenderer, "You have export this students to file", FONTDIR);

        bool isExport = false;

        Button curYearButton = Button((SCREEN_WIDTH - 200) / 2 , 60, 200, 30, 2, BLACK, WHITE, WHITE, WHITE, "School year: " + schoolYear, 20);

        string preText = "<";
        Button preButton = Button(SCREEN_WIDTH / 2 - 40 - 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, preText, 20);

        string nextText = ">";
        Button nextButton = Button(SCREEN_WIDTH / 2 + 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, nextText, 20);

        vector <Button> listTitleButton;
        vector <Button> listContentButton;
        int listWidth[8] = {150, 220, 140, 100, 100, 100, 90, 100};
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
        SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
        for(auto it : listTitleButton) {
            it.Display(gRenderer);
        }
        for(auto it : listContentButton) {
            it.Display(gRenderer);
        }

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
                    break;
                }

                //SDL_RenderClear(gRenderer);

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

                int exportState = exportFileButton.isMouseClick(&event);
                if(exportState == 1) {
                    exportFileButton.FillCol = exportFileButton.PressCol;
                    isExport = true;

                    exportStudentInASameYear(schoolYear, exportSchoolYearFilename + schoolYear + ".csv");
                }
                else if(exportState == 2) {
                    exportFileButton.FillCol = exportFileButton.HoverCol;
                }
                else {
                    exportFileButton.FillCol = exportFileButton.InitCol;
                }

                if(isExport) {
                    exportOKButton.Display(gRenderer, (SCREEN_WIDTH - exportOKButton.mWidth) / 2, 30);
                }

                exportFileButton.Display(gRenderer);

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
                    SDL_RenderClear(gRenderer);
                    SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
                    for(auto it : listTitleButton) {
                        it.Display(gRenderer);
                    }
                    for(auto it : listContentButton) {
                        it.Display(gRenderer);
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

void staffViewAllCourse() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 300;
    const int buttonHeight = 30;
    const int startX = (SCREEN_WIDTH - buttonWidth) / 2;
    const int startY = 100;
    const int plusY = 15;

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "View All Courses")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Course* curCourse = allCourse;

        vector <Button> listButton;
        int cnt = 0;

        while(curCourse) {
            Button tmp = Button(startX, startY + (buttonHeight + plusY) * cnt, buttonWidth, buttonHeight, 2, BLACK, DARKGREEN, RED, RED, curCourse->Info.courseID, 20);
            tmp.textColor = WHITE;

            listButton.push_back(tmp);
            ++cnt;
            curCourse = curCourse->Next;
        }

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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
                    staffChooseScoreOption();
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                // Display
                backButton.Display(gRenderer);

                for(auto it : listButton) {
                    int listButtonState = it.isMouseClick(&event);
                    if(listButtonState == 1) {
                        it.FillCol = it.PressCol;
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
                        staffChooseOptionWindow(1, it.Text);
                        staffViewAllCourse();
                        return;
                    }
                    else if(listButtonState == 2) {
                        it.FillCol = it.HoverCol;
                    }
                    else {
                        it.FillCol = it.InitCol;
                    }
                }

                for(auto i : listButton) {
                    i.Display(gRenderer);
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
        IMG_Quit();
        SDL_Quit();
    }
}

void staffViewCourseScore(Course* curCourse) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 165, startY = 150;
    const int buttonHeight = 25;
    const int numStudentInAPage = 10;
    int numAllStudent = 0, numMaxPage = 0, page = 0;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "View score")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        string courseIDText = "Course ID: "  + curCourse->Info.courseID;
        Button courseIDButton = Button((SCREEN_WIDTH - 350) / 2, 20, 350, 30, 2, BLACK, WHITE, RED, RED, courseIDText,20);

        string courseNameText = "Course name: "  + curCourse->Info.courseName;
        Button courseNameButton = Button((SCREEN_WIDTH - 600) / 2, 65, 600, 30, 2, BLACK, WHITE, RED, RED, courseNameText,20);

        Button exportFileButton = Button(SCREEN_WIDTH - 330, 27, 300, 30, 2, BLACK, GREY, RED, LIGHTBLUE, "Export to CSV file", 20);

        TextOutput exportOKButton = TextOutput(RED, 22);
        exportOKButton.loadText(gRenderer, "You have export your score to file", FONTDIR);

        string preText = "<";
        Button preButton = Button(SCREEN_WIDTH / 2 - 40 - 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, preText, 20);

        string nextText = ">";
        Button nextButton = Button(SCREEN_WIDTH / 2 + 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, nextText, 20);

        bool isExport = false;

        vector <Button> listButton[10], listTitleButton[10];

        int curX = startX, curY = startY;
        string curText[] = {"Student ID", "Midterm", "Final", "Other", "Total", "GPA"};
        const int Width[] = {250, 100, 100, 100, 100, 100};

        for(int i = 0; i < 6; i++) {
            if(i > 0) {
                curX += Width[i - 1];
            }

            Button tmp = Button(curX, startY, Width[i], buttonHeight, 2, BLACK, WHITE, RED, RED, curText[i], 15);

            listTitleButton[i].push_back(tmp);
        }

        CourseScore* curScore = curCourse->courseScoreHead;

        while(curScore) {
            ++numAllStudent;
            curScore = curScore->Next;
        }

        numMaxPage = numAllStudent / numStudentInAPage;
        if(numAllStudent % numStudentInAPage != 0) numMaxPage++;

        curScore = curCourse->courseScoreHead;

        curY = startY;
        int cnt = 0;

        while(curScore && cnt < numStudentInAPage) {
            curX = startX;
            curY += buttonHeight;
            ++cnt;

            for(int i = 0; i < 6; i++) {
                if(i > 0) {
                    curX += Width[i - 1];
                }

                string tmpText = "";
                stringstream ss;

                if(i == 0) tmpText = curScore->StudentID;
                if(i == 1) {
                    ss << fixed << setprecision(2) << curScore->studentScore.MidTerm << ' ';
                    ss >> tmpText;
                }
                if(i == 2) {
                    ss << fixed << setprecision(2) << curScore->studentScore.Final << ' ';
                    ss >> tmpText;
                }
                if(i == 3) {
                    ss << fixed << setprecision(2) << curScore->studentScore.Other << ' ';
                    ss >> tmpText;
                }
                curScore->studentScore.calTotal();
                if(i == 4) {
                    ss << fixed << setprecision(2) << curScore->studentScore.MidTerm << ' ';
                    ss >> tmpText;
                }
                if(i == 5) {
                    tmpText = "";
                    tmpText += curScore->studentScore.GPA;
                }

                Button tmp = Button(curX, curY, Width[i], buttonHeight, 2, BLACK, WHITE, RED, RED, tmpText, 15);
                listButton[i].push_back(tmp);
            }

            curScore = curScore->Next;
        }

        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
        for(int i = 0; i < 6; i++) {
            for(auto it: listTitleButton[i])
                it.Display(gRenderer);

            for(auto it: listButton[i])
                it.Display(gRenderer);
        }

        bool isChange = false;

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
                    break;
                }

                backButton.Display(gRenderer);
                courseIDButton.Display(gRenderer);
                courseNameButton.Display(gRenderer);
                preButton.Display(gRenderer);
                nextButton.Display(gRenderer);

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

                int exportState = exportFileButton.isMouseClick(&event);
                if(exportState == 1) {
                    exportFileButton.FillCol = exportFileButton.PressCol;
                    isExport = true;

                    Student* allStudent = nullptr;
                    loadAllStudentData(allStudent, studentFileName);

                    Course* allCourse = nullptr;
                    loadAllCourseData(allCourse, courseFileName, allStudent);

                    Course* currentCourse = findCourseByID(allCourse, curCourse->Info.courseID);
                    currentCourse->exportCourseScoreToCSVFile(exportCourseScoreFilename + currentCourse->Info.courseID + ".csv");
                }
                else if(exportState == 2) {
                    exportFileButton.FillCol = exportFileButton.HoverCol;
                }
                else {
                    exportFileButton.FillCol = exportFileButton.InitCol;
                }

                if(isExport) {
                    exportOKButton.Display(gRenderer, SCREEN_WIDTH - 340, 1);
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

                if(isChange) {
                    for(int i = 0; i < 6; i++)
                        listButton[i].clear();

                    isChange = false;

                    curScore = curCourse->courseScoreHead;

                    curY = startY;
                    int startStudent = page * numStudentInAPage;
                    cnt = 0;

                    while(curScore && cnt < startStudent) {
                        ++cnt;
                        curScore = curScore->Next;
                    }

                    cnt = 0;

                    while(curScore && cnt < numStudentInAPage) {
                        curX = startX;
                        curY += buttonHeight;
                        ++cnt;

                        for(int i = 0; i < 6; i++) {
                            if(i > 0) {
                                curX += Width[i - 1];
                            }

                            string tmpText = "";
                            stringstream ss;

                            if(i == 0) tmpText = curScore->StudentID;
                            if(i == 1) {
                                ss << fixed << setprecision(2) << curScore->studentScore.MidTerm << ' ';
                                ss >> tmpText;
                            }
                            if(i == 2) {
                                ss << fixed << setprecision(2) << curScore->studentScore.Final << ' ';
                                ss >> tmpText;
                            }
                            if(i == 3) {
                                ss << fixed << setprecision(2) << curScore->studentScore.Other << ' ';
                                ss >> tmpText;
                            }
                            curScore->studentScore.calTotal();
                            if(i == 4) {
                                ss << fixed << setprecision(2) << curScore->studentScore.MidTerm << ' ';
                                ss >> tmpText;
                            }
                            if(i == 5) {
                                tmpText = "";
                                tmpText += curScore->studentScore.GPA;
                            }

                            Button tmp = Button(curX, curY, Width[i], buttonHeight, 2, BLACK, WHITE, RED, RED, tmpText, 15);
                            listButton[i].push_back(tmp);
                        }

                        curScore = curScore->Next;
                    }

                    SDL_RenderClear(gRenderer);

                    SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);
                    for(int i = 0; i < 6; i++) {
                        for(auto it: listTitleButton[i])
                            it.Display(gRenderer);

                        for(auto it: listButton[i])
                            it.Display(gRenderer);
                    }
                }

                exportFileButton.Display(gRenderer);

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

        listButton[0] = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Profile", buttonTextSize);

        listButton[1] = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Add Student", buttonTextSize);

        listButton[2] = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Score", buttonTextSize);

        listButton[3] = Button(startX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "View Student", buttonTextSize);

        listButton[4] = Button(startX + buttonWidth + plusX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Change Password", buttonTextSize);

        listButton[5] = Button(startX + 2 * (buttonWidth + plusX), startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Log Out", buttonTextSize);

        for(int i = 0; i < 6; i++) {
            listButton[i].textColor = WHITE;
        }
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
                                staffChooseOptionWindow(0, "");
                                staffWindow(curStaff);
                                return;
                            }

                            case 2: {
                                staffChooseScoreOption();
                                staffWindow(curStaff);
                                return;
                            }

                            case 3: {
                                staffViewStudentWindow();
                                staffWindow(curStaff);
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

void staffViewListClassWindow() {
    Class* allClass = nullptr;
    loadAllClassData(allClass, classFileName);

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "List class")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Class* curClass = allClass;
        const int numClassPerCol = 6;
        const int startX = 80, startY = 100;
        const int plusX = 60, plusY = 60;

        int cnt = 0, curX = startX, curY = startY;
        int buttonWidth = 150, buttonHeight = 30;

        vector <Button> listButton;

        while(curClass) {
            ++cnt;

            if(cnt > numClassPerCol) {
                cnt = 1;
                curX += plusX + buttonWidth, curY = startY;
            }

            Button tmp = Button(curX, curY, buttonWidth, buttonHeight, 2, BLACK, DARKGREEN, RED, GREY, curClass->className, 20);
            curClass = curClass->Next;

            curY += plusY;

            listButton.push_back(tmp);
        }

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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

                for(int i = 0; i < listButton.size(); ++i) {
                    int ButtonState = listButton[i].isMouseClick(&event);
                    if(ButtonState == 1) {
                        listButton[i].FillCol = listButton[i].PressCol;

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

                        Class* allClass = nullptr;
                        loadAllClassData(allClass, classFileName);

                        Class* curClass = findClassByID(allClass, listButton[i].Text);
                        staffViewStudentInAClass(curClass, 0);
                        staffViewListClassWindow();
                        return;
                    }
                    else if(ButtonState == 2) {
                        listButton[i].FillCol = listButton[i].HoverCol;
                    }
                    else {
                        listButton[i].FillCol = listButton[i].InitCol;
                    }
                }

                // Display
                for(auto it: listButton) {
                    it.Display(gRenderer);
                }
                backButton.Display(gRenderer);

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

void staffEnterSchoolYearWindow() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 270;
    const int buttonHeight = 30;
    const int startX = (SCREEN_WIDTH - buttonWidth) / 2;
    const int startY = 150;
    const int plusY = 60;
    const int buttonTextSize = 27;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Enter school year")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button schoolYearTextbox = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, WHITE, RED, GREY, "", 20);
        TextOutput schoolYearText = TextOutput(BLACK, 20);
        schoolYearText.loadText(gRenderer, "School year", FONTDIR);

        Button enterButton = Button((SCREEN_WIDTH - 100) / 2, startY + 50, 100, 30, 2, BLACK, LIGHTBLUE, RED, GREY, "Enter", 19);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

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
                    staffViewStudentWindow();
                    return;
                }
                else if(backButtonState == 2) {
                    backButton.FillCol = backButton.HoverCol;
                }
                else {
                    backButton.FillCol = backButton.InitCol;
                }

                int enterButtonState = enterButton.isMouseClick(&event);
                if(enterButtonState == 1) {
                    enterButton.FillCol = enterButton.PressCol;
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

                    staffViewStudentInASameYear(schoolYearTextbox.Text,0);
                    staffEnterSchoolYearWindow();
                    return;
                }
                else if(enterButtonState == 2) {
                    enterButton.FillCol = enterButton.HoverCol;
                }
                else {
                    enterButton.FillCol = enterButton.InitCol;
                }

                schoolYearTextbox.Display(gRenderer);
                schoolYearText.Display(gRenderer, startX, startY - buttonHeight);
                backButton.Display(gRenderer);

                enterButton.Display(gRenderer);

                schoolYearTextbox.isTextBox(gRenderer, &event);

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

void staffViewStudentWindow() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 270;
    const int buttonHeight = 140;
    const int startX = 45;
    const int startY = 150;
    const int plusX = 90;
    const int plusY = 60;
    const int buttonTextSize = 27;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "View Student")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button viewClassStudentButton, viewSchoolYearStudentButton;

        viewSchoolYearStudentButton = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "View Students In Same School Year", 25);

        viewClassStudentButton = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "View Students In Class", 25);

        viewClassStudentButton.textColor = WHITE;
        viewSchoolYearStudentButton.textColor = WHITE;

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button findStudentButton = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Find Student Information", 25);
        findStudentButton.textColor = WHITE;

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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

                int viewSchoolYearStudentState = viewSchoolYearStudentButton.isMouseClick(&event);
                if(viewSchoolYearStudentState == 1) {
                    viewSchoolYearStudentButton.FillCol = viewSchoolYearStudentButton.PressCol;
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

                    staffEnterSchoolYearWindow();
                    return;
                }
                else if(viewSchoolYearStudentState == 2) {
                    viewSchoolYearStudentButton.FillCol = viewSchoolYearStudentButton.HoverCol;
                }
                else {
                    viewSchoolYearStudentButton.FillCol = viewSchoolYearStudentButton.InitCol;
                }

                int viewClassStudentState = viewClassStudentButton.isMouseClick(&event);
                if(viewClassStudentState == 1) {
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

                    viewClassStudentButton.FillCol = viewClassStudentButton.PressCol;
                    staffViewListClassWindow();
                    staffViewStudentWindow();
                    return;
                }
                else if(viewClassStudentState == 2) {
                    viewClassStudentButton.FillCol = viewClassStudentButton.HoverCol;
                }
                else {
                    viewClassStudentButton.FillCol = viewClassStudentButton.InitCol;
                }

                int findStudentState = findStudentButton.isMouseClick(&event);
                if(findStudentState == 1) {
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

                    findStudentButton.FillCol = findStudentButton.PressCol;
                    staffFindStudentWindow(0);
                    staffViewStudentWindow();
                    return;
                }
                else if(findStudentState == 2) {
                    findStudentButton.FillCol = findStudentButton.HoverCol;
                }
                else {
                    findStudentButton.FillCol = findStudentButton.InitCol;
                    }


                // Display
                viewSchoolYearStudentButton.Display(gRenderer);
                viewClassStudentButton.Display(gRenderer);
                findStudentButton.Display(gRenderer);
                backButton.Display(gRenderer);

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

void staffChooseOptionWindow(int Type, string courseID) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 270;
    const int buttonHeight = 140;
    const int startX = 45;
    const int startY = 150;
    const int plusX = 90;
    const int plusY = 60;
    const int buttonTextSize = 27;

    string Title = Type ? "Add student" : "Score";

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, Title)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button addManualButton, addCsvButton, viewScoreButton, courseIDButton;

        if(Type == 0) {
            addManualButton = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Add Student By Manual", 25);

            addCsvButton = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Add Student By Csv", 25);
        }
        else {
            addManualButton = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Add Score by Manual", 25);

            addCsvButton = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Add Score By Csv", 25);

            viewScoreButton = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "View Course Score", 25);

            courseIDButton = Button((SCREEN_WIDTH - 350) / 2, 40, 350, 40, 2, BLACK, WHITE, RED, RED, courseID, 20);
        }

        addManualButton.textColor = WHITE;
        addCsvButton.textColor = WHITE;
        viewScoreButton.textColor = WHITE;

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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

                int manualButtonState = addManualButton.isMouseClick(&event);
                if(manualButtonState == 1) {
                    addManualButton.FillCol = addManualButton.PressCol;
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

                    if(Type == 0) {
                        staffaddStudentByManual();
                    }
                    else {
                        if(Type == 1){
                            Student* allStudent = nullptr;
                            loadAllStudentData(allStudent, studentFileName);

                            Course* allCourse = nullptr;
                            loadAllCourseData(allCourse, courseFileName, allStudent);

                            Course* curCourse = findCourseByID(allCourse, courseID);

                            staffaddScoreByManual(curCourse);
                        }
                    }
                    return;
                }
                else if(manualButtonState == 2) {
                    addManualButton.FillCol = addManualButton.HoverCol;
                }
                else {
                    addManualButton.FillCol = addManualButton.InitCol;
                }

                int csvButtonState = addCsvButton.isMouseClick(&event);
                if(csvButtonState == 1) {
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

                    addCsvButton.FillCol = addCsvButton.PressCol;

                    if(Type == 0) {
                        staffImportDataWindow(0, "");
                    }
                    else {
                        if(Type == 1) staffImportDataWindow(1, courseID);
                    }

                    return;
                }
                else if(csvButtonState == 2) {
                    addCsvButton.FillCol = addCsvButton.HoverCol;
                }
                else {
                    addCsvButton.FillCol = addCsvButton.InitCol;
                }

                // Display
                addManualButton.Display(gRenderer);
                addCsvButton.Display(gRenderer);

                if(Type == 1) {
                    int viewScoreState = viewScoreButton.isMouseClick(&event);
                    if(viewScoreState == 1) {
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

                        viewScoreButton.FillCol = viewScoreButton.PressCol;

                        Student* allStudent = nullptr;
                        loadAllStudentData(allStudent, studentFileName);

                        Course* allCourse = nullptr;
                        loadAllCourseData(allCourse, courseFileName, allStudent);

                        Course* curCourse = findCourseByID(allCourse, courseID);

                        staffViewCourseScore(curCourse);
                        staffChooseOptionWindow(1, courseID);
                        return;
                    }
                    else if(viewScoreState == 2) {
                        viewScoreButton.FillCol = viewScoreButton.HoverCol;
                    }
                    else {
                        viewScoreButton.FillCol = viewScoreButton.InitCol;
                    }

                    viewScoreButton.Display(gRenderer);
                    courseIDButton.Display(gRenderer);
                }
                backButton.Display(gRenderer);

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

void staffChooseScoreOption() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 270;
    const int buttonHeight = 140;
    const int startX = 45;
    const int startY = 150;
    const int plusX = 90;
    const int plusY = 60;
    const int buttonTextSize = 27;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "View Score")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button viewCourseScore, viewClassScore;

        viewCourseScore = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Manage Courses Score", 25);

        viewClassScore = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "View Score In Classes", 25);

        Button findStudentButton = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, PURPLE, RED, RED, "Find Student Score", 25);

        findStudentButton.textColor = WHITE;
        viewCourseScore.textColor = WHITE;
        viewClassScore.textColor = WHITE;

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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

                int viewCourseScoreState = viewCourseScore.isMouseClick(&event);
                if(viewCourseScoreState == 1) {
                    viewCourseScore.FillCol = viewCourseScore.PressCol;
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

                    staffViewAllCourse();
                    return;
                }
                else if(viewCourseScoreState == 2) {
                    viewCourseScore.FillCol = viewCourseScore.HoverCol;
                }
                else {
                    viewCourseScore.FillCol = viewCourseScore.InitCol;
                }

                int viewClassScoreState = viewClassScore.isMouseClick(&event);
                if(viewClassScoreState == 1) {
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

                    viewClassScore.FillCol = viewClassScore.PressCol;
                    staffViewListClassWindow();
                    staffChooseScoreOption();
                    return;
                }
                else if(viewClassScoreState == 2) {
                    viewClassScore.FillCol = viewClassScore.HoverCol;
                }
                else {
                    viewClassScore.FillCol = viewClassScore.InitCol;
                }

                int findStudentState = findStudentButton.isMouseClick(&event);
                if(findStudentState == 1) {
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

                    findStudentButton.FillCol = findStudentButton.PressCol;
                    staffFindStudentWindow(1);
                    staffChooseScoreOption();
                    return;
                }
                else if(findStudentState == 2) {
                    findStudentButton.FillCol = findStudentButton.HoverCol;
                }
                else {
                    findStudentButton.FillCol = findStudentButton.InitCol;
                }

                // Display
                viewCourseScore.Display(gRenderer);
                viewClassScore.Display(gRenderer);
                findStudentButton.Display(gRenderer);

                backButton.Display(gRenderer);

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

void staffFindStudentWindow(int Type) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int buttonWidth = 400;
    const int buttonHeight = 30;
    const int startX = (SCREEN_WIDTH - buttonWidth) / 2;
    const int startY = 250;
    const int plusY = 60;
    const int buttonTextSize = 27;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "Find Student")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button findStudentButton = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, WHITE, LIGHTBLUE, GREY, "", 20);

        Button findButton = Button((SCREEN_WIDTH - 100) / 2, startY + 80, 100, 30, 2, BLACK, LIGHTBLUE, RED, GREY, "Find", 20);

        TextOutput warningText = TextOutput(RED, 25);
        warningText.loadText(gRenderer, "We do not find this student", FONTDIR);

        TextOutput findStudentText = TextOutput(BLACK, 20);
        findStudentText.loadText(gRenderer, "Student ID", FONTDIR);

        bool isFind = false;

        bool quit = false;
        int isLoad = 0;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    exit(0);
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

                int findButtonState = findButton.isMouseClick(&event);
                if(findButtonState == 1) {
                    findButton.FillCol = findButton.PressCol;
                    isFind = true;

                    Student* allStudent = nullptr;
                    loadAllStudentData(allStudent, studentFileName);

                    Student* curStudent = findStudentByID(allStudent, findStudentButton.Text);

                    if(curStudent) {
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

                        if(Type == 0) {
                            studentEditProfileWindow(curStudent);
                            staffFindStudentWindow(Type);
                        }
                        else {
                            studentViewScore(curStudent);
                            staffFindStudentWindow(Type);
                        }
                        return;
                    }
                }
                else if(findButtonState == 2) {
                    findButton.FillCol = findButton.HoverCol;
                }
                else {
                    findButton.FillCol = findButton.InitCol;
                }

                if(isFind) {
                    warningText.Display(gRenderer, (SCREEN_WIDTH - warningText.mWidth) / 2, startY + 120);
                }

                backButton.Display(gRenderer);
                findStudentButton.Display(gRenderer);
                findButton.Display(gRenderer);
                findStudentText.Display(gRenderer, startX, startY - 25);

                findStudentButton.isTextBox(gRenderer, &event);

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
