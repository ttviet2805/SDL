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
        curStudent->Next->Info->ID = stuID;
        getline(fin, curStudent->Next->Info->firstName);
        getline(fin, curStudent->Next->Info->lastName);
        getline(fin, curStudent->Next->Info->Gender);
        getline(fin, curStudent->Next->Info->Dob);
        getline(fin, curStudent->Next->Info->SocialID);
        getline(fin, curStudent->Next->Info->Class);
        getline(fin, curStudent->Next->Info->schoolyear);

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
        fout << curStudent->Info->ID << '\n' << curStudent->Info->firstName << '\n' << curStudent->Info->lastName << '\n';
        fout << curStudent->Info->Gender << '\n' << curStudent->Info->Dob << '\n';
        fout << curStudent->Info->SocialID << '\n' << curStudent->Info->Class << '\n';
        fout << curStudent->Info->schoolyear << '\n';
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
        cout << tmpStudent->Info->ID << '\n';
        cout << tmpStudent->Info->firstName << '\n' << tmpStudent->Info->lastName << '\n';
        cout << tmpStudent->Info->Gender << '\n' << tmpStudent->Info->Dob << '\n';
        cout << tmpStudent->Info->SocialID << '\n' << tmpStudent->Info->Class << '\n';
        cout << tmpStudent->Info->schoolyear << '\n';
        cout << '\n';
        tmpStudent = tmpStudent->Next;
    }
}

Student* findStudentByID(Student* allStudent, string ID) {
    Student* curStudent = allStudent;

    while(curStudent) {
        if(curStudent->Info->ID == ID) return curStudent;
        curStudent = curStudent->Next;
    }

    return nullptr;
}

void Student::viewProfile() {
    cout << Info->ID << '\n' << Info->firstName << '\n' << Info->lastName << '\n' << Info->Gender << '\n';
    cout << Info->Dob << '\n' << Info->SocialID << '\n' << Info->Class << '\n';
    cout << Info->schoolyear << '\n';
}

void Student::viewScoreBoard() {
    StudentScore* curScore = Score;

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    while(curScore) {
        Course* curCourse = findCourseByID(allCourse, curScore->courseID);
        cout << curCourse->Info->courseID << ' ' << curCourse->Info->courseName << '\n';
        cout << curScore->studentScore->MidTerm << ' ' << curScore->studentScore->Final << ' ' << curScore->studentScore->Other << '\n';

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
        fout << curCourse->Info->courseID << ' ' << curCourse->Info->courseName << '\n';
        fout << curScore->studentScore->MidTerm << ' ' << curScore->studentScore->Final << ' ' << curScore->studentScore->Other << '\n';

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

    Class* curClass = findClassByID(allClass, Info->Class);

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
    newScore->studentScore->setScore(MidTerm, Final, Other);

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

    Student* curStudent = findStudentByID(allStudent, tmpStudent->Info->ID);

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
                curText = curStudent->Info->ID;
                break;

            case 1:
                curText = curStudent->Info->firstName;
                break;

            case 2:
                curText = curStudent->Info->lastName;
                break;

            case 3:
                curText = curStudent->Info->Gender;
                break;

            case 4:
                curText = curStudent->Info->Dob;
                break;

            case 5:
                curText = curStudent->Info->SocialID;
                break;

            case 6:
                curText = curStudent->Info->Class;
                break;

            case 7:
                curText = curStudent->Info->schoolyear;
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

                for(int i = 0; i < 8; i++)
                    listTextbox[i].isTextBox(gRenderer, &event);

                int saveButtonState = saveButton.isMouseClick(&event);
                if(saveButtonState == 1) {
                    curStudent->Info->ID = listTextbox[0].Text;
                    curStudent->Info->firstName = listTextbox[1].Text;
                    curStudent->Info->lastName = listTextbox[2].Text;
                    curStudent->Info->Gender = listTextbox[3].Text;
                    curStudent->Info->Dob = listTextbox[4].Text;
                    curStudent->Info->SocialID = listTextbox[5].Text;
                    curStudent->Info->Class = listTextbox[6].Text;
                    curStudent->Info->schoolyear = listTextbox[7].Text;

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
        string studentName = curStudent->Info->firstName + " " + curStudent->Info->lastName;
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
                    if(buttonState == 1) {
                        listButton[i].FillCol = listButton[i].PressCol;
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
