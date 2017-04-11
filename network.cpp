#include "network.h"
#include "user.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>

Network::Network() {}

Network::~Network() {}

int Network::read_friends(const char *filename) {
   std::ifstream ifile(filename);
   if (ifile.fail()) {
      return -1; // failure
   }

   int num_ppl;
   ifile >> num_ppl; // number of people in the file

   for (int i = 0; i < num_ppl; i++) {
   
      // read in the user's ID
      int id;
      ifile >> id;

      // initialize a tab variable to contain the '\t' character
      std::string tab;

      // read in the user's name
      getline(ifile, tab, '\t');
      std::string name;
      getline(ifile, name);

      // read in the user's year;
      getline(ifile, tab, '\t');
      int year;
      ifile >> year;

      // read in the user's zipcode;
      getline(ifile, tab, '\t');
      int zip;
      ifile >> zip;

      // read in the user's friend list
      getline(ifile, tab, '\t');
      std::string list;
      getline(ifile, list);
      std::stringstream ss(list);
      int friend_id;
      std::vector<int> friends;
      while (ss >> friend_id) {
         friends.push_back(friend_id);
      }

      // add the new guy to the network
      User new_guy(id, name, year, zip, friends);
      _users.push_back(new_guy);
   }

   ifile.close(); // close the input file

   return 0; // success
}

int Network::write_friends(const char *filename) {
   std::ofstream ofile(filename);
   if (ofile.fail()) {
      return -1; // failure
   }

   ofile << _users.size() << std::endl;

   for (int i = 0; i < _users.size(); i++) {
      // write down this user's ID
      ofile << _users[i].return_id() << std::endl;

      // write down this user's name
      ofile << '\t' << _users[i].return_name() << std::endl;

      // write down this user's year
      ofile << '\t' << _users[i].return_year() << std::endl;

      // write down this user's zipcode
      ofile << '\t' << _users[i].return_zip() << std::endl;

      // write down this user's friend list
      std::vector<int> friends = _users[i].return_friends();
      ofile << '\t';
      for (int j = 0; j < friends.size(); j++) {
         ofile << friends[j] << " ";
      }
      ofile << std::endl;
   }

   ofile.close(); // close the output file

   return 0; // success
}

void Network::add_user(std::string username, int yr, int zipcode) {
   int id = _users.size();
   std::vector<int> friends;
   User new_guy(id, username, yr, zipcode, friends);
   _users.push_back(new_guy);
}


int Network::add_connection(std::string name1, std::string name2) {
   // initialize two users' ID and their index
   int name1_id = -1;
   int name2_id = -1;
   int name1_index = -1;
   int name2_index = -1;

   for (int i = 0; i < _users.size(); i++) {
      // check name1
      if (_users[i].return_name() == name1) {
         name1_id = _users[i].return_id();
         name1_index = i;
      }
      // check name2
      if (_users[i].return_name() == name2) {
         name2_id = _users[i].return_id();
         name2_index = i;
      }
   }

   // invalid users
   if (name1_id == -1 || name2_id == -1) {
      return -1; // faillure
   }
   // add connections
   else {
      _users[name1_index].add_friend(name2_id);
      _users[name2_index].add_friend(name1_id);
   }

   return 0; // success
}

int Network::remove_connection(std::string name1, std::string name2) {
   // initialize two users' ID and their index
   int name1_id = -1;
   int name2_id = -1;
   int name1_index = -1;
   int name2_index = -1;

   for (int i = 0; i < _users.size(); i++) {
      // check name1
      if (_users[i].return_name() == name1) {
         name1_id = _users[i].return_id();
         name1_index = i;
      }
      // check name2
      if (_users[i].return_name() == name2) {
         name2_id = _users[i].return_id();
         name2_index = i;
      }
   }

   // invalid users
   if (name1_id == -1 || name2_id == -1) {
      return -1; // failure: invalid users
   }
   // delete connections
   else {
      bool result1 = _users[name1_index].delete_friend(name2_id);
      bool result2 = _users[name2_index].delete_friend(name1_id);
      if (result1 == false || result2 == false) {
         return -1; // failure: not friends
      }
   }

   return 0; // success
}

int Network::get_id(std::string username) {
   for (int i = 0; i < _users.size(); i++) {
      if (_users[i].return_name() == username) {
         int id = _users[i].return_id();
         return id;
      }
   }

   return -1; // failure
}

std::vector<int> Network::shortest_path(int from, int to) {
   // reset every user's depth and pred value back to -1
   for (int i = 0; i < _users.size(); i++) {
      _users[i].set_user(-1, -1);
   }

   // add the start vertex to the queue
   std::deque<int> queue;
   std::vector<int> path;
   bool path_found = false;
   queue.push_back(from);
   _users[from].set_user(0, -1);

   // BFS search
   while (queue.size() != 0) {
      int front = queue.front();
      int depth = _users[front].return_depth();
      queue.pop_front();
      if (front == to) {
         path_found = true;
         break;
      }
      else {
         std::vector<int> friends = _users[front].return_friends();
         for (int j = 0; j < friends.size(); j++) {
            int id = friends[j];
            if (_users[id].return_depth() == -1) {
               queue.push_back(id);
               _users[id].set_user(depth + 1, front);
            }
         }
      }
   }

   // return the path
   if (path_found == true) {
      int id = to;
      while (id != from) {
         path.insert(path.begin(), id);
         int pred = _users[id].return_pred();
         id = pred;
      }
      path.insert(path.begin(), from);
   }

   // return the shortest path
   return path;
}

std::vector< std::vector<int> > Network::groups() {
   // reset every user's depth and pred value back to -1
   for (int i = 0; i < _users.size(); i++) {
      _users[i].set_user(-1, -1);
   }

   std::vector< std::vector<int> > groups;
   int processed = 0; // number of people processed

   while (processed < _users.size()) {
      
      std::deque<int> queue;
      std::vector<int> set;

      // find the first unprocessed user in the network
      for (int k = 0; k < _users.size(); k++) {
         if (_users[k].return_depth() == -1) {
            queue.push_back(k);
            set.push_back(k);
            _users[k].set_user(0, -1);
            processed++;
            break;
         }
      }

      // BFS search
      while (queue.size() != 0) {
         int front = queue.front();
         int depth = _users[front].return_depth();
         queue.pop_front();

         // find all unprocessed friends
         std::vector<int> friends = _users[front].return_friends();
         for (int j = 0; j < friends.size(); j++) {
            int id = friends[j];
            if (_users[id].return_depth() == -1) { // unprocessed
               queue.push_back(id); // push this unprocessed friend into the queue
               set.push_back(id); // push this unprocessed friend into the set
               _users[id].set_user(depth + 1, front);
               processed++;
            }
         }
      }

      // push this disjoint set into the groups vector
      groups.push_back(set);
   }

   return groups;
}

std::vector<int> Network::suggest_friends(int who, int& score) {
   // reset every user's depth and pred value back to -1
   for (int i = 0; i < _users.size(); i++) {
      _users[i].set_user(-1, -1);
   }

   std::deque<int> queue;
   std::vector<int> suggested;
   int from = who;
   queue.push_back(from); // start the queue with this user
   _users[from].set_user(0,-1);
   std::vector<int> friended = _users[who].return_friends();

   // find all potential friends (depth == 2)
   while (_users[queue.front()].return_depth() < 2) {
      int front = queue.front();
      int depth = _users[front].return_depth();
      queue.pop_front();

      // find all unprocessed friends
      std::vector<int> friends = _users[front].return_friends();
      for (int j = 0; j < friends.size(); j++) {
         int id = friends[j];
         if (_users[id].return_depth() == -1) {
            queue.push_back(id);
            _users[id].set_user(depth + 1, front);
         }
      }
   }

   // the number of potential friends
   int size = queue.size();

   // find the score for each potential friends
   for (int k = 0; k < size; k++) {
      int id = queue[k];
      std::vector<int> friends = _users[id].return_friends();
      for (int l = 0; l < friends.size(); l++) {
         int friend_id = friends[l];
         for (int n = 0; n < friended.size(); n++) {
            int friended_id = friended[n];
            if (friended_id == friend_id) {
               _users[id].inc_score();
            }
         }
      }
   }

   // find the highest score
   for (int p = 0; p < size; p++) {
      int id = queue[p];
      if (_users[id].return_score() > score) {
         score = _users[id].return_score();
      }
   }

   // add people with the highesr score into the suggested vector
   for (int q = 0; q < size; q++) {
      int id = queue[q];
      if (_users[id].return_score() == score) {
         suggested.push_back(_users[id].return_id());
      }
   }

   return suggested;
}

int Network::return_size() {
   int size = _users.size();
   return size;
}

int Network::return_id(int i) {
   int id = _users[i].return_id();
   return id;
}

std::string Network::return_name(int i) {
   std::string name = _users[i].return_name();
   return name;
}

int Network::return_year(int i) {
   int year = _users[i].return_year();
   return year;
}

int Network::return_zip(int i) {
   int zip = _users[i].return_zip();
   return zip;
}

std::vector<int> Network::return_friends(int i) {
   std::vector<int> friends = _users[i].return_friends();
   return friends;
}