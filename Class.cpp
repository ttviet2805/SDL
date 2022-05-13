#include "Class.h"

#define classFileName = "Data/AllClass.txt"
#define studentFileName "Data/AllStudent.txt"

void Class::loadStudentInClass() {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Student* curStudent = allStudent;

    StudentInClass* Dummy = new StudentInClass;
    StudentInClass* curStudentInClass = Dummy;

    while(curStudent) {
        if(curStudent->Info->Class == className) {
            curStudentInClass->Next = new StudentInClass;
            curStudentInClass->Next->StudentID = curStudent->Info->ID;
            curStudentInClass = curStudentInClass->Next;
        }

        curStudent = curStudent->Next;
    }

    studentHead = Dummy->Next;
    StudentInClass* Del = Dummy;
    Dummy = Dummy->Next;

    delete Del;
}

void loadAllClassData(Class* &allClass, string fileName) {
    ifstream fin;
    fin.open(fileName);

    if(!fin) {
        cout << "Can not open class file";
        system("pause");
        return;
    }

    Class* Dummy = new Class;
    Class* curClass = Dummy;

    string tmpClassName;

    while(!fin.eof() && getline(fin, tmpClassName)) {
        curClass->Next = new Class;
        curClass->Next->className = tmpClassName;

        curClass->Next->loadStudentInClass();

        curClass = curClass->Next;
    }

    allClass = Dummy->Next;
    Class* Del = Dummy;
    Dummy = Dummy->Next;
    delete Del;

    fin.close();
}

void saveAllClassData(Class* allClass, string fileName) {
    ofstream fout;
    fout.open(fileName);

    if(!fout) {
        cout << "Can not open class file";
        system("pause");
        return;
    }

    Class* curClass = allClass;

    while(curClass) {
        fout << curClass->className << '\n';
        curClass = curClass->Next;
    }

    fout.close();
}

void showAllClassData(Class* allClass) {
    Class* curClass = allClass;

    while(curClass) {
        cout << curClass->className << '\n';
        StudentInClass* tmp = curClass->studentHead;

        while(tmp) {
            cout << tmp->StudentID << endl;
            tmp = tmp->Next;
        }

        curClass = curClass->Next;
    }
}

Class* findClassByID(Class* allClass, string ID) {
    Class* curClass = allClass;

    while(curClass) {
        if(curClass->className == ID) return curClass;
        curClass = curClass->Next;
    }

    return nullptr;
}
