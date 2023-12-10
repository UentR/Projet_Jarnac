#include "Debug.hpp"

// Use this function to log most commun words
void writeToDebugFile(string logInfo) {
  ofstream debugFile;
  debugFile.open("Text/WordsIA.txt", ios::app);
  time_t now = time(0);
  if (debugFile.is_open()) {
    // char Date[20];
    // char Heure[20];
    // strftime(Date, sizeof(Date), "%d/%m/%Y", localtime(&now));
    // strftime(Heure, sizeof(Heure), "%H:%M:%S", localtime(&now));
    // debugFile << Date << " : " << Heure << " : " << logInfo << endl;
    debugFile << logInfo << endl;
    debugFile.close();
  } else {
    cout << "Unable to open debug file." << endl;
  }
}