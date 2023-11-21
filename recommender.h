#pragma once
#include <iomanip>
#include "user.h"
#define vecDouble2D vector<vector<double>>

using namespace std;

class RecommenderSystem {

public:
    vector<double> average;
    vector<User> users;
    vector<Category> categories;
    vecDouble2D userToCategory;
    vecDouble2D train_userToCat;
    int userSize;
    int categorySize;
    static double RMSE_UToU;
    static double RMSE_CToC;
    bool trainData;

    RecommenderSystem(vecDouble2D userToCategory, vector<User>& users, vector<Category>& categories, bool training) {
        this->userToCategory = userToCategory;
        this->users = users;
        this->categories = categories;
        userSize = users.size();
        categorySize = categories.size();
        average.resize(userSize);
        trainData = training;
        if (training) { //to ca'lculate better recommender
            train_userToCat = userToCategory;
            CollaborativeFilteringCategoryCategory();
            CollaborativeFilteringUserUser();
        }
        else {
            switchingHybridRecommend();
        }
    }


    void switchingHybridRecommend() {
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

                        if (trainData) {
                            SquareError += pow(train_userToCat[j][i] - pred_rating, 2);
                            validCount++;
                        }
                        userToCategory[j][i] = pred_rating;
                        //users[j].evaluatedCategory[c1] = true;
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
                        //RSME - checking efficincy of prediction model
                        if (trainData) {
                            SquareError += pow(train_userToCat[i][j] - (pred_rating + average[i]), 2);
                            validCount++;
                        }

                        userToCategory[i][j] = pred_rating;
                        //users[i].evaluatedCategory[n] = true;
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
