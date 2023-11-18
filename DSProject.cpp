#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> //1 decimal place print
#define vecDouble2D vector<vector<double>>

using namespace std;

class User {
public:
    vector<bool> evaluatedCategory;
    int userID;


};

struct Category {
public:
    int key;
    string categoryName;
    vector<string> keywords;
    string description;
    vector<double> featureVector; // for similarity between different categories using cosine similarity on both vectors
    Category(int key, string name, vector<string> keywords, string descrip, vector<double> featureMatrix, bool evaluated): key(key), categoryName(name),
        keywords(keywords), description(descrip){}
    Category(){}
};


struct Product {
    string name, category, brand;
    int stock;
    double price, overall_rating;
    int reviewNum;
    vector<string> reviews;
    int key; //index in orig vector in case products are shuffled
    Product(int key, string n, double p, string c, string brand, int st): key(key), reviewNum(0), name(n), price(p),
                                                                          category(c), stock(st), overall_rating(0){}
};

class RecommenderSystem {
    
public:
    vector<double> average;
    vector<User> users;
    vector<Category> categories;
    vecDouble2D userToCategory;
    vecDouble2D test_userToCat;
    int userSize;
    int categorySize;
    static double RMSE_UToU;
    static double RMSE_CToC;
    bool testData;

    RecommenderSystem(vecDouble2D userToCategory, vector<User> users, vector<Category> categories, bool testing) {
        this->userToCategory = userToCategory;
        this->users = users;
        this->categories = categories;
        userSize = users.size();
        categorySize = categories.size();
        average.resize(userSize);
        testData = testing;
        if (testing) { //to calculate better recommender
            test_userToCat = userToCategory;
            CollaborativeFilteringCategoryCategory();
            CollaborativeFilteringUserUser();
        }
        else {
            switchingHybridRecommend();
        }
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
                    for (int k = userSize, n = 0;  k < userToCategory[0].size(); k++, n++) {
                        if (k == i) continue;
                        top += userToCategory[i][k] * userToCategory[j][k];
                        bottom += abs(userToCategory[i][k]);

                    }

                    if (top && bottom) {
                        pred_rating = top / bottom;
                            
                        if (testData) {
                            SquareError += pow(test_userToCat[j][i] - pred_rating, 2);
                            validCount++;
                        }
                        userToCategory[j][i] = pred_rating;
                    }
                }
            }
        }

        if (testData) {
            RMSE_CToC = SquareError / validCount;
            RMSE_CToC = sqrt(RMSE_CToC);
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
                    userToCategory[i][j] =  dotProduct / (sqrt(magnitude1) * sqrt(magnitude2));
                else
                    userToCategory[i][j] = 0;
            }
        }
    }

    void predict_UToU() {
        double SquareError = 0;
        int validCount = 0;
        for (int i = 0; i < userSize; i++) {
            for (int j = userSize, n = 0; j < userToCategory[0].size(); j++, n++){
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
                        if (testData) {
                            SquareError += pow(test_userToCat[i][j] - (pred_rating + average[i]), 2);
                            validCount++;
                        }
                  
                        userToCategory[i][j] = pred_rating;
                    }
                    else {
                        userToCategory[i][j] = pred_rating - average[i];
                    }
                }
            }
        }

        if (testData) {
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


//ISSSUES!!!
//1. Sparsity problem(graph) e.g 1 category interacted
//2. Zero vector after norm of data (no recommendation for that user) 

int main()
{
    vector<Category> categories(3);  //categories.csv
    vector<Product> products;        //products.csv
    vector<User> users(3);           //users.csv
    vecDouble2D userToCategory = {
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 1, 5, 0},
        {0, 0, 0, 0, 5, 3},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
    };

    vecDouble2D userToCategory_testSet = {
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 2, 5, 3},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
    };
    
    users[0].evaluatedCategory.push_back(true);
    users[0].evaluatedCategory.push_back(true);
    users[0].evaluatedCategory.push_back(true);
    users[1].evaluatedCategory.push_back(true);
    users[1].evaluatedCategory.push_back(true);
    users[1].evaluatedCategory.push_back(false);
    users[2].evaluatedCategory.push_back(false);
    users[2].evaluatedCategory.push_back(true);
    users[2].evaluatedCategory.push_back(true);



    RecommenderSystem testRecommender(userToCategory_testSet, users, categories, true);
    RecommenderSystem Recommender(userToCategory, users, categories, false);

    Recommender.displayRelationGraph();
}

/*user category relationship(directed) based on :  1. bought           score: +4             similarity score checker: cosine similarity(0.0-1.0)
                                                   2. favorited        score: +3                         e.g cos(5) = 0.9960
                                                   3. viewed           score: +2
                                                   4. rated            score: if(x > 3.5) +1, else -2(viewed +2)
                                                   5. unfavorite       score: -3

  itemToitem relationship(undirected)           :   (if 2 products have same category, price range(past history dependent), product categories have similar keywords)
  userTouser relationship(undirected)           :  if(u2 has more than 2 categories same as u1) recommend similar products 
                                                    to u1 as u2(other than similar)
                                                    and vice versa


  overall 1 mixed-graph(directed and undirected)
  recommender system will recommend both user interacted category products and other similar category products.(content based filtering)
  it will also recommend category products from the user, a user has relationship with.

*/
//Program flow
//login user
/*user option menu(after login):
1. browse products -> 1) view -> 1) buy now
                                 2) add to cart
                                 3) favorites
                                 4) rate product
                                 5) back
                      2) search -> view -> 1) buy now
                                           2).. 
                                           ..

                      3) add filter -> 1) price -> view..
                                       2) brand

                      4) reload page(shuffle products)                    ..
                                           ..

2. browse cateogories -> 1) fitness -> view ->  1) buy now -> 1) quantity -> (validation checks and recommender)  
                                       search   2) add to cart
                                       filter   3) favorites
                                       reload   4) rate product
                                                5) back       
                             
                         2)..
                         3)..

3. Get Recommended Products -> view
                               search (recommender not used)
                               filter
                               reload

4. view cart -> 1) remove from cart
                2) checkout

5. view favorites -> 1) product1 -> 1) buy now
                     2).            2) add to cart
                                    3)..
                                    4)..
                                    5)..

vector<vector<double>> userCategoryRel =>      u1  u2 c1 c2 c3
                                           u1  {0, 1, 2, 3, 0},   //c1 similar to c3(features): c3 should be recommended(to u1) | ML:content based filtering
                                           u2  {1, 0, 2, 0, 0},   //u2 similar to u1          : c2 should be recommended(to u2) | ML:collaborative filtering
                                           c1  {0, 0, 0, 0, 1},   
                                           c2  {0, 0, 0, 0, 0},
                                           c3  {0, 0, 1, 0, 0}.

Graph type: Mixed Bipartite graph [directed-undirected weighted(multi edge)]
Traversal: BFS (breadth first search) using queue


};

*/

