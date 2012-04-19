#include <iostream>
#include <boost/concept_check.hpp>
#include "../util.h"

using namespace std;

int main() {
   vector <string> substr = split("LoL haha", " ");
   vector <string> s2 = split("ip:port", ":");
   for (unsigned int i = 0; i < substr.size(); i++)
      cout << substr[i] << "\t" << s2[i] << endl;
   cout << "\n";
   return 0;
}