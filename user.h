#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <windows.h>

using namespace std;


class User {
    string first_name, last_name;
    int key, age;
    string id, password, email, city, phone_number;

public:
    vector<bool> evaluatedCategory;

    User(int key_, string uname, string passw, string fname, string lname, int age_, string email_, string phone, string city_) {
        key = key_;
        id = uname;
        password = passw;
        first_name = fname;
        last_name = lname;
        age = age_;
        email = email_;
        phone_number = phone;
        city = city_;
    }
    User(){}

    void display_details()
    {
        cout << "--------------------------------------------" << endl;
        cout << "->PROFILE<-" << endl;
        cout << "First Name: " << first_name << endl;
        cout << "Last Name: " << last_name << endl;
        cout << "Age: " << age << endl;
        cout << "Email: " << email << endl;
        cout << "Phone Number: " << phone_number << endl;
        cout << "City: " << city << endl;
        cout << "--------------------------------------------" << endl; 
        //_getch();
    }
    void modifyDetails() {
        int option;

        do {
            system("cls");
            cout << "Modify:\n1.First Name\n2.Last Name\n3.Age\n4.Email\n5.Phone Number\n6.Education"
                << "\n7.Academic Details\n\nPress 8 to return to main menu" << endl;
            cin >> option;

            switch (option) {
            case 1:
                cout << "Set First Name: "; cin >> first_name; cout << "First Name Set to " << first_name << endl; Sleep(650);
                break;
            case 2:
                cout << "Set Last Name: "; cin >> last_name; cout << "Last Name Set to " << last_name << endl; Sleep(650);
                break;
            case 3:
                cout << "Set Age: "; cin >> age; cout << "Age Set to " << age << endl; Sleep(650);
                break;
            case 4:
                cout << "Set Email: "; cin >> email; cout << "Email set to " << email << endl; Sleep(650);
                break;
            case 5:
                cout << "Set Phone Number: "; cin >> phone_number; cout << "Phone Number set to " << phone_number << endl; Sleep(650);
                break;

            default:
                cout << "Invalid choice! Enter from 1 to 7" << endl; Sleep(500);
                break;
            }

            updateRecord();
        } while (option != 8);

        system("cls");
    }
    void user_setter()
    {
        bool flag = 0;
        cout << "First name: ";
        cin >> first_name;
        cout << "Last name: ";
        cin >> last_name;
        do {
            cout << "Age: ";
            cin >> age;
            if (age > 80 || age < 10) { cout << "Invalid Age! Re-enter" << endl; }
        } while (age > 80 || age < 10);

        cout << "Location(city): ";
        cin >> city;

        do {
            flag = 0;
            cout << "Enter email: ";
            cin >> email;
            if (!isEmailValid(email)) { cout << "Invalid Email! Re-enter" << endl; flag = 1; }
        } while (flag);

        do {
            cout << "Enter Phone number (numbers only): ";
            cin >> phone_number;
            if (!isPhoneNumValid(phone_number)) { cout << "Invalid Email! Re-enter" << endl; flag = 1; }
        } while (flag);

        system("cls");

    }
    void writeRecord() {
        fstream file;

        file.open("user.csv", ios::out | ios::app);


        file << id << "," << password << "," << first_name << "," << last_name << "," << age << ","
            << "," << email << "," << phone_number << "," << city << ",";

        /*for (int i = 0; i < num_skills; i++) {
            file << skill[i][0] << "," << skill[i][1] << ",";
        }*/
        file << "\n";

        file.close();


    }
    void updateRecord() {
        vector<string> lines;
        string line, word;
        int pos = 0, n = 0, i = 0;

        fstream file;
        file.open("user.csv", ios::in);

        getline(file, line);
        lines.push_back(line);
        n++;

        while (getline(file, line)) {

            istringstream ss(line);

            getline(ss, word, ',');
            if (id == word) {
                pos = n;
                n--;
            }
            else {
                lines.push_back(line);
            }

            n++;
        }

        file.close();



        file.open("user.csv", ios::out);

        i = 0;
        while (i < n) {

            if (i == pos || (i == n - 1 && n == pos)) {
                file << id << "," << password << "," << first_name << "," << last_name << "," << age << ","
                    << email << "," << phone_number << ","
                    << city << ",";

                /*for (int a = 0; a < num_skills; a += 2) {
                    file << skill[a][0] << "," << skill[a][1] << ",";
                }*/
                file << "\n";
            }



            file << lines[i] << "\n";
            i++;

        }

        file.close();
    }
    void deleteRecord() {
        string line, word;

        fstream inFile("user.csv", ios::in | ios::out);
        ofstream outFile("temp.csv");


        while (getline(inFile, line)) {

            istringstream ss(line);

            getline(ss, word, ',');
            if (word == id) {
                continue;
            }

            outFile << line << "\n";


        }

        inFile.close();
        outFile.close();

        remove("user.csv");
        rename("temp.csv", "user.csv");
    }
    bool username_verify(vector<User> u) {

        for (auto& user : u) {
            if (id == user.id) {
                return true;
            }
        }

        return false;
    }
    bool isEmailValid(string email) {
        const regex pattern("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
        return regex_match(email, pattern);
    }
    bool isPhoneNumValid(string str)
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (!isdigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }
    void set_user_id_and_password(vector<User> u)
    {
        bool flag = false;
        do {
            flag = 0;
            cout << "Set Username: " << "\t[Please remember this username to login into your account]" << endl;
            cin >> id;
            if (username_verify(u) == true) { cout << "Username already taken! Re-enter!" << endl; flag = 1; }
        } while (flag == 1);
        set_password();
    }
    void set_password()
    {
        int check = 0;

        regex pattern("[a-zA-Z0-9]{8,}");
        do
        {
            check = 0;
            cout << "Set Password: \t[at least 8 characters & of letters|digits only]: " << endl;
            cin >> password;

            if (!regex_match(password, pattern)) {
                cout << "Password does not match the required format! Please enter password again!" << endl;
                check = 1;

            }
            else {

                cout << "Password accepted!" << endl;
            }

        } while (check == 1);


    }
    void set_idPass(string username, string pass) { //for filing
        id = username; password = pass; 
    } 
    int getKey() {
        return key;
    }

};

class Category {
public:
    int key;
    string categoryName;
    string description;
    vector<Product> products;
    vector<string> keywords;
    vector<double> featureVector; //for similarity between different categories(content-bsaed filtering)
    Category(int key, string name, string descrip, vector<Product> products, vector<string> keywords, vector<double> featureMatrix) : key(key), categoryName(name),
        products(products), keywords(keywords), description(descrip) {}
    Category() {}
};


struct Product {
    int productID; //index in orig vector in case products are shuffled
    string name, category, brand;
    double price, overall_rating;
    int stock;
    int reviewNum;
    vector<string> reviews;
    Product(int key, string n, double p, string c, string brand, int st) : productID(key), reviewNum(0), name(n), price(p),
        category(c), stock(st), overall_rating(0) {}
};



class admin
{
    string adminID, adminPass;
    User* user;
    vector <User> users;
    vector <Category> categories;
    vector <Product> products;

public:
    admin() {
        adminID = "anas"; //can take user input to set(just saving time)
        adminPass = "anas123";
        Category c;
        c.key = 2;
        Category b;
        b.key = 1;
        Category d;
        d.key = 0;
        categories.push_back(c);
        categories.push_back(b);
        categories.push_back(d);


        //readUserToCategoryGraph();
        //readUserRecord();
        //readCategoryRecord();
        //readProductRecord();
    }

    //void login(User* u) {
    //    int check1, search1, getch();

    //    system("cls");
    //    SetColor(1);
    //    cout << "Hello " << u->get_first_name() << "! Welcome to this Wonderful Platform\n\nThe Hunt Begins!\n" << endl;
    //    SetColor(13);
    //    do
    //    {
    //        if (u->get_type() == "user") {
    //            cout << "1. Search For A Job\n\n2. View Hired/Applied to Jobs\n\n3. Subscribe to Premium*";
    //        }
    //        else {
    //            cout << "1. Post A Job\n\n2. View Jobs Posted\n\n3. All Employees";
    //        }
    //        cout << "\n\n4. View Account Details\n\n5. Modify Acccount Details\n\n6. Logout\n" << endl;
    //        cin >> search1;

    //        switch (search1)
    //        {
    //        case 1:
    //            if (u->get_type() == "user") {
    //                JobsSearch(u);
    //            }
    //            else {
    //                if (employer* e = dynamic_cast<employer*>(u)) { //needed to call jobpost(), only present in child class(employer)
    //                    e->JobPost();
    //                }
    //                else {
    //                    cout << "Dynamic cast error"; getch();
    //                }
    //            }
    //            break;

    //        case 2:
    //            viewJobs(u);
    //            break;


    //        case 3:
    //            EmpSub(u);
    //            break;

    //        case 4:
    //            u->display_details(u->get_type());
    //            system("cls");
    //            break;

    //        case 5:
    //            u->modifyDetails();

    //            readUserRecord();
    //            readJobRecord();
    //            readEmployerRecord();
    //            break;

    //        case 6:
    //            SetColor(5); cout << "\n\t\t\t\t\tLogging out.."; Sleep(300); cout << "."; Sleep(500); SetColor(0);
    //            break;

    //        default:
    //            cout << "Wrong Entry!"; Sleep(400);
    //            system("cls");
    //            break;
    //        }
    //    } while (search1 != 6);

    //}
    void readUserRecord() {
        vector<string> row;

        string line, word;
        fstream file("data\\users.csv", ios::in);

        getline(file, line);
        while (getline(file, line)) {

            row.clear();
            istringstream ss(line);
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }

            users.push_back(*new User(stoi(row[0]), row[1], row[2], row[3], row[4], stoi(row[5]), row[6], row[7], row[8]));
            users.back().evaluatedCategory.resize(categories.size());

            for (int m = 9; m < row.size(); m++) {
                for (auto& category : categories) {
                    if (category.key == stoi(row[m])) {
                        users.back().evaluatedCategory[category.key] = true;
                    }
                    else {
                        if (!users.back().evaluatedCategory[category.key])
                            users.back().evaluatedCategory[category.key] = false;
                    }
                    cout << users.back().evaluatedCategory[category.key];
                }
            }
        }
    }
    //void readEmployerRecord() {
    //    vector<string> row;

    //    string line, word;

    //    int i = 0, n = 0;
    //    employers.clear();

    //    //counting number of records
    //    fstream file("employer.csv", ios::in);

    //    getline(file, line);
    //    while (getline(file, line)) {
    //        n++;
    //    }

    //    file.close();

    //    employers.resize(n);


    //    //reading records
    //    file.open("employer.csv", ios::in);

    //    getline(file, line);
    //    while (getline(file, line)) {

    //        row.clear();

    //        istringstream ss(line);

    //        while (getline(ss, word, ',')) {
    //            row.push_back(word);
    //        }

    //        employers[i].set_idPass(row[0], row[1]);
    //        employers[i].set_company_name(row[2]);
    //        employers[i].set_email(row[3]);
    //        employers[i].set_phone_number(row[4]);
    //        employers[i].set_city(row[5]);
    //        employers[i].set_department(row[6]);
    //        employers[i].set_sub_depart(row[7]);

    //        for (int m = 8; m < row.size(); m++) {//finding jobs that match particular employer and adding in its object
    //            for (auto& Job : Jobs) {
    //                if (Job.JobID == stoi(row[m])) {
    //                    employers[i].Jobs[m - 8] = &Job;
    //                    employers[i].addJobEmployees(&Job);
    //                }
    //            }
    //        }

    //        i++;

    //    }
    //}
    //void readJobRecord() {
    //    vector<string> row;

    //    string line, word;

    //    int i = 0, n = 0;
    //    Jobs.clear();

    //    //counting number of records
    //    fstream file("Jobs.csv", ios::in);

    //    getline(file, line);
    //    while (getline(file, line)) {
    //        n++;
    //    }

    //    file.close();

    //    Jobs.resize(n);


    //    //reading records
    //    file.open("Jobs.csv", ios::in);

    //    getline(file, line);
    //    while (getline(file, line)) {

    //        row.clear();

    //        istringstream ss(line);

    //        while (getline(ss, word, ',')) {

    //            row.push_back(word);

    //        }

    //        Jobs[i].setJobID(stoi(row[0]));
    //        Jobs[i].setTitle(row[1]);
    //        Jobs[i].setDec(row[2]);
    //        Jobs[i].setExp(stoi(row[3]));
    //        Jobs[i].setMinMax(stoi(row[4]), stoi(row[5]));
    //        Jobs[i].setOpenings(stoi(row[6]));

    //        for (int m = 0, n = 7; n < 12; n += 2, m++) {
    //            if (row[n] != "") {
    //                Jobs[i].set_skill(row[n], row[n + 1], m);
    //            }
    //        }

    //        for (int n = 17; n < 23; n++) {
    //            if (row[n] != "") {
    //                Jobs[i].responsibilities[n - 17] = row[n];
    //                Jobs[i].num_res++;
    //            }
    //        }

    //        int pos1 = line.find("[");
    //        int pos2 = line.find("]");
    //        string hiredStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
    //        istringstream ss2(hiredStr);

    //        while (getline(ss2, hiredStr, ',')) {
    //            for (auto& user : users) {
    //                if (user.get_username() == hiredStr) {
    //                    Jobs[i].addHired(user);
    //                }
    //            }
    //        }

    //        int pos3 = line.find("^", pos2);
    //        string AppStr = line.substr(pos2 + 2, pos3 - pos2 - 2);
    //        istringstream ss3(AppStr);

    //        while (getline(ss3, AppStr, ',')) {
    //            for (auto& user : users) {
    //                if (user.get_username() == AppStr) {
    //                    Jobs[i].addApplicant(user);
    //                }
    //            }
    //        }

    //        pos3 = line.find_last_of("^");
    //        int pos4 = line.find_last_of("~");
    //        string IntStr = line.substr(pos3 + 2, pos4 - pos3 - 3);
    //        istringstream ss4(IntStr);


    //        while (getline(ss4, IntStr, ',')) {
    //            for (auto& user : users) {
    //                if (user.get_username() == IntStr) {
    //                    Jobs[i].addInterviewee(user);
    //                }
    //            }
    //        }

    //        pos4 = line.find_last_of("~");
    //        int pos5 = line.find_last_of("|");
    //        string ScrStr = line.substr(pos4 + 2, pos5 - pos4 - 3);
    //        istringstream ss5(ScrStr);


    //        while (getline(ss5, ScrStr, ',')) {
    //            for (auto& user : users) {
    //                if (user.get_username() == ScrStr) {
    //                    Jobs[i].addScreeners(user);
    //                }
    //            }
    //        }



    //        i++;

    //    }
    //}
    //void readUserToCategoryGraph(){}

    vector<User> get_users()
    {
        return users;
    }

};