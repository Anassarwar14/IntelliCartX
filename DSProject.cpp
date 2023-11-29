#include "recommender.h"

using namespace std;


//ISSSUES!!!
//1. Sparsity problem(graph) e.g 1 category interacted
//2. Zero vector after norm of data (no recommendation for that user) 

int main()
{
    vector<Category> categories(3);  //categories.csv
    vector<Product> products;        //products.csv
    vector<User> users(3);           //users.csv
    
    vecDouble2D userToCategory_trainSet = {
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 2, 5, 3},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
    };
    
    vecDouble2D userToCategory_testSet = {
        {0, 0, 0, 1, 5, 4},
        {0, 0, 0, 1, 5, 0},
        {0, 0, 0, 0, 5, 3},
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



    RecommenderSystem testRecommender(userToCategory_trainSet, users, categories, true);
    RecommenderSystem Recommender(userToCategory_testSet, users, categories, false);

    Recommender.displayRelationGraph();

    admin a;
    a.readUserRecord();
}

/*user category relationship(directed) based on :  1. bought           score: +4           similarity checker: cosine similarity(-1.0 - 1.0)
                                                   2. favorited        score: +3                          cos() = A.B / ||A||.||B||
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

