#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <windows.h>
#include <conio.h> 
#include <stdlib.h>
#define vecDouble2D vector<vector<double>>

using namespace std;


void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
void gotoxy(short x, short y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursorBack() {
    for (int i = 0; i < 21; i++) {
        cout << "\b \b";
    }
    cout << "\x1b[1A\t\t   \b\b";
}

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

    string get_username() {
        return id;
    }

    string get_password() {
        return password;
    }

    string get_first_name() {
        return first_name;
    }

};



struct Product {
    string productID; //can use int for hashing
    string name, brand, category;
    double price;
    int stock, overall_rating;
    string categoryBuffer;
    vector<string> product_categoriesName;

    Product(string key, string n, string b, string c, double p, int st, int r) : productID(key), name(n), brand(b), categoryBuffer(c),
        price(p), stock(st), overall_rating(r) {
        istringstream ss(categoryBuffer);
        int i = 0;
        string category;
        while (getline(ss, category, '|')) {
            product_categoriesName.push_back(category);
            i++;
        }
    }


    void displayProduct() {
        system("cls");
        string Title = name;
        transform(Title.begin(), Title.end(), Title.begin(), ::toupper);

        SetColor(13); cout << "\n\n\t\t\t\t  -----" << Title << "-----"; SetColor(8);
        cout << "\n\n\t\tProductID: \n\t\t  "; SetColor(9); cout << productID;
        
        SetColor(8); cout << "\n\n\t\tBrand:  \n\t\t "; SetColor(9); cout << brand; 

        SetColor(8); cout << "\n\n\t\tOverall Rating: \n\t\t ";  SetColor(4); cout << overall_rating; SetColor(9);

        SetColor(8); cout << "\n\n\n\t\t\033[4mCategories in: \033[24m: "; SetColor(9);
        SetColor(13); cout << categoryBuffer << "\t";
        SetColor(8); cout << "\033[4mPrice\033[24m: ";
        SetColor(2); cout << "$" << price << endl << endl;

        cout << endl << endl;
        SetColor(10); cout << "\n\n\n\t\t\t\t\t>> [A]dd to Cart <<";  SetColor(6);  cout << "    |    "; SetColor(12); cout << "[C]ancel"; SetColor(7);
        cout << "\n\t\t\t\t\t  _____________\t\t   ______"; SetColor(5);

        int choice = toupper(_getch());
        if (choice == 'A') {
            add_item(*this);
        }

        //recommender update
    }

};

class Category {
public:
    int key;
    string categoryName;
    string description;
    vector<string> keywords;
    string keywordBuffer;
    vector<Product> products;
    vector<double> featureVector; //for similarity between different categories(content-bsaed filtering)
    Category(int key, string name, string descrip, string k): key(key), categoryName(name),
        description(descrip), keywordBuffer(k){
        istringstream ss(keywordBuffer);
        int i = 0;
        string keyword;
        while (getline(ss, keyword, '|')) {
            keywords.push_back(keyword);
            i++;
        }
        
    }
    Category() {}
};


class Graph {
    double** UserToCategory;
    int userSize, categorySize;
public:
    Graph(int uSize, int cSize): userSize(uSize), categorySize(cSize){
        UserToCategory = new double*[uSize + cSize];
        for (int i = 0; i < uSize + cSize; i++) {
            UserToCategory[i] = new double[uSize + cSize];
            for (int j = 0; j < uSize + cSize; j++) {
                UserToCategory[i][j] = 0;
            }
        }
    }
    ~Graph() {
        for (int i = 0; i < userSize + categorySize; i++) {
            delete UserToCategory[i];
        }
        delete[] UserToCategory;
    }

    void addEdge(int UserKey, int CategoryKey, double Weight) {
        UserToCategory[UserKey - 1][userSize + CategoryKey - 1] = Weight;
    }
    
    void getGraph(vecDouble2D& G) {
        G.resize((userSize + categorySize) * (userSize + categorySize));
        for (int i = 0; i < userSize + categorySize; i++) {
            for (int j = 0; j < userSize + categorySize; j++) {
                G[i].push_back(UserToCategory[i][j]);
            }
        }
    }

    void displayRelationGraph() {
        for (int i = 0; i < userSize + categorySize; i++) {
            for (int j = 0; j < userSize + categorySize; j++) {
                cout << fixed << setprecision(0);
                cout << UserToCategory[i][j] << " ";
            }
            cout << endl;
        }
    }

};


class admin
{
    string adminID, adminPass;
    User* user;
    vector <User> users;
    vector <Category> categories;   
    vector <Product> products;
    vecDouble2D UserToCategoryGraph;
    

public:
    admin() {
        adminID = "anas"; //can take user input to set(just saving time)
        adminPass = "anas123";

        readCategoryRecord();
        readProductRecord();
        readUserRecord();
        readUserToCategoryGraph();
        //userToCategory = new Graph(users.size(), categories.size());

    }

    void displayAllProducts(User* u) {
        system("cls");
        int m = 0, x = 5, y = 6, a = 0;
        int n = 0;
        char choice = 'A';

        for (int i = 0, j = 0; j < products.size(); i++, j++) {

            gotoxy(x, y);
            SetColor(8); cout << "   --------Product#" << ++m << "--------";

            gotoxy(x, ++y);
            string e;
            string r;
            if (products[i].name.length() > 21) {
                e = products[i].name.substr(0, 21);
                SetColor(1); cout << "|>" << e << ".." << "<|";
            }
            else {
                SetColor(1); cout << "|>" << products[i].name << "<|";
            }

            gotoxy(x, ++y);
            SetColor(6); cout << "[" << products[i].brand << "]"; SetColor(8);

            gotoxy(x, ++y);
            if (products[i].categoryBuffer.length() > 21) {
                e = products[i].categoryBuffer.substr(0, 21);
                SetColor(6); cout << e << ".."; SetColor(8);
            }
            else {
                SetColor(6); cout << "Categories: " << products[i].categoryBuffer; SetColor(8);
            }

            gotoxy(x, ++y);
            cout << "Price: "; SetColor(2);  cout << "$" << products[i].price;  SetColor(8);

            gotoxy(x, ++y);
            cout << "Rating: "; SetColor(4); cout << products[i].overall_rating; SetColor(8);

            gotoxy(x, ++y);
            cout << "Stock: "; SetColor(3); cout << products[i].stock; SetColor(8);
            y = 6;
            x += 50;


            SetColor(7);
            gotoxy(2, 16);
            for (int k = 0; k < 29; k++) {
                cout << "____";
            }
            SetColor(3); gotoxy(3, 18); cout << "<[P:Prev\t\t\t\t\t\tS:Select\t\t\t\t\t     More:M]>";
            SetColor(11); gotoxy(1, 28); cout << "Press 'E' to exit"; SetColor(5);



            //cases for moving accross pages
            if (j == products.size() - 1 && m % 3 != 0) {
                x = 5, y = 6;
                do {
                    choice = toupper(_getch());
                    if (choice == 'P' && m > 3 && (m - 2) % 3 == 0) {
                        m -= 5;
                        j -= 5;//although m and i j have 1 unit dist diff i j gets ++ once the cycle repeats
                        i -= 5;//in the case of showall
                        system("cls");
                        break;
                    }
                    else if (choice == 'P' && m > 3 && (m - 1) % 3 == 0) {
                        m -= 4;
                        j -= 4;
                        i -= 4;
                        system("cls");
                        break;
                    }
                } while (choice != 'S' && choice != 'E');

            }
            else if (j != products.size() - 1 && m % 3 == 0) {
                x = 5, y = 6;
                do {
                    choice = toupper(_getch());
                    if (choice == 'P' && m > 3) {
                        m -= 6;
                        j -= 6;
                        i -= 6;
                        system("cls");
                        break;
                    }

                    if (choice == 'M') { system("cls"); }
                } while (choice != 'M' && choice != 'S' && choice != 'E');

            }
            else if (j == products.size() - 1 && m <= 3) {
                do {
                    choice = toupper(_getch());
                } while (choice != 'S' && choice != 'E');
            }
            else if (j == products.size() - 1 && m % 3 == 0) {
                x = 5, y = 6;
                do {
                    choice = toupper(_getch());
                    if (choice == 'P' && m > 3) {
                        m -= 6;
                        j -= 6;
                        i -= 6;
                        system("cls");
                        break;
                    }
                } while (choice != 'S' && choice != 'E');
            }


            if (choice == 'S') {
                gotoxy(1, 23); cout << "Enter Product-Num#: ";
                do {
                    cin >> a;

                    if (a > m || a < 0) {
                        cout << "Unavailable Re-enter!"; Sleep(500);
                        cursorBack();
                    }
                } while (a > m || a < 0);


                a--;
                products[a].displayProduct();
                
                x = 5; y = 6; m = 0; i = -1; j = -1; choice = 'B';
                system("cls");

            }
            else if (choice == 'E') { system("cls"); break; }

        }

    }

    void displayAllCategories() {
        for (auto& category : categories) {
            cout << category.key << " " << category.categoryName << endl;
        }
        _getch();
    }









    void login(User* u) {
        int check1, search1, getch();

        system("cls");
        SetColor(1);
        cout << "Hello " << u->get_first_name() << "! Welcome to this Wonderful Platform\n\nThe Hunt Begins!\n" << endl;
        SetColor(13);
        do
        {
            cout << "1. View Account Details\n\n2. Modify Acccount Details\n\n3. Browse Products ";
            cout << "\n\n4. Browse Categories \n\n5. Get Recommended Products\n\n6. View Cart\n\n7. View Favourites\n\n8. Logout" << endl;
            cin >> search1;

            switch (search1)
            {
            case 1:
                system("cls");
                u->display_details();
                break;

            case 2:
                system("cls");
                u->modifyDetails();
                readUserRecord();
                break;


            case 3:
                displayAllProducts(u);
                break;

            case 4:
                system("cls");
                displayAllCategories();
                system("cls");
                break;

            case 5:
              
                break;

            case 6:
                

            case 7:

                break;

            case 8:
                system("cls");
                SetColor(5); cout << "\n\t\t\t\t\tLogging out.."; Sleep(300); cout << "."; Sleep(500); SetColor(0);
                break;

            default:
                cout << "Wrong Entry!"; Sleep(400);
                system("cls");
                break;
            }
        } while (search1 != 6);

    }

















































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
            users.back().evaluatedCategory.resize(categories.size() + 1);

            for (int m = 9; m < row.size(); m++) {
                if (row[m] == "")
                    continue;
                for (auto& category : categories) {
                    if (category.key == stoi(row[m])) {
                        users.back().evaluatedCategory[category.key] = true;
                    }
                    else {
                        if (!users.back().evaluatedCategory[category.key])
                            users.back().evaluatedCategory[category.key] = false;
                    }
                }
            }

        }
        

        file.close();
    }

    void readCategoryRecord() {
        vector<string> row;

        string line, word;
        fstream file("data\\categories.csv", ios::in);

        getline(file, line);
        while (getline(file, line)) {

            row.clear();
            istringstream ss(line);
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }

            categories.push_back(*new Category(stoi(row[0]), row[1], row[2], row[3]));
        }

        file.close();
    }
    
    void readProductRecord() {
        vector<string> row;

        string line, word;
        fstream file("data\\products.csv", ios::in);

        getline(file, line);
        while (getline(file, line)) {

            row.clear();
            istringstream ss(line);
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }

            products.push_back(*new Product(row[0], row[1], row[2], row[3], stod(row[4]), stoi(row[5]), stoi(row[6])));

            for (auto& category : categories) { ///POSSIBLE HASHING
                for (auto& productCategories : products.back().product_categoriesName) {
                    if (productCategories == category.categoryName) {
                        category.products.push_back(products.back());
                    }
                }
            }
        }

        file.close();
    }

    
    void readUserToCategoryGraph() {
        vector<string> row;
        Graph userToCategory(users.size(), categories.size());
        string line, word;
        fstream file("data\\user category training set.csv", ios::in);

        getline(file, line);
        while (getline(file, line)) {

            row.clear();
            istringstream ss(line);
            while (getline(ss, word, ',')) {
                row.push_back(word);
            }

            userToCategory.addEdge(stoi(row[0]), stoi(row[1]), stod(row[2]));
        }

        //userToCategory.displayRelationGraph();
        userToCategory.getGraph(UserToCategoryGraph);
        file.close();

    }

    vecDouble2D getGraph() {
        return UserToCategoryGraph;
    }

    string get_username() {
        return adminID;
    }

    string get_password() {
        return adminPass;
    }

    vector<User> get_users()
    {
        return users;
    }

};