#pragma once

#include <string>
#include <SDL.h>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 640;

const SDL_Color BLACK = {0, 0, 0, 255};
const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color RED = {255, 0, 0, 255};
const SDL_Color LIGHTBLUE = {0, 255, 255, 255};
const SDL_Color VIOLET = {143, 80, 157, 255};
const SDL_Color GREY = {216, 216, 216, 255};
const SDL_Color GREEN = {124, 252, 0, 255};
const SDL_Color DARKGREEN = {0, 150, 0, 255};
const SDL_Color PURPLE = {128, 0, 128, 255};

//Font constants
const string FONTDIR = "Data/Font/times-new-roman-14.ttf";

// File constants
const string accountFileName = "Data/AllAccount.txt";
const string studentFileName = "Data/AllStudent.txt";
const string courseFileName = "Data/AllCourse.txt";
const string courseScoreFileName = "Data/CourseScore/";
const string classFileName = "Data/AllClass.txt";
const string studentScoreFileName = "Data/StudentScore/";
const string staffFileName = "Data/AllStaff.txt";
const string studentInClassFileName = "Data/Class/";
const string studentCSVFileName = "Data/StudentCSV.csv";
const string exportClassFilename = "Data/Class/";
const string exportSchoolYearFilename = "Data/SchoolYear/";
