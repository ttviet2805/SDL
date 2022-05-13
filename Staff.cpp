#include "Staff.h"

#define accountFileName "Data/AllAccount.txt"
#define studentFileName "Data/AllStudent.txt"
#define courseFileName "Data/AllCourse.txt"
#define courseScoreFileName "Data/CourseScore/"
#define classFileName "Data/AllClass.txt"
#define studentScoreFileName "Data/StudentScore/"
#define staffFileName "Data/AllStaff.txt"

void loadAllStaffData(Staff* & allStaff, string fileName) {
    ifstream fin;
    fin.open(fileName);

    Staff* Dummy = new Staff;
    Staff* curStaff = Dummy;

    string tmpID;
    while(!fin.eof() && getline(fin, tmpID)) {
        curStaff->Next = new Staff;
        curStaff->Next->Info->ID = tmpID;
        getline(fin, curStaff->Next->Info->fullName);

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
        fout << curStaff->Info->ID << '\n' << curStaff->Info->fullName << '\n';
        curStaff = curStaff->Next;
    }

    fout.close();
}

void showAllStaffData(Staff* allStaff) {
    Staff* curStaff = allStaff;

    while(curStaff) {
        cout << curStaff->Info->ID << ' ' << curStaff->Info->fullName << '\n';
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
        StudentInfo* Info = curStudent->Info;

        fout << Info->ID << '\n' << Info->firstName << '\n' << Info->lastName << '\n' << Info->Gender << '\n';
        fout << Info->Dob << '\n' << Info->SocialID << '\n' << Info->Class << '\n';
        fout << Info->schoolyear << '\n';

        curStudentInClass = curStudentInClass->Next;
    }

    fout.close();
}

void viewStudentInASameYear(string schoolYear) {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Student* curStudent = allStudent;

    while(curStudent) {
        if(curStudent->Info->schoolyear == schoolYear) {
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
        if(curStudent->Info->schoolyear == schoolYear) {
            StudentInfo* Info = curStudent->Info;

            fout << Info->ID << '\n' << Info->firstName << '\n' << Info->lastName << '\n' << Info->Gender << '\n';
            fout << Info->Dob << '\n' << Info->SocialID << '\n' << Info->Class << '\n';
            fout << Info->schoolyear << '\n';
        }

        curStudent = curStudent->Next;
    }

    fout.close();
}



// Front End

void staffaddStudentByManual() {
    Student* newStudent = new Student;
    cout << "ID: ";
    getline(cin, newStudent->Info->ID);
    cout << "First name: ";
    getline(cin, newStudent->Info->firstName);
    cout << "Last name: ";
    getline(cin, newStudent->Info->lastName);
    cout << "Gender: ";
    getline(cin, newStudent->Info->Gender);
    cout << "Date of Birth: ";
    getline(cin, newStudent->Info->Dob);
    cout << "Social ID: ";
    getline(cin, newStudent->Info->SocialID);
    cout << "Class: ";
    getline(cin, newStudent->Info->Class);
    cout << "School year: ";
    getline(cin, newStudent->Info->schoolyear);

    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    addANewStudent(allStudent, newStudent);

    saveAllStudentData(allStudent, studentFileName);

    Account* newAccount = new Account;
    newAccount->username = newStudent->Info->ID;
    newAccount->type = 1;
    addANewAccount(newAccount);
}

void staffaddStudentByCSV(string fileName) {
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
        newStudent->Info->ID = ID;
        getline(fin, newStudent->Info->firstName, ',');
        getline(fin, newStudent->Info->lastName, ',');
        getline(fin, newStudent->Info->Gender, ',');
        getline(fin, newStudent->Info->Dob, ',');
        getline(fin, newStudent->Info->SocialID, ',');
        getline(fin, newStudent->Info->Class, ',');
        getline(fin, newStudent->Info->schoolyear);

        addANewStudent(allStudent, newStudent);

        newStudent->viewProfile();

        Account* newAccount = new Account;
        newAccount->username = newStudent->Info->ID;
        newAccount->type = 1;
        addANewAccount(newAccount);
    }

    saveAllStudentData(allStudent, studentFileName);

    showAllStudentInfo(allStudent);

    fin.close();
}
