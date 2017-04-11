#include "network.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <iomanip>

using namespace std;

void print_menu() {
   cout << endl;
   cout << "MENU:" << endl;
   cout << "Option 1: Add a user" << endl;
   cout << "Option 2: Add friend connection" << endl;
   cout << "Option 3: Remove friend connection" << endl;
   cout << "Option 4: Print users" << endl;
   cout << "Option 5: List friends" << endl;
   cout << "Option 6: Write to file" << endl;
   cout << "Option 7: Find the shortest path" << endl;
   cout << "Option 8: Find any disjoint sets" << endl;
   cout << "Option 9: Make friend suggestion" << endl;
   cout << "Exit the program: type in any invalid integer" << endl;
   cout << "Enter your command: ";
}

int main(int argc, char *argv[]) {
   if (argc < 2) {
      cout << "Please provide a file name!" << endl;
      return 1;
   }

   Network net1;
   char *ifile_name = argv[1];
   int ifile_open = net1.read_friends(ifile_name);
   while (ifile_open == -1) {
      cout << "Failed to open this file!" << endl;
      cout << "Please provide a valid input file: ";
      cin >> ifile_name;
      ifile_open = net1.read_friends(ifile_name);
   }

   bool off = false;
   cout << left;
   while (off == false) {
      print_menu();

      int option;
      cin >> option;
      string myline;
      getline(cin, myline);
      stringstream ss(myline);
      cout << endl;

      if (option == 1) { // add a user
         string first, last;
         int year, zip;
         ss >> first >> last >> year >> zip;
         string name = first + " " + last;
         net1.add_user(name, year, zip);
      }

      else if (option == 2) { // add friend connection
         string first1, last1, first2, last2;
         ss >> first1 >> last1 >> first2 >> last2;
         string name1 = first1 + " " + last1;
         string name2 = first2 + " " + last2;
         int result = net1.add_connection(name1, name2);
         if (result == -1) {
            cout << "Invalid users!" << endl;
         }
      }

      else if (option == 3) { // remove friend connection
         string first1, last1, first2, last2;
         ss >> first1 >> last1 >> first2 >> last2;
         string name1 = first1 + " " + last1;
         string name2 = first2 + " " + last2;
         int result = net1.remove_connection(name1, name2);
         if (result == -1) {
            cout << "Invalid users or not friends yet!" << endl;
         }
      }
      else if (option == 4) { // print users
         cout << setw(5) << "ID" << setw(25) << "Name";
         cout << setw(10) << "Year" << setw(10) << "Zip" << endl;
         cout << "==================================================" << endl;

         for (int i = 0; i < net1.return_size(); i++) {
            cout << setw(5) << net1.return_id(i);
            cout << setw(25) << net1.return_name(i);
            cout << setw(10) << net1.return_year(i);
            cout << setw(10) << net1.return_zip(i) << endl;
         }
      }

      else if (option == 5) { // list friends
         string first, last;
         ss >> first >> last;
         string name = first + " " + last;
         int id = net1.get_id(name);
         if (id == -1) {
            cout << "Invalid users!" << endl;
         }

         else {
            vector<int> friends = net1.return_friends(id);

            cout << setw(5) << "ID" << setw(25) << "Name";
            cout << setw(10) << "Year" << setw(10) << "Zip" << endl;
            cout << "==================================================" << endl;

            for (int j = 0; j < friends.size(); j++) {
               int id = friends[j];
               cout << setw(5) << net1.return_id(id);
               cout << setw(25) << net1.return_name(id);
               cout << setw(10) << net1.return_year(id);
               cout << setw(10) << net1.return_zip(id) << endl;
            }
         }
      }

      else if (option == 6) { // write to file
         int size = ss.gcount();
         char *ofile_name = new char[size];
         ss >> ofile_name;
         int ofile_open = net1.write_friends(ofile_name);
         if (ofile_open == -1) {
            cout << "Failed to open this file! Please try again." << cout;
         }

         delete [] ofile_name;
      }

      else if(option == 7) { // shortest path
         string first1, last1, first2, last2;
         ss >> first1 >> last1 >> first2 >> last2;
         string name1 = first1 + " " + last1;
         string name2 = first2 + " " + last2;
         int id1 = net1.get_id(name1); // from
         int id2 = net1.get_id(name2); // to
         vector<int> path = net1.shortest_path(id1, id2);

         if (path.size() > 0) {
            cout << "Distance: " << path.size() - 1 << endl;
            for (int i = 0; i < path.size(); i++) {
               if (i != path.size() - 1) {
                  cout << net1.return_name(path[i]) << " -> ";
               }
               else { // last one
                  cout << net1.return_name(path[i]) << endl;
               }
            }
         }
         else {
            cout << "None" << endl;
         }
      }

      else if (option == 8) {
         vector< vector<int> > groups = net1.groups();
         for (int i = 0; i < groups.size(); i++) {
            cout << "Set " << i << " => ";
            vector<int> set = groups[i];
            for (int j = 0; j < set.size(); j++) {
               int id = set[j];
               if (j < set.size() - 1) {
                  cout << net1.return_name(id) << ", ";
               }
               else { // last one
                  cout << net1.return_name(id) << endl;
               }
            }
         }
      }

      else if (option == 9) {
         string first, last;
         ss >> first >> last;
         string name = first + " " + last;
         int id = net1.get_id(name);
         int score = 0;
         vector<int> suggested = net1.suggest_friends(id, score);

         if (suggested.size() > 0) {
            cout << "The suggested friend(s) is/are:" << endl;
            for (int i = 0; i < suggested.size(); i++) {
               int id = suggested[i];
               cout << "\t" << setw(25) << net1.return_name(id);
               cout << "Score: " << score << endl;
            }
         }
         else {
            cout << "None" << endl;
         }
      }

      else { // exit the program
         off = true;
      }

   }

     return 0;
}
