#ifndef NETWORK_H
#define NETWORK_H
#include "user.h"
#include <string>
#include <vector>
#include <deque>

class Network {
public:
   Network();
   ~Network();
   int read_friends(const char *filename);
   int write_friends(const char *filename);
   void add_user(std::string username, int yr, int zipcode);
   int add_connection(std::string name1, std::string name2);
   int remove_connection(std::string name1, std::string name2);
   int get_id(std::string username);

   // network 2 functions
   std::vector<int> shortest_path(int from, int to);
   std::vector< std::vector<int> > groups();
   std::vector<int> suggest_friends(int who, int& score);

   // return functions
   int return_size();
   int return_id(int i);
   std::string return_name(int i);
   int return_year(int i);
   int return_zip(int i);
   std::vector<int> return_friends(int i);

private:
   std::vector<User> _users;

};

#endif