#include <fstream>
#include <iostream>

using namespace std;

void writeToDebugFile(string& logInfo) {
  ofstream debugFile;
  debugFile.open("debug.txt", ios::app);
  if (debugFile.is_open()) {
    debugFile << logInfo << endl;
    debugFile.close();
  } else {
    cout << "Unable to open debug file." << endl;
  }
}