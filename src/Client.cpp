#include <sys/shm.h>

#include <iostream>

using namespace std;

#include <unistd.h>

#include "Struct.hpp"
struct item;

int main() {
  int PlayStruct = shmget(0x1234, sizeof(item), 0660 | IPC_EXCL);
  int ShouldPlay = shmget(0x1235, sizeof(bool), 0660 | IPC_EXCL);

  if ((PlayStruct < 0) || (ShouldPlay < 0)) {
    cout << "Error: Shared memory segment not found." << endl;
    return 1;
  }

  item* p = (item*)shmat(PlayStruct, 0, 0);
  bool* s = (bool*)shmat(ShouldPlay, 0, 0);

  cout << *s << endl;

  while (!(*s)) {
    // Wait for the server to set the flag
    usleep(100000);
  }
  p->Played = true;
  int T;
  cout << "Enter a number: ";
  cin >> T;
  p->PlayerID = T;

  cout << "Player 1: " << p->Played << endl;
  cout << "Player ID " << p->PlayerID << endl;

  *s = false;

  return 0;
}