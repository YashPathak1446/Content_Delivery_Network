#include <fstream>
#include <string>

using std::ofstream;
using std::string;

int main() {
  string expected1 = "Hello!\n123\n45.\n33";
  string expected2 = "Let's call it a tie";
  string expected3 =
      "What...is the airspeed velocity of an unladen swallow?\nWhat do you "
      "mean, "
      "an African or European swallow?\nHuh? What?  I...I don't know that...";

  ofstream outFile;
  outFile.open("test1.txt");
  outFile << expected1;
  outFile.close();

  outFile.open("test2.txt");
  outFile << expected2;
  outFile.close();

  outFile.open("test3.txt");
  outFile << expected3;
  outFile.close();
  return 0;
}