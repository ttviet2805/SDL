#include "Student.h"

void loadAllStudentData(Student* &allStudent, string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not open student file";
        system("pause");
        return;
    }

    Student* Dummy = new Student;
    Student* curStudent = Dummy;

    string stuID;
    while(!fin.eof() && getline(fin, stuID)) {
        curStudent->Next = new Student;
        curStudent->Next->Info.ID = stuID;
        getline(fin, curStudent->Next->Info.firstName);
        getline(fin, curStudent->Next->Info.lastName);
        getline(fin, curStudent->Next->Info.Gender);
        getline(fin, curStudent->Next->Info.Dob);
        getline(fin, curStudent->Next->Info.SocialID);
        getline(fin, curStudent->Next->Info.Class);
        getline(fin, curStudent->Next->Info.schoolyear);

        curStudent = curStudent->Next;
    }

    allStudent = Dummy->Next;
    Student* Del = Dummy;
    Dummy = Dummy->Next;
    delete Del;

    fin.close();
}

void saveAllStudentData(Student* allStudent, string filename) {
    ofstream fout;
    fout.open(filename);

    if(!fout) {
        cout << "Can not open student file";
        system("pause");
        return;
    }

    Student* curStudent = allStudent;

    while(curStudent) {
        fout << curStudent->Info.ID << '\n' << curStudent->Info.firstName << '\n' << curStudent->Info.lastName << '\n';
        fout << curStudent->Info.Gender << '\n' << curStudent->Info.Dob << '\n';
        fout << curStudent->Info.SocialID << '\n' << curStudent->Info.Class << '\n';
        fout << curStudent->Info.schoolyear << '\n';
        curStudent = curStudent->Next;
    }

    fout.close();
}

void Student::addStudentScore(StudentScore* newScore) {
    if(!Score) {
        Score = newScore;
        return;
    }

    if(Score->courseID == newScore->courseID) {
        Score->studentScore = newScore->studentScore;
        return;
    }

    StudentScore* curScore = Score;

    while(curScore->Next) {
        if(curScore->courseID == newScore->courseID) {
            curScore->studentScore = newScore->studentScore;
            return;
        }

        curScore = curScore->Next;
    }

    curScore->Next = newScore;
}

void showAllStudentInfo(Student* allStudent) {
    Student* tmpStudent = allStudent;

    while(tmpStudent) {
        cout << tmpStudent->Info.ID << '\n';
        cout << tmpStudent->Info.firstName << '\n' << tmpStudent->Info.lastName << '\n';
        cout << tmpStudent->Info.Gender << '\n' << tmpStudent->Info.Dob << '\n';
        cout << tmpStudent->Info.SocialID << '\n' << tmpStudent->Info.Class << '\n';
        cout << tmpStudent->Info.schoolyear << '\n';
        cout << '\n';
        tmpStudent = tmpStudent->Next;
    }
}

Student* findStudentByID(Student* allStudent, string ID) {
    Student* curStudent = allStudent;

    while(curStudent) {
        if(curStudent->Info.ID == ID) return curStudent;
        curStudent = curStudent->Next;
    }

    return nullptr;
}

void Student::viewProfile() {
    cout << Info.ID << '\n' << Info.firstName << '\n' << Info.lastName << '\n' << Info.Gender << '\n';
    cout << Info.Dob << '\n' << Info.SocialID << '\n' << Info.Class << '\n';
    cout << Info.schoolyear << '\n';
}

void Student::viewScoreBoard() {
    StudentScore* curScore = Score;

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    while(curScore) {
        Course* curCourse = findCourseByID(allCourse, curScore->courseID);
        cout << curCourse->Info.courseID << ' ' << curCourse->Info.courseName << '\n';
        cout << curScore->studentScore.MidTerm << ' ' << curScore->studentScore.Final << ' ' << curScore->studentScore.Other << '\n';

        curScore = curScore->Next;
    }

    cout << '\n';
}

void Student::exportScoreBoard(string fileName) {
    ofstream fout;
    fout.open(fileName);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    StudentScore* curScore = Score;

    while(curScore) {
        Course* curCourse = findCourseByID(allCourse, curScore->courseID);
        fout << curCourse->Info.courseID << ' ' << curCourse->Info.courseName << '\n';
        fout << curScore->studentScore.MidTerm << ' ' << curScore->studentScore.Final << ' ' << curScore->studentScore.Other << '\n';

        curScore = curScore->Next;
    }

    fout << '\n';

    fout.close();
}

void Student::viewStudentInCurrentClass() {
    Class* allClass = nullptr;
    loadAllClassData(allClass, classFileName);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Class* curClass = findClassByID(allClass, Info.Class);

    StudentInClass* curStudentInClass = curClass->studentHead;

    while(curStudentInClass) {
        Student* curStudent = findStudentByID(allStudent, curStudentInClass->StudentID);

        if(curStudent) {
            curStudent->viewProfile();
            cout << '\n';
        }

        curStudentInClass = curStudentInClass->Next;
    }
}

StudentScore* createAStudentScore(string CourseID, float MidTerm, float Final, float Other) {
    StudentScore* newScore = new StudentScore;

    newScore->courseID = CourseID;
    newScore->studentScore.setScore(MidTerm, Final, Other);

    return newScore;
}

// --------------------------------------------------------------------------- //

void studentEditProfileWindow(Student* tmpStudent) {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int textboxWidth = 300;
    const int textboxHeight = 40;
    const int startX = 120;
    const int startY = 150;
    const int plusX = 240;
    const int plusY = 55;

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Student* curStudent = findStudentByID(allStudent, tmpStudent->Info.ID);

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

        vector <Button> listTextbox(8);

        for(int i = 0; i < 8; i++) {
            string curText = "";

            switch (i) {
            case 0:
                curText = curStudent->Info.ID;
                break;

            case 1:
                curText = curStudent->Info.firstName;
                break;

            case 2:
                curText = curStudent->Info.lastName;
                break;

            case 3:
                curText = curStudent->Info.Gender;
                break;

            case 4:
                curText = curStudent->Info.Dob;
                break;

            case 5:
                curText = curStudent->Info.SocialID;
                break;

            case 6:
                curText = curStudent->Info.Class;
                break;

            case 7:
                curText = curStudent->Info.schoolyear;
                break;
            }

            if(i < 4)
                listTextbox[i] = Button(startX, startY + (textboxHeight + plusY) * i, textboxWidth, textboxHeight, 2, BLACK, WHITE, WHITE, GREY, curText, 20);
            else
                listTextbox[i] = Button(startX + (textboxWidth + plusX), startY + (textboxHeight + plusY) * (i - 4), textboxWidth, textboxHeight, 2, BLACK, WHITE, WHITE, GREY, curText, 20);
        }

        vector <TextOutput> listText;

        for(int i = 0; i < 8; i++) {
            string curText = "";

            switch (i) {
            case 0:
                curText = "ID";
                break;

            case 1:
                curText = "First name";
                break;

            case 2:
                curText = "Last name";
                break;

            case 3:
                curText = "Gender";
                break;

            case 4:
                curText = "Date of birth";
                break;

            case 5:
                curText = "Social ID";
                break;

            case 6:
                curText = "Class";
                break;

            case 7:
                curText = "School year";
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

                for(int i = 0; i < 8; i++)
                    listTextbox[i].Display(gRenderer);

                for(int i = 0; i < 4; i++)
                    listText[i].Display(gRenderer, startX, startY + (textboxHeight + plusY) * i - 25);

                for(int i = 4; i < 8; i++)
                    listText[i].Display(gRenderer, startX + (textboxWidth + plusX), startY + (textboxHeight + plusY) * (i - 4) - 25);

                saveButton.Display(gRenderer);
                backButton.Display(gRenderer);

                for(int i = 1; i < 8; i++) {
                    if(listTextbox[i].isTextBox(gRenderer, &event)) {
                        event.button.button = SDL_BUTTON_RIGHT;
                    }
                }

                int saveButtonState = saveButton.isMouseClick(&event);
                if(saveButtonState == 1) {
                    curStudent->Info.ID = listTextbox[0].Text;
                    curStudent->Info.firstName = listTextbox[1].Text;
                    curStudent->Info.lastName = listTextbox[2].Text;
                    curStudent->Info.Gender = listTextbox[3].Text;
                    curStudent->Info.Dob = listTextbox[4].Text;
                    curStudent->Info.SocialID = listTextbox[5].Text;
                    curStudent->Info.Class = listTextbox[6].Text;
                    curStudent->Info.schoolyear = listTextbox[7].Text;

                    saveButton.FillCol = saveButton.PressCol;
                    saveAllStudentData(allStudent, studentFileName);
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
                    studentWindow(curStudent);
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

void studentViewScore(Student* tmpStudent) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    Student* curStudent = findStudentByID(allStudent, tmpStudent->Info.ID);

    StudentScore* curScore = curStudent->Score;

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "View Scoreboard")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button exportFileButton = Button(SCREEN_WIDTH - 350, 50, 300, 30, 2, BLACK, GREY, RED, LIGHTBLUE, "Export to CSV file", 20);

        TextOutput exportOKButton = TextOutput(RED, 22);
        exportOKButton.loadText(gRenderer, "You have export your score to file", FONTDIR);

        vector <Button> listButton[10];

        int curX = startX, curY = startY;

        for(int i = 0; i < 7; i++) {
            int Width = 0, Height = 25;
            string curText = "";

            switch (i) {
                case 0: {
                    Width = 200;
                    curText = "Course ID";
                    break;
                }

                case 1: {
                    Width = 300;
                    curText = "Course name";
                    curX += 200;
                    break;
                }

                case 2: {
                    Width = 100;
                    curText = "Midterm";
                    curX += 300;
                    break;
                }

                case 3: {
                    Width = 100;
                    curText = "Final";
                    curX += 100;
                    break;
                }

                case 4: {
                    Width = 100;
                    curText = "Other";
                    curX += 100;
                    break;
                }

                case 5: {
                    Width = 100;
                    curText = "Total";
                    curX += 100;
                    break;
                }

                case 6: {
                    Width = 100;
                    curText = "GPA";
                    curX += 100;
                    break;
                }
            }

            Button tmp = Button(curX, startY, Width, Height, 2, BLACK, WHITE, RED, RED, curText, 15);

            listButton[i].push_back(tmp);
        }

        curY = startY;

        while(curScore) {
            Course* curCourse = findCourseByID(allCourse, curScore->courseID);

            if(curCourse) {
                curX = startX;
                curY += 25;

                stringstream ss;

                for(int i = 0; i < 7; i++) {
                    int Width = 0, Height = 25;
                    string curText = "";

                    switch (i) {
                        case 0: {
                            Width = 200;
                            curText = curCourse->Info.courseID;
                            break;
                        }

                        case 1: {
                            Width = 300;
                            curText = curCourse->Info.courseName;
                            curX += 200;
                            break;
                        }

                        case 2: {
                            Width = 100;
                            ss << fixed << setprecision(2) << curScore->studentScore.MidTerm << ' ';
                            ss >> curText;
                            curX += 300;
                            break;
                        }

                        case 3: {
                            Width = 100;
                            ss << fixed << setprecision(2) << curScore->studentScore.Final << ' ';
                            ss >> curText;
                            curX += 100;
                            break;
                        }

                        case 4: {
                            Width = 100;
                            ss << fixed << setprecision(2) << curScore->studentScore.Other << ' ';
                            ss >> curText;
                            curX += 100;
                            break;
                        }

                        case 5: {
                            Width = 100;
                            curScore->studentScore.calTotal();
                            ss << fixed << setprecision(2) << curScore->studentScore.Total << ' ';
                            ss >> curText;
                            curX += 100;
                            break;
                        }

                        case 6: {
                            Width = 100;
                            curX += 100;
                            break;
                        }
                    }

                    Button tmp = Button(curX, curY, Width, 25, 2, BLACK, WHITE, RED, RED, curText, 15);

                    listButton[i].push_back(tmp);
                }
            }

            curScore = curScore->Next;
        }

        bool isExport = false;

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                for(int i = 0; i < 7; i++) {
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
                    studentWindow(curStudent);
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
                    curStudent->exportScoreBoard("Data/StudentScore/" + curStudent->Info.ID + ".txt");
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

                backButton.Display(gRenderer);
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

void studentViewStudentInClass(Student* curStudent, int page) {
    Class* allClass = nullptr;
    loadAllClassData(allClass, classFileName);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Class* curClass = findClassByID(allClass, curStudent->Info.Class);

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    const int startX = 40, startY = 100;
    const int numStudentInAPage = 15;

    const string backgroundPath = "Data/Image/StudentBackground.jpg";

    if(!init(gWindow, gRenderer, "List Students In Class")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        Button backButton = Button(20, 20, 80, 30, 2, BLACK, RED, LIGHTBLUE, GREY, "Back", 20);

        Button curClassButton = Button((SCREEN_WIDTH - 150) / 2 , 60, 150, 30, 2, BLACK, WHITE, WHITE, WHITE, "Class: " + curClass->className, 20);

        string preText = "<";
        Button preButton = Button(SCREEN_WIDTH / 2 - 40 - 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, preText, 20);

        string nextText = ">";
        Button nextButton = Button(SCREEN_WIDTH / 2 + 15, 580, 40, 40, 2, GREEN, GREEN, RED, LIGHTBLUE, nextText, 20);

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

        StudentInClass* curStudentInClass = curClass->studentHead;

        int numStudentInClass = 0;
        StudentInClass* tmpStu = curStudentInClass;

        while(tmpStu) {
            numStudentInClass++;
            tmpStu = tmpStu->Next;
        }

        int numMaxPage = numStudentInClass / numStudentInAPage;
        if(numStudentInClass % numStudentInAPage != 0) numMaxPage++;

        curY = startY;

        int startStu = page * numStudentInAPage, cnt = 0;

        while(curStudentInClass && cnt < startStu) {
            ++cnt;
            curStudentInClass = curStudentInClass->Next;
        }

        cnt = 0;

        while(curStudentInClass && cnt < numStudentInAPage) {
            ++cnt;
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
                            Width = 230;
                            curText = curStudent->Info.firstName;
                            curX += 150;
                            break;
                        }

                        case 2: {
                            Width = 150;
                            curText = curStudent->Info.lastName;
                            curX += 230;
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
                            Width = 100;
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
                if(preButtonState == 1) {
                    if(page > 0) {
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

                        for(int i = 0; i < 8; i++) {
                            listButton[i].clear();
                        }

                        preButton.FillCol = preButton.PressCol;
                        studentViewStudentInClass(curStudent, page - 1);
                        return;
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
                    if(page < numMaxPage) {
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

                        for(int i = 0; i < 8; i++) {
                            listButton[i].clear();
                        }

                        nextButton.FillCol = nextButton.PressCol;
                        studentViewStudentInClass(curStudent, page + 1);
                        return;
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

void studentWindow(Student* curStudent) {
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

    if(!init(gWindow, gRenderer, "Student")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        bool quit = false;

        TextOutput welcomeText = TextOutput(RED, 22);
        string studentName = curStudent->Info.firstName + " " + curStudent->Info.lastName;
        welcomeText.loadText(gRenderer, "Welcome " + studentName,FONTDIR);

        SDL_Texture* backgroundImage = nullptr;
        loadImage(gRenderer, backgroundImage, backgroundPath);

        vector <Button> listButton(5);

        listButton[0] = Button(startX, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "PROFILE", buttonTextSize);

        listButton[1] = Button(startX + buttonWidth + plusX, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "VIEW SCORE", buttonTextSize);

        listButton[2] = Button(startX + (buttonWidth + plusX) * 2, startY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "VIEW STUDENT IN CLASS", buttonTextSize);

        listButton[3] = Button(startX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "CHANGE PASSWORD", buttonTextSize);

        listButton[4] = Button(startX + buttonWidth + plusX, startY + buttonHeight + plusY, buttonWidth, buttonHeight, 2, BLACK, LIGHTBLUE, RED, RED, "LOG OUT", buttonTextSize);

        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                SDL_RenderClear(gRenderer);

                SDL_RenderCopy(gRenderer, backgroundImage, NULL, NULL);

                welcomeText.Display(gRenderer, SCREEN_WIDTH - welcomeText.mWidth - 20, 10);


                for(int i = 0; i < 5; i++) {
                    listButton[i].Display(gRenderer);
                }

                for(int i = 0; i < 5; i++) {
                    int buttonState = listButton[i].isMouseClick(&event);
                    //listButton[i].FillCol = listButton[i].InitCol;
                    if(buttonState == 1) {
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
                        listButton[i].FillCol = listButton[i].PressCol;

                        switch (i) {
                            case 0: {
                                studentEditProfileWindow(curStudent);
                                return;
                                break;
                            }

                            case 1: {
                                studentViewScore(curStudent);
                                return;
                            }

                            case 2: {
                                studentViewStudentInClass(curStudent, 0);
                                return;
                            }

                            case 3: {
                                Account* allAccount = nullptr;
                                loadAllAccountData(allAccount, accountFileName);

                                Account* curAccount = findAccountByID(allAccount, curStudent->Info.ID);
                                if(!curAccount) {
                                    cout << "Change password problem";
                                    return;
                                }

                                curAccount->userChangePassword();
                                return;
                                break;
                            }

                            case 4: {
                                systemProcess();
                                return;
                                break;
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
        IMG_Quit();
        SDL_Quit();
    }
}
