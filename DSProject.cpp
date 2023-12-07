#include "recommender.h"

using namespace std;

int main()
{

    admin a;
	vector<User> users;
	vector<Category> categories;
	users = a.get_users();
	categories = a.get_categories();
    vecDouble2D userToCategory_trainSet = a.getGraph();
    vecDouble2D userToCategory_testSet  = a.getGraph();



   RecommenderSystem testRecommender(userToCategory_trainSet, users, categories, true);
   RecommenderSystem Recommender(userToCategory_testSet, users, categories, false);

  
   int choice, check1 = 0, i = 0, check2 = 0, count = 0, userSize = 0,  incUsername = 0;
   User u;
   string user_name, password;
   char _end = 'e';


   intro();

   do
   {
	   system("Color F0");
	   frame();

	   cout << "Enter from the choice of action:\n\t\t1: Sign up as a User\n\t\t2: Login as a User\n\t\t3: Admin\n\n\t\tPress 4 to exit\t\t";
	   cin >> choice;
	   system("cls");
	   switch (choice)
	   {

	   case 1:
		   SetColor(1); cout << "-->Enter User details<--\n" << endl; SetColor(4);
		   u.user_setter();
		   _getch();

		   break;


	   case 2:
		   do {
			   frame();
			   cout << "-----LOGIN-----\n\n\t\t";

			   a.readUserRecord();
			   users = a.get_users();

			   cout << "Enter Username: ";
			   cin >> user_name;



			   incUsername = 1;
			   for (i = 0; i < users.size(); i++)
			   {
				   if (users[i].get_username() == user_name)
				   {
					   count = 0;
					   incUsername = 0;
					   do
					   {
						   gotoxy(16, 7);
						   cout << "Enter password: ";
						   cin >> password;

						   if (users[i].get_password() == password)
						   {
							   a.recommendations = Recommender.computeTopKRecommendations(&users[i]);
							   a.login(&users[i]);
							   break; //count =3;
						   }
						   else
						   {
							   gotoxy(10, 16);
							   SetColor(4);
							   cout << "Incorrect Password!!\tAttempt: " << count + 1 << "/3" << endl;
							   SetColor(0);
							   count++;
						   }
					   } while (count < 3);
					   break;
				   }
			   }

			   if (incUsername == 1) {
				   gotoxy(9, 18); SetColor(4);  cout << "Incorrect Username! Press any key to continue\n\n\n\t ";
				   SetColor(8); cout << "Press 'B' to return"; _end = toupper(_getch());
			   }

		   } while (_end != 'B' && incUsername == 1);

		   break;

	   case 3:
		   do {
			   frame();
			   cout << "-----LOGIN-----\n\n\t\t";


			   cout << "Enter Username: ";
			   cin >> user_name;

			   incUsername = 1;

			   if (a.get_username() == user_name)//check
			   {
				   count = 0;
				   incUsername = 0;
				   do
				   {
					   gotoxy(16, 7);
					   cout << "Enter password: ";
					   cin >> password;

					   if (a.get_password() == password)
					   {
						//   a.login();
						   break;
					   }
					   else
					   {
						   gotoxy(10, 16);
						   SetColor(4);
						   cout << "Incorrect Password!!\tAttempt: " << count + 1 << "/3" << endl;
						   SetColor(0);
						   count++;
					   }
				   } while (count < 3);
				   break;
			   }


			   if (incUsername == 1) {
				   gotoxy(9, 18); SetColor(4);  cout << "Incorrect Username! Press any key to continue\n\n\n\t ";
				   SetColor(8); cout << "Press 'B' to return"; _end = toupper(_getch());
			   }

		   } while (_end != 'B' && incUsername == 1);

		   break;

	   case 4://added so it doesnt go to default
		   break;

	   default:
		   frame();
		   SetColor(4);
		   cout << "Invalid entry! Press any key to continue" << endl;
		   _getch();
		   break;

	   }
   } while (choice != 6);


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

