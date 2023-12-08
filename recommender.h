#pragma once
#include <iomanip>
#include "user.h"


using namespace std;

template <typename T>
class PriorityQueue {
    vector<T> heap;
    int heapSize;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown() {
        int size = heap.size();
        int index = 0;
        while (index < size) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < size && heap[leftChild] < heap[smallest])
                smallest = leftChild;

            if (rightChild < size && heap[rightChild] < heap[smallest])
                smallest = rightChild;

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else {
                break;
            }
        }

    }

public:

    void push(T value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    T pop() {
        if (heap.empty()) {
            throw out_of_range("Priority queue is empty");
        }

        T root = heap[0];
        swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        heapifyDown();
        return root;
    }

    // Return the smallest element (root) without removing it
    T top() const {
        if (heap.empty()) {
            throw out_of_range("Priority queue is empty");
        }
        return heap[0];
    }

    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }
};



void frame() {
    system("cls");
    int i, j, k;

    SetColor(8);
    for (i = 0; i < 25; i++) {
        for (j = 5; j < 90; j++) {
            gotoxy(j, i);
            cout << " ";

            if (j == 5 || j == 90 - 1) {
                gotoxy(j, i);
                cout << "!";
            }


            if (i == 0 || i == 25 - 1) {
                gotoxy(j, i);
                cout << "~";
            }
        }
    }
    gotoxy(15, 3);
    SetColor(0);//default color
}


void intro() {

    system("Color E7");
    frame();
    gotoxy(32, 6);
    SetColor(5);
    cout << "+++++WELCOME TO IntelliCartX+++++";
    SetColor(1);
    cout << "\n\n\n\n\t\t\t   Ready to hunt for high quality products ? \n\n\t\t\t\tYou are at the right place!" << endl;
    SetColor(8);
    cout << "\n\n\n\t\t\t\tPress any key to continue";

    _getch();
    system("cls");
}

class RecommenderSystem {
    vector<double> average;
    vector<User> users;
    vector<Category> categories;
    vecDouble2D userToCategory;
    vecDouble2D train_userToCat;
    int userSize, categorySize;
    bool trainData;
    static double RMSE_UToU;
    static double RMSE_CToC;

public:

    RecommenderSystem(vecDouble2D userToCategory, vector<User>& users, vector<Category>& categories, bool training) {
        this->userToCategory = userToCategory;
        this->users = users;
        this->categories = categories;
        userSize = users.size();
        categorySize = categories.size();
        average.resize(userSize);
        trainData = training;
        if (training) { //to choose better recommender
            train_userToCat = userToCategory;
            CollaborativeFilteringCategoryCategory();
            CollaborativeFilteringUserUser();
        }
        else {
            SwitchingHybridRecommend();
            cout << endl << RMSE_CToC << " ";
            cout << RMSE_UToU << endl;
        }
    }


    void SwitchingHybridRecommend() {
        if (RMSE_UToU < RMSE_CToC)
            CollaborativeFilteringUserUser();
        else
            CollaborativeFilteringCategoryCategory();
    }

    void CollaborativeFilteringCategoryCategory() {
        similarItems();
        predict_CToC();
    }

    void CollaborativeFilteringUserUser() {
        getAverage();
        normalizeData();
        similarUsers();
        predict_UToU();
        denormalizeData();
    }

    void similarItems() {
        for (int i = userSize, c1 = 0; i < userToCategory[0].size(); i++, c1++) {
            for (int j = userSize, c2 = 0; j < userToCategory[0].size(); j++, c2++) {
                double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;
                int count = 0;

                if (i == j) {
                    userToCategory[i][j] = 1;
                    continue;
                }

                for (int k = 0; k < userSize; k++) {
                    if (!users[k].evaluatedCategory[c1] || !users[k].evaluatedCategory[c2]) continue;
                    dotProduct += userToCategory[k][i] * userToCategory[k][j];
                    magnitude1 += userToCategory[k][i] * userToCategory[k][i];
                    magnitude2 += userToCategory[k][j] * userToCategory[k][j];
                    count++;
                }

                if (magnitude1 && magnitude2)
                    userToCategory[i][j] = dotProduct / (sqrt(magnitude1) * sqrt(magnitude2));
                else
                    userToCategory[i][j] = 0;

            }
        }

    }
    void predict_CToC() {
        double SquareError = 0;
        int validCount = 0;
        for (int i = userSize, c1 = 0; i < userToCategory[0].size(); i++, c1++) {
            for (int j = 0; j < userSize; j++) {
                if (!users[j].evaluatedCategory[c1]) {
                    double pred_rating = 0, top = 0, bottom = 0;
                    for (int k = userSize, n = 0; k < userToCategory[0].size(); k++, n++) {
                        if (k == i) continue;
                        top += userToCategory[i][k] * userToCategory[j][k];
                        bottom += abs(userToCategory[i][k]);

                    }

                    if (top && bottom) {
                        pred_rating = top / bottom;

                        if (trainData && train_userToCat[j][i]) {
                            SquareError += pow(train_userToCat[j][i] - pred_rating, 2);
                            validCount++;
                            //users[j].evaluatedCategory[c1] = true;
                        }
                        userToCategory[j][i] = pred_rating;
                    }
                }
            }
        }

        if (trainData) {
            RMSE_CToC = SquareError / validCount;
            RMSE_CToC = sqrt(RMSE_CToC);
        }

    }


    void getAverage() {
        double sum = 0;
        double count = 0;
        for (int i = 0; i < userSize; i++) {
            sum = 0;
            count = 0;
            for (int j = userSize, k = 0; j < userToCategory[0].size(); j++, k++) {// category loop
                if (users[i].evaluatedCategory[k]) {
                    sum += userToCategory[i][j];
                    count++;
                }
            }
            if (count)
                average[i] = sum / count;
            else
                average[i] = 0;
        }
    }
    void normalizeData() {
        for (int i = 0; i < userSize; i++) {
            for (int j = userSize, k = 0; j < userToCategory[0].size(); j++, k++) {
                if (users[i].evaluatedCategory[k]) {
                    userToCategory[i][j] -= average[i];
                }
            }
        }
    }
    void denormalizeData() {
        for (int i = 0; i < userSize; i++) {
            for (int j = userSize; j < userToCategory[0].size(); j++) {
                userToCategory[i][j] += average[i];
            }
        }
    }
    void similarUsers() { //using cosine similarity
        for (int i = 0; i < userSize; i++) {
            for (int j = 0; j < userSize; j++) {
                double dotProduct = 0, magnitude1 = 0, magnitude2 = 0;
                int count = 0;

                if (i == j) {
                    userToCategory[i][j] = 1;
                    continue;
                }

                for (int k = userSize, n = 0; k < userToCategory[0].size(); k++, n++) {//categories
                    if (!users[i].evaluatedCategory[n] || !users[j].evaluatedCategory[n]) continue;
                    dotProduct += userToCategory[i][k] * userToCategory[j][k];
                    magnitude1 += userToCategory[i][k] * userToCategory[i][k];
                    magnitude2 += userToCategory[j][k] * userToCategory[j][k];
                    count++;
                }

                if (magnitude1 && magnitude2)
                    userToCategory[i][j] = dotProduct / (sqrt(magnitude1) * sqrt(magnitude2));
                else
                    userToCategory[i][j] = 0;
            }
        }
    }
    void predict_UToU() {
        double SquareError = 0;
        int validCount = 0;
        for (int i = 0; i < userSize; i++) {
            for (int j = userSize, n = 0; j < userToCategory[0].size(); j++, n++) {
                if (!users[i].evaluatedCategory[n]) {
                    double pred_rating = 0, top = 0, bottom = 0;
                    for (int k = 0; k < userSize; k++) {
                        if (k == i) continue;
                        top += userToCategory[i][k] * userToCategory[k][j];
                        bottom += abs(userToCategory[i][k]);
                    }

                    if (top && bottom) {
                        pred_rating = top / bottom;
                        //RMSE - checking efficincy of prediction model
                        if (trainData && train_userToCat[i][j]) {
                            SquareError += pow(train_userToCat[i][j] - (pred_rating + average[i]), 2);
                            validCount++;
                            users[i].evaluatedCategory[n] = true; //THIS IF TRAINING IS DONE ONCE OVERALL BEFORE TESTING 
                        }

                        userToCategory[i][j] = pred_rating;
                    }
                    else {
                        userToCategory[i][j] = pred_rating - average[i];
                    }
                }
            }
        }

        if (trainData) {
            RMSE_UToU = SquareError / validCount;
            RMSE_UToU = sqrt(RMSE_UToU);
        }
    }

    //Using Priority Queue:      in O(nlogk)
    //While      Introsort: does in O(nlogn)
    vector<Category> computeTopKRecommendations(User* u) {
        vector<double> arr;
        for (int j = userSize; j < userToCategory[0].size(); j++) {
            arr.push_back(userToCategory[u->getKey()][j]);
        }

        vector<double> largest_K_Vals = findTopK(arr, 10);
        vector<Category> recommendations;
        double threshold = 0;
        for (int j = userSize, k = 0; j < userToCategory[0].size(); j++) {
            if (userToCategory[u->getKey()][j] == largest_K_Vals[k] && largest_K_Vals[k] > threshold)
                recommendations.push_back(categories[k]);
        }
        return recommendations;
    }

    vector<double> findTopK(const vector<double>& arr, int k) {
        PriorityQueue<double> minHeap;

        for (int i = 0; i < k; ++i) {
            minHeap.push(arr[i]);
        }

        for (int i = k; i < arr.size(); ++i) {
            if (arr[i] > minHeap.top()) {
                minHeap.pop();
                minHeap.push(arr[i]);
            }
        }

        vector<double> result;
        result.reserve(k);
        while (!minHeap.empty()) {
            result.push_back(minHeap.top());
            minHeap.pop();
        }

        reverse(result.begin(), result.end()); //make descending
        return result;
    }
    

    void displayRecommendations(vector<Category> recommendations){
        for (int i = 0; i < recommendations.size();) {
            if (i % 5 == 0) {
                //display
                i++;
            }
            recommendations[i].products[rand() % recommendations[i].products.size() - 1];
        }
    }

    vecDouble2D getRecommendedData() {
        return userToCategory;
    }
    void displayRelationGraph() {
        for (int i = 0; i < userToCategory.size(); i++) {
            for (int j = 0; j < userToCategory.size(); j++) {
                cout << fixed << setprecision(3);
                cout << userToCategory[i][j] << "\t";
            }
            cout << endl;
        }
    }
};

double RecommenderSystem::RMSE_UToU = 0;
double RecommenderSystem::RMSE_CToC = 0;
