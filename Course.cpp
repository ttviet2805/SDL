#include "Course.h"

void Course::loadCourseScoreData(string fileName, Student* &allStudent) {
    ifstream fin;
    fin.open(fileName);

    CourseScore* Dummy = new CourseScore;
    CourseScore* cur = Dummy;

    string tmpstudentID;
    while(!fin.eof() && getline(fin, tmpstudentID)) {
        cur->Next = new CourseScore;
        cur->Next->StudentID = tmpstudentID;
        fin >> cur->Next->studentScore.MidTerm >> cur->Next->studentScore.Final >> cur->Next->studentScore.Other;
        fin.get();

        Student* curStudent = findStudentByID(allStudent, tmpstudentID);

        if(curStudent) {
            StudentScore* newScore = new StudentScore;
            newScore->courseID = Info.courseID;
            newScore->studentScore.setScore(cur->Next->studentScore.MidTerm, cur->Next->studentScore.Final, cur->Next->studentScore.Other);
            curStudent->addStudentScore(newScore);
        }

        cur = cur->Next;
    }

    courseScoreHead = Dummy->Next;
    CourseScore* Del = Dummy;
    Dummy = Dummy->Next;
    delete Del;

    fin.close();
}

void Course::saveCourseScoreData(string fileName) {
    ofstream fout;
    fout.open(fileName);

    CourseScore* curScore = courseScoreHead;

    while(curScore) {
        fout << curScore->StudentID << '\n';
        fout << curScore->studentScore.MidTerm << ' ' << curScore->studentScore.Final << ' ' << curScore->studentScore.Other << '\n';
        curScore = curScore->Next;
    }

    fout.close();
}

void loadAllCourseData(Course* &allCourse, string fileName, Student* &allStudent) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not open course file";
        system("pause");
        return;
    }

    Course* Dummy = new Course;
    Course* curCourse = Dummy;

    string tmpID;
    while(!fin.eof() && getline(fin, tmpID)) {
        curCourse->Next = new Course;
        curCourse->Next->Info.courseID = tmpID;
        getline(fin, curCourse->Next->Info.courseName);

        curCourse->Next->loadCourseScoreData(courseScoreFileName + curCourse->Next->Info.courseID + ".txt", allStudent);
        curCourse = curCourse->Next;
    }

    allCourse = Dummy->Next;
    Course* Del = Dummy;
    Dummy = Dummy->Next;
    delete Del;

    fin.close();
}

void saveAllCourseData(Course* allCourse, string fileName) {
    ofstream fout;
    fout.open(fileName);

    if(!fout) {
        cout << "Can not open course file";
        system("pause");
        return;
    }

    Course* curCourse = allCourse;

    while(curCourse) {
        fout << curCourse->Info.courseID << '\n' << curCourse->Info.courseName << '\n';
        curCourse->saveCourseScoreData(courseScoreFileName + curCourse->Info.courseID + ".txt");
        curCourse = curCourse->Next;
    }

    fout.close();
}

void showAllCourseData(Course* allCourse) {
    Course* curCourse = allCourse;

    while(curCourse) {
        cout << curCourse->Info.courseID << '\n' << curCourse->Info.courseName << '\n';
        curCourse = curCourse->Next;
    }
}

Course* findCourseByID(Course* allCourse, string ID) {
    Course* curCourse = allCourse;

    while(curCourse) {
        if(curCourse->Info.courseID == ID) return curCourse;
        curCourse = curCourse->Next;
    }

    return nullptr;
}

void Course::addCourseScore(CourseScore* newScore) {
    if(!courseScoreHead) {
        courseScoreHead = newScore;
        return;
    }

    if(courseScoreHead->StudentID == newScore->StudentID) {
        courseScoreHead->studentScore = newScore->studentScore;
        return;
    }

    CourseScore* curScore = courseScoreHead;

    while(curScore->Next) {
        if(curScore->StudentID == newScore->StudentID) {
            curScore->studentScore = newScore->studentScore;
            return;
        }

        curScore = curScore->Next;
    }

    if(curScore->StudentID == newScore->StudentID) {
        curScore->studentScore = newScore->studentScore;
        return;
    }

    curScore->Next = newScore;
}

void Course::viewStudentScore() {
    CourseScore* curScore = courseScoreHead;

    while(curScore) {
        cout << curScore->StudentID << '\n';
        cout << curScore->studentScore.MidTerm << ' ' << curScore->studentScore.Final << ' ' << curScore->studentScore.Other << '\n';

        curScore = curScore->Next;
    }
}

void Course::exportCourseScoreToCSVFile(string fileName) {
    ofstream fout;
    fout.open(fileName);

    CourseScore* curScore = courseScoreHead;

    while(curScore) {
        fout << curScore->StudentID << ',';
        fout << curScore->studentScore.MidTerm << ',' << curScore->studentScore.Final << ',' << curScore->studentScore.Other << '\n';

        curScore = curScore->Next;
    }

    fout.close();
}

CourseScore* createACourseScore(string studentID, float Midterm, float Final, float Other) {
    CourseScore* newScore = new CourseScore;

    newScore->StudentID = studentID;
    newScore->studentScore.setScore(Midterm, Final, Other);

    return newScore;
}

bool addCourseScoreByCSV(string courseID, string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        fin.close();
        return false;
    }

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    Course* curCourse = findCourseByID(allCourse, courseID);

    if(!curCourse) {
        fin.close();
        return false;
    }

    string stuID;
    while(!fin.eof() && getline(fin, stuID, ',')) {
        CourseScore* newScore = new CourseScore;

        newScore->StudentID = stuID;
        fin >>  newScore->studentScore.MidTerm;
        fin.get();
        fin >>  newScore->studentScore.Final;
        fin.get();
        fin >> newScore->studentScore.Other;
        fin.get();

        curCourse->addCourseScore(newScore);
    }

    CourseScore* ttt = curCourse->courseScoreHead;
    while(ttt) {
        cout << ttt->StudentID << endl;
        ttt = ttt->Next;
    }

    saveAllCourseData(allCourse, courseFileName);

    fin.close();
}
