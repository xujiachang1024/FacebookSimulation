#ifndef USER_H
#define USER_H
#include <string>
#include <vector>

class User {
public:
   User(int id, std::string name, int year, int zip, std::vector<int> friends);
   ~User();
   void add_friend(int id);
   bool delete_friend(int id);
   // Accessors:
   int return_id();
   std::string return_name();
   int return_year();
   int return_zip();
   std::vector<int> return_friends();
   int return_depth();
   int return_pred();
   int return_score();
   void set_user(int depth, int pred);
   void inc_score();
   
private:
   int _id;
   std::string _name;
   int _year;
   int _zip;
   std::vector<int> _friends;
   int _depth;
   int _pred;
   int _score;

};

#endif