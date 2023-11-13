#ifndef __CACHECDN_H__
#define __CACHECDN_H__

/* Add any includes here! */
#include <map>
#include <string>

using std::map;

namespace ICS45C {
namespace CDN {

/* Define your class here! */
class Cache {
 public:
  // Declaring constructor of Cache class
  Cache(unsigned int freshnessCount);
  // Declaring a destructor of Cache class
  ~Cache();
  // Declaring getText method
  std::string getText(std::string filepath);
  // Declaring getBinary method
  char* getBinary(std::string filepath);
  // Declaring isCached method
  bool isCached(std::string filepath);
  // Declaring getFreshness method
  unsigned int getFreshness(std::string filepath);
  // Declaring markFileFresh method
  void markFileFresh(std::string filepath);
  // Declaring purgeCache method
  void purgeCache();
  // Declaring topFile method
  std::string topFile();
  // Declaring getStats method
  std::string getStats();

 private:
  // member freshnessCount
  unsigned int freshnessCount;
  // A map to keep track of filepath to the string file content
  map<std::string, std::string> filePathTofileContent;
  // A map to keep track of filepath to the count how many times its been called
  map<std::string, unsigned int> filePathTofileCount;
  // A map to keep track of the filepath to the character pointer of the binary
  // file
  map<std::string, char*> filePathTobinaryFileContent;
  // A map to keep track of the filepath to the number of requests
  map<std::string, int> filePathTofileRequests;
  // total requests
  unsigned int totalRequests;
  // total disk reads
  unsigned int diskReads;
};

}  // namespace CDN
}  // namespace ICS45C

#endif  // __CACHECDN_H__
