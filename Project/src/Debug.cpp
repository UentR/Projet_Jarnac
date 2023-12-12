#include "Debug.hpp"

char* envDebugLevel = getenv("DEBUG");
bool Empty = envDebugLevel == NULL;
const DebugLevel LEVEL = (Empty) ? (DebugLevel)0 : (DebugLevel)atoi((envDebugLevel));

// Use this function to log most commun words
void writeToDebugFile(string logInfo, int level) {
  if (level > LEVEL) {
    return;
  }
  ofstream debugFile;
  debugFile.open("Debug.txt", ios::app);
  time_t now = time(0);
  if (debugFile.is_open()) {
    char Date[20];
    char Heure[20];
    strftime(Date, sizeof(Date), "%d/%m/%Y", localtime(&now));
    strftime(Heure, sizeof(Heure), "%H:%M:%S", localtime(&now));
    debugFile << Date << " : " << Heure << " : " << logInfo << endl;
    debugFile.close();
  } else {
    cout << "Unable to open debug file." << endl;
  }
}

void writeToStatsFile(string logInfo) {
  ofstream debugFile;
  debugFile.open("Text/WordsIA.txt", ios::app);
  time_t now = time(0);
  if (debugFile.is_open()) {
    debugFile << logInfo << endl;
    debugFile.close();
  } else {
    cout << "Unable to open debug file." << endl;
  }
}

void flushDebug() {
  ofstream debugFile;
  debugFile.open("Debug.txt", ios::trunc);
  debugFile.close();
}

// int main() {
//   cout << "Hello World" << endl;
//   writeToDebugFile("Warning", 1);
//   writeToDebugFile("Info", 2);
//   writeToDebugFile("Error", 3);
//   writeToDebugFile("All Log", 4);
//   return 0;
// }