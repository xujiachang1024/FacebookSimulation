#include "user.h"
#include <string>
#include <vector>

User::User(int id, std::string name, int year, int zip, std::vector<int> friends) {
   _id = id;
   _name = name;
   _year = year;
   _zip = zip;
   _friends = friends;
   _depth = -1;
   _pred = -1;
   _score = 0;
}

User::~User() {}

void User::add_friend(int id) {
   _friends.push_back(id);
}

bool User::delete_friend(int id) {
   bool deleted = false;
   for (int i = 0; i < _friends.size(); i++) {
      if (_friends[i] == id) {
         _friends.erase(_friends.begin() + i);
         deleted = true;
         break;
      }
   }
   return deleted;
}

int User::return_id() {
   int id = _id;
   return id;
}

std::string User::return_name() {
   std::string name = _name;
   return name;
}

int User::return_year() {
   int year = _year;
   return year;
}

int User::return_zip() {
   int zip = _zip;
   return zip;
}

std::vector<int> User::return_friends() {
   std::vector<int> friends = _friends;
   return friends;
}

int User::return_depth() {
   int depth = _depth;
   return depth;
}

int User::return_pred() {
   int pred = _pred;
   return pred;
}

int User::return_score() {
   int score = _score;
   return score;
}

void User::set_user(int depth, int pred) {
   _depth = depth;
   _pred = pred;
}

void User::inc_score() {
   _score++;
}