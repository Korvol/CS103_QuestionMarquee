#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <string> 
#include <windows.h>
#include <vector>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <utility>


using namespace std;

bool access = false; 
bool admin = false;
bool isStaff = false;
string user = ""; 
string day = "?";
string dayList[5] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY" };
void registerParent();
void registerStaff();
void parentLogin();
void staffLogin();
void forSleep(int sec);
void listFood();
void orderFood();
bool isint(string s);
void complain();
vector<pair<string, vector<string>>> read_csv(string filename);
int main()
{
    int inp; 
    srand(time(NULL));
    if (access) { // if you've signed in, you see this
        cout << "------AROHANUI LUNCH ORDERS------\nWhat would you like to do?\n1: List menu items\n2: Order food\n3: Submit complaint\n4: Log out" << endl;
        cin >> inp;
        switch (inp) {
        case 1:
            listFood();
            break;
        case 2:
            orderFood();
            break;

        case 3:
            complain();
            break;
        case 4:
            access = false;
            user = "";
            isStaff = false;
            main();
            break;
        default:
            main();
            break;
        }

    }
    else if (admin) { // if you have admin access, you see this
        cout << "-----------ADMIN PANEL-----------\nWhat would you like to do?\n1: Update Menu\n2: View daily order report\n3: View weekly sales report\n4: View weekly pending report\n5: View complaints\n6:Log out" << endl;
    }
    else { // otherwise you see a log in screen
        day = dayList[rand() % 5]; // This gives you a random day each time you enter the login screen
        cout << "------AROHANUI LUNCH ORDERS------\nWhat would you like to do?\n1: Login as a parent\n2: Login as a staff member\n3: Exit Program" << endl;
        cin >> inp;
        switch (inp) {
        case 1:
            parentLogin();
            break;
        case 2:
            staffLogin();
            break;
        case 3:
            break;
        default:
            main();
            break;
        }
    }
    return 0;
}
//function to read the csv file and return a vector of pairs with the vector containing the data
vector<pair<string, vector<string>>> read_csv(string filename) {
    ifstream file(filename);
    vector<pair<string, vector<string>>> result;
    string line;
    string cell;
    vector<string> values;
    while (getline(file, line)) {
        stringstream lineStream(line);
        while (getline(lineStream, cell, ',')) {
            values.push_back(cell);
        }
        result.push_back(make_pair(values.at(0), values));
        values.clear();
    }
    return result;
}
//this is how you would call the function later
// vector<pair<string, vector<string>>> result = read_csv("Customer_registration.csv");

bool isint(string s) {
    bool x = true; // 
    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i])) {
            x = false;
        }
    }
    return x;
}

void forSleep(int sec) {

    for (int i = 0; i < sec; i++) { // this function just does the . . . thing you see lots of programs do
        cout << ".";
        Sleep(1000);
    }
    cout << "\n";
}

void listFood() {
    vector<pair<string, vector<string>>> result = read_csv("menu.csv"); // loads the menu for the function
    cout << "On a food's special discount day, it is 20% off." << endl;
    for (int i = 0; i < result.size(); i++) { // goes through the csv file and shows all menu items
        cout << result.at(i).first << ": " << result.at(i).second.at(3) << "      Discount day: " << result.at(i).second.at(4) << "(" << result.at(i).second.at(5) << ")" << endl;
    }
    forSleep(3);
    main();
}

void orderFood() {
    string inp;
    int inpint;
    vector<pair<string, vector<string>>> menu = read_csv("menu.csv");
    vector<string> itemList = {};
    for (int i = 0; i < menu.size(); i++) {
        itemList.push_back(menu.at(i).first);
    }
    if (isStaff) {
        vector<pair<string, vector<string>>> userdata = read_csv("staffList.csv");
    }
    else {
        vector<pair<string, vector<string>>> userdata = read_csv("parentsList.csv");
    }
    vector<pair<string, vector<string>>> order = {};
    cout << "How many items will you be ordering?" << endl;
    cin >> inp;
    while (!isint(inp)) {
        cout << "Invalid input. Please input a number above 0" << endl;
        cin >> inp;
    }
    inpint = stoi(inp);
    main();
}
void complain() {
    string pCom;
    cout << "Enter your complaint here (any inappropriate messages will be forwarded to admin)" << endl;
    cin.ignore(1000, '\n');
    getline(cin, pCom);
    ofstream pFile;
    pFile.open("complaints.csv", ios::app);
    pFile << user << "," << pCom << endl;
    pFile.close();
    cout << "Your complaint has been noted, thank you. Returning to main menu." << endl;
    forSleep(3);
    main();
}
void parentLogin() {
    string pName, pKey, actUser = "a", actPass = "b", inp;
    bool escape = false;
    cout << "Do you have an account with us? 1 (yes) or 2 (no)." << endl;
    cin.ignore(1000, '\n');
    cin >> inp;
    if (inp == "1" || "2") {
        escape = true;
    }
    while (!escape) {
        cout << "Invalid input. Do you have an account with us? 1 (yes) or 2 (no)." << endl;
        cin.clear();
        cin >> inp;
        if (inp == "1" || "2") {
            escape = true;
        }
    }
    if (inp == "1") {
        cout << "What is your name?" << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        getline(cin, pName);
        cout << "What's your password?" << endl;
        cin.clear();
        getline(cin, pKey);
        vector<pair<string, vector<string>>> result = read_csv("parentsList.csv");
        for (int i = 0; i < result.size(); i++) {
            if (result.at(i).first == pName) {
                actUser = result.at(i).first;
                actPass = result.at(i).second.at(1);
            }

        }
        if (actUser == pName && actPass == pKey) {
            access = true;
            cout << "Login successful!" << endl;
            user = actUser; 
            isStaff = false;
            forSleep(3);
            main();
        }
    }
    else if (inp == "2") {
        registerParent();
    }
}
void staffLogin() {
    string pName, pKey, actUser = "a", actPass = "b";
    int inp;
    cout << "Do you have an account with us? 1 (yes) or 2 (no)" << endl;
    cin >> inp;
    if (inp == 1) {
        cout << "What is your name?" << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        getline(cin, pName);
        cout << "What's your password?" << endl;
        cin.clear();
        getline(cin, pKey);
        vector<pair<string, vector<string>>> result = read_csv("staffList.csv");
        for (int i = 0; i < result.size(); i++) {
            if (result.at(i).first == pName) {
                actUser = result.at(i).first;
                actPass = result.at(i).second.at(1);
            }

        }
        if (actUser == pName && actPass == pKey) {
            if (actUser == "admin") {
                admin = true;
            }
            else {
                access = true;
            }
            cout << "Login successful!" << endl;
            user = actUser;
            isStaff = true;
            forSleep(3);
            main();
        }
    }
    else if (inp == 2) {
        registerStaff();
    }
    else {
        staffLogin();
    }
}
void registerStaff() {
    string sName, pKey, gender, dob, contactNo = "02222222222", visa, visExp, sec;
    regex dobreg("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(19|20)[0-9]{2}");
    regex exp("(0[1-9]|1[0-2])/(2[5-9]|[3-9][0-9])");
    regex secex("[0-9]{3}");
    regex visn("[0-9]{16}");
    regex phn("0[0-9]{10}");
    string genderList[9] = { "m", "f", "nb", "gd", "male", "female", "nonbinary", "genderdiverse", "gender-diverse" };
    bool escape = false;
    cout << "What is your name (Input first and last)?" << endl;
    cin.clear();
    cin.ignore(1000, '\n');
    getline(cin, sName);
    cout << "Please input a password." << endl;
    cin.clear();
    cin >> pKey;
    cout << "What is your gender? Use one of the following: male (m), female (f), nonbinary(nb), gender-diverse(gd)" << endl;
    cin.clear();
    cin >> gender;
    for (auto& i : gender) {
        i = tolower(i);
    }
    for (int i = 0; i < 9; i++) {
        if (gender == genderList[i]) {
            escape = true;
        }
    }
    while (!escape) {
        cout << "Invalid gender. Please use one of the following: male (m), female (f), nonbinary(nb), gender-diverse(gd)." << endl;
        cin >> gender;
        for (auto& i : gender) {
            i = tolower(i);
        }
        for (int i = 0; i < 9; i++) {
            if (gender == genderList[i]) {
                escape = true;
            }
        }
    }
    escape = false;
    if (gender == "m") {
        gender = "male";
    }
    else if (gender == "f") {
        gender = "female";
    }
    else if (gender == "nb") {
        gender = "nonbinary";
    }
    else if (gender == "gd" || gender == "genderdiverse") {
        gender = "gender-diverse";
    }
    cout << "What is your Date of Birth. Please use the format dd/mm/yyyy." << endl;
    cin.clear();
    cin >> dob;
    if (regex_match(dob.begin(), dob.end(), dobreg))
        escape = true;
    while (!escape) {
        cout << "Invalid Date of Birth. Please use the format dd/mm/yyyy." << endl;
        cin >> dob;
        if (regex_match(dob.begin(), dob.end(), dobreg))
            escape = true;
    }
    escape = false;
    cout << "What is your phone number?" << endl;
    cin.clear();
    cin >> contactNo;
    if (regex_match(contactNo.begin(), contactNo.end(), phn))
        escape = true;
    while (!escape) {
        cout << "Invalid phone number. Phone numbers have 11 digits and start with a 0. Insert 0s at the end if your phone number is below 11 digits." << endl;
        cin >> contactNo;
        if (regex_match(contactNo.begin(), contactNo.end(), phn))
            escape = true;
    }
    escape = false;
    cout << "What is your Visa card number?" << endl;
    cin.clear();
    cin >> visa;
    if (regex_match(visa.begin(), visa.end(), visn))
        escape = true;
    while (!escape) {
        cout << "Invalid Visa number. Visa card numbers have 16 digits." << endl;
        cin >> visa;
        if (regex_match(visa.begin(), visa.end(), visn))
            escape = true;

    }
    escape = false;
    cout << "What is your Visa card's expiry date? Please use the format mm/yy" << endl;
    cin.clear();
    cin >> visExp;
    if (regex_match(visExp.begin(), visExp.end(), exp))
        escape = true;
    while (!escape) {
        cout << "Invalid expiry date. Please use the format mm/yy. Years before 2025 are invalid." << endl;
        cin >> visExp;
        if (regex_match(visExp.begin(), visExp.end(), exp))
            escape = true;
    }
    escape = false;
    cout << "What is your visa card's security number (The 3 numbers on the back)" << endl;
    cin >> sec;
    if (regex_match(sec.begin(), sec.end(), secex))
        escape = true;
    while (!escape) {
        cout << "Invalid security code. Please input a valid security code." << endl;
        cin >> sec;
        if (regex_match(sec.begin(), sec.end(), secex))
            escape = true;
    }
    escape = false;
    ofstream outputFile("staffList.csv", ios_base::app);
    if (!outputFile) {
        cerr << "Error opening file for writing." << endl;
    }
    outputFile << "\n" << sName << "," << pKey << ",false," << contactNo << "," << gender << "," << dob << "," << visa << "," << visExp;
    outputFile.close();
    forSleep(3);
    main();
}
void registerParent() {
    string pName, pKey, gender, dob, contactNo = "02222222222", visa, visExp, sec, cName;
    int cRoom;
    regex dobreg("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(19|20)[0-9]{2}");
    regex exp("(0[1-9]|1[0-2])/(2[5-9]|[3-9][0-9])");
    regex secex("[0-9]{3}");
    regex visn("[0-9]{16}");
    regex phn("0[0-9]{10}");
    string genderList[9] = { "m", "f", "nb", "gd", "male", "female", "nonbinary", "genderdiverse", "gender-diverse" };
    bool escape = false;
    cout << "What is your name (Input first and last)?" << endl;
    cin.clear();
    cin.ignore(1000, '\n');
    getline(cin, pName);
    cout << "Please input a password." << endl;
    cin >> pKey;
    cin.clear();
    cout << "What is your gender? Use one of the following: male (m), female (f), nonbinary(nb), gender-diverse(gd)" << endl; 
    cin >> gender;
    for (auto& i : gender) {
            i = tolower(i);
    }
    for (int i = 0; i < 9; i++) {
        if (gender == genderList[i]) {
            escape = true;
        }
    }
    while (!escape) {
        cout << "Invalid gender. Please use one of the following: male (m), female (f), nonbinary(nb), gender-diverse(gd)." << endl;
        cin >> gender;
        for (auto& i : gender) {
                i = tolower(i);
        }
        for (int i = 0; i < 9; i++) {
            if (gender == genderList[i]) {
                escape = true;
            }
        }
    }
    escape = false;
    if (gender == "m") {
        gender = "male";
    }
    else if (gender == "f") {
        gender = "female";
    }
    else if (gender == "nb") {
        gender = "nonbinary";
    }
    else if (gender == "gd" || gender == "genderdiverse") {
        gender = "gender-diverse";
    }
    cout << "What is your Date of Birth. Please use the format dd/mm/yyyy." << endl;
    cin >> dob;
    if ( regex_match(dob.begin(), dob.end(), dobreg) )
        escape = true;
    while (!escape) {
        cout << "Invalid Date of Birth. Please use the format dd/mm/yyyy." << endl;
        cin >> dob;
        if ( regex_match(dob.begin(), dob.end(), dobreg) )
            escape = true;
    }
    escape = false;
    escape = false;
    cout << "What is your phone number?" << endl; // What a creep!
    cin.clear();
    cin >> contactNo;
    if (regex_match(contactNo.begin(), contactNo.end(), phn))
        escape = true;
    while (!escape) {
        cout << "Invalid phone number. Phone numbers have 11 digits and start with a 0. Insert 0s at the end if your phone number is below 11 digits." << endl;
        cin >> contactNo;
        if (regex_match(contactNo.begin(), contactNo.end(), phn))
            escape = true;
    }
    escape = false;
    cout << "What is your Visa card number?" << endl;
    cin.clear();
    cin >> visa;
    if (regex_match(visa.begin(), visa.end(), visn))
        escape = true;
    while (!escape) {
        cout << "Invalid Visa number. Visa card numbers have 16 digits." << endl;
        cin >> visa;
        if (regex_match(visa.begin(), visa.end(), visn))
            escape = true;

    }
    escape = false;
    cout << "What is your Visa card's expiry date? Please use the format mm/yy" << endl;
    cin.clear();
    cin >> visExp;
    if (regex_match(visExp.begin(), visExp.end(), exp))
        escape = true;
    while (!escape) {
        cout << "Invalid expiry date. Please use the format mm/yy. Years before 2025 are invalid." << endl;
        cin >> visExp;
        if (regex_match(visExp.begin(), visExp.end(), exp))
            escape = true;
    }
    escape = false;
    cout << "What is your visa card's security number (The 3 numbers on the back)" << endl;
    cin >> sec;
    if (regex_match(sec.begin(), sec.end(), secex))
        escape = true;
    while (!escape) {
        cout << "Invalid security code. Please input a valid security code." << endl;
        cin >> sec;
        if (regex_match(sec.begin(), sec.end(), secex))
            escape = true;
    }
    escape = false;
    cout << "What's your child's name?" << endl;
    cin.clear();
    cin.ignore(1000, '\n');
    getline(cin, cName);
    cout << "What room is your child in?" << endl;
    cin >> cRoom;
    while (cin.fail() || cRoom > 20 || cRoom < 1) {
        cout << "Input a valid room number. There are 20 rooms labeled 1-20." << endl;
        cin >> cRoom;
    }
    ofstream outputFile("parentsList.csv", ios_base::app);
    if (!outputFile) {
        cerr << "Error opening file for writing." << endl;
    }// sends the registered parents details to the csv file
    outputFile << "\n" << pName << "," << pKey << "," << cName << "," << cRoom << "," << contactNo << "," << gender << "," << dob << "," << visa << "," << sec << "," << visExp;
    outputFile.close();
    forSleep(3);
    main();
}