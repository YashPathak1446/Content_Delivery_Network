#include "cacheCDN.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::getline;
using std::ifstream;
using std::stringstream;

/* Implement any functions and methods here! */
namespace ICS45C {
namespace CDN {

// Check if file is binary or normal text file
// https://stackoverflow.com/a/277538
bool checkBinary(std::string filepath) {
  bool isBinary;
  int bytes;
  std::ifstream inFile(filepath);
  while ((bytes = inFile.get()) != EOF && bytes <= 127)
    ;
  if (bytes == EOF)
    isBinary = false;
  else
    isBinary = true;
  return isBinary;
}

// Implementing the constructor of class Cache:
Cache::Cache(unsigned int freshnessCount)
    : freshnessCount(freshnessCount), totalRequests(0), diskReads(0) {}

// Implementing the destructor of class Cache:
Cache::~Cache() {
  for (auto filePath : filePathTobinaryFileContent) {
    delete[] filePath.second;
  }
  filePathTobinaryFileContent.clear();
}

// Implementing the getText method of Cache Class:
std::string Cache::getText(std::string filepath) {
  // Check if filepath is in map. If not, add read the file and add it to map
  // corrresponding to the filepath. Also set the count for that filepath being
  // called to 0.
  if (filePathTofileContent.find(filepath) == filePathTofileContent.end()) {
    ifstream inFile;
    std::string text;
    inFile.open(filepath);
    std::string fileContent = "";
    while (getline(inFile, text)) {
      fileContent += text + '\n';
    }
    if (filePathTofileRequests.find(filepath) == filePathTofileRequests.end())
      filePathTofileRequests[filepath] = 0;
    filePathTofileContent[filepath] = fileContent;
    filePathTofileCount[filepath] = 0;
    inFile.close();
    diskReads++;
  }
  // Next, after creating the filepath (or if it already exists:) just increase
  // the count getText has been used on it, and return the fileContent.
  if (filePathTofileCount[filepath] < freshnessCount) {
    totalRequests++;
    filePathTofileRequests[filepath] += 1;
    filePathTofileCount[filepath] += 1;
    return filePathTofileContent[filepath];
  }
  // Finally, if the count is = to freshnessCount, then redefine the filepath
  // and the fileCount and reRead it with maybe new altered text in the file.
  else {
    filePathTofileRequests[filepath] += 1;
    filePathTofileCount[filepath] = 1;
    totalRequests++;
    diskReads++;
    filePathTofileContent[filepath] = "";
    ifstream inFile;
    std::string text;
    inFile.open(filepath);
    std::string fileContent = "";
    while (getline(inFile, text)) {
      fileContent += text + '\n';
    }
    filePathTofileContent[filepath] = fileContent;
    inFile.close();
    return filePathTofileContent[filepath];
  }
}

// Implementing the getBinary method of Cache Class
char* Cache::getBinary(std::string filepath) {
  if (filePathTobinaryFileContent.find(filepath) ==
          filePathTobinaryFileContent.end() ||
      freshnessCount <= 1) {
    ifstream inBinFile(filepath,
                       std::ios::in | std::ios::binary | std::ios::ate);
    std::streampos size;
    char* memblock;
    diskReads++;
    if (inBinFile.is_open()) {
      inBinFile.seekg(0, std::ios::end);
      size = inBinFile.tellg();
      inBinFile.seekg(0, std::ios::beg);
      memblock = new char[size];
      inBinFile.read(memblock, size);
      filePathTobinaryFileContent[filepath] = memblock;
      filePathTofileCount[filepath] = 0;
      filePathTofileRequests[filepath] = 0;
      inBinFile.close();
    } else {
      std::cerr << "Unable to open file";
      exit(1);
    }
  }
  // Next, after creating the filepath (or if it already exists:) just
  // increase the count getBinary has been used on it, and return the
  // address of the binary file.
  if (filePathTofileCount[filepath] < freshnessCount) {
    totalRequests++;
    filePathTofileRequests[filepath] += 1;
    filePathTofileCount[filepath] += 1;
    return filePathTobinaryFileContent[filepath];
  }
  // Finally, if the count is = to freshnessCount, then redefine the filepath
  // and the fileCount and reRead it with maybe new altered text in the file.
  else {
    totalRequests++;
    diskReads++;
    filePathTofileCount[filepath] = 1;
    filePathTofileRequests[filepath] += 1;
    delete[] filePathTobinaryFileContent[filepath];
    ifstream inBinFile(filepath,
                       std::ios::in | std::ios::binary | std::ios::ate);
    std::streampos size;
    char* memblock;
    if (inBinFile.is_open()) {
      inBinFile.seekg(0, std::ios::end);
      size = inBinFile.tellg();
      inBinFile.seekg(0, std::ios::beg);
      memblock = new char[size];
      while (inBinFile.tellg() < size) {
        inBinFile.read(memblock, size);
      }
      filePathTobinaryFileContent[filepath] = memblock;
      inBinFile.close();
    } else {
      std::cerr << "Unable to open file";
      exit(1);
    }
    return filePathTobinaryFileContent[filepath];
  }
}

// Implementing the isCached method of Cache Class:
bool Cache::isCached(std::string filepath) {
  // Check if file is in binary File content map
  bool isBinary = checkBinary(filepath);
  // If it is a binary file
  if (isBinary) {
    return (filePathTobinaryFileContent.find(filepath) ==
                    filePathTobinaryFileContent.end()
                ? false
                : true);
  }
  // else if it is a normal text file
  else {
    return (filePathTofileContent.find(filepath) == filePathTofileContent.end()
                ? false
                : true);
  }
}

// Implementing the getFreshness method of Cache Class
unsigned int Cache::getFreshness(std::string filepath) {
  bool isBinary = checkBinary(filepath);
  // if it is a binary file
  if (isBinary) {
    // When a file has never been requested
    if (filePathTobinaryFileContent.find(filepath) ==
        filePathTobinaryFileContent.end())
      return 0;
    // Only possible when freshnessCount == 0 or freshnessCount == 1
    if (freshnessCount < filePathTofileCount[filepath]) return 0;
    // All other cases
    return (freshnessCount - filePathTofileCount[filepath]);
  }
  // if it is a normal text file
  else {
    // When a file has never been requested
    if (filePathTofileContent.find(filepath) == filePathTofileContent.end())
      return 0;
    // Only possible when freshnessCount == 0 or freshnessCount == 1
    if (freshnessCount < filePathTofileCount[filepath]) return 0;
    // All other cases
    return (freshnessCount - filePathTofileCount[filepath]);
  }
}

// Implementing the markFileFresh method of Cache Class
void Cache::markFileFresh(std::string filepath) {
  bool isBinary = checkBinary(filepath);
  // if it is a binary file
  if (isBinary) {
    // When a binary file is not stored
    if (filePathTobinaryFileContent.find(filepath) ==
        filePathTobinaryFileContent.end())
      return;
    // In other cases
    filePathTofileCount[filepath] = 0;
    return;
  }
  // if it is a normal text/ascii file
  else {
    // When an ascii file is not stored
    if (filePathTofileContent.find(filepath) == filePathTofileContent.end())
      return;
    // In other cases
    filePathTofileCount[filepath] = 0;
    return;
  }
}

// Implementing the purgeCache method of Cache Class
void Cache::purgeCache() {
  // Reference to clear method on map objects:
  // https://www.geeksforgeeks.org/mapclear-c-stl/#:~:text=clear()%20function%20is%20used,the%20name%20of%20the%20map.
  filePathTofileContent.clear();
  filePathTobinaryFileContent.clear();
  filePathTofileCount.clear();
  return;
}

// Implementing the topFile method of Cache Class
std::string Cache::topFile() {
  std::string topFilePath = "";
  if (filePathTofileRequests.empty()) return topFilePath;
  int max = 0;
  for (auto filePath : filePathTofileRequests) {
    if (filePath.second > max) {
      max = filePath.second;
      topFilePath = filePath.first;
    }
  }
  return topFilePath;
}

// Implementing the getStats method of Cache Class
std::string Cache::getStats() {
  std::string stats = "Cache Stats\n";
  stats += "----------\n";
  stringstream totalRequestsString;
  totalRequestsString << totalRequests;
  stats += "Total requests: " + totalRequestsString.str() + '\n';

  int totalFilesRequestedInt = 0;
  totalFilesRequestedInt = filePathTofileRequests.size();
  stringstream totalFilesRequested;
  totalFilesRequested << totalFilesRequestedInt;
  stats += "Total files requested: " + totalFilesRequested.str() + '\n';

  stringstream averageRequests;
  if (totalFilesRequestedInt == 0)
    stats += "Average requests per file: 0.00\n";
  else {
    stringstream averageRequests;
    averageRequests << std::fixed << std::setprecision(2)
                    << (double)totalRequests / totalFilesRequestedInt;
    stats += "Average requests per file: " + averageRequests.str() + '\n';
  }

  std::string topFileName = Cache::topFile();
  int topFileCountInt = filePathTofileRequests[topFileName];
  stringstream topFileCount;
  topFileCount << topFileCountInt;
  stats +=
      "Top file: " + topFileName + " (" + topFileCount.str() + " requests)\n";

  stringstream totalDiskReads;
  totalDiskReads << diskReads;
  stats += "Total times read from disk: " + totalDiskReads.str() + '\n';

  stats += "----------\n";
  return stats;
}

}  // namespace CDN
}  // namespace ICS45C