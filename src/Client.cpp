#include <sys/shm.h>

#include <iostream>

using namespace std;

#include <unistd.h>

#include "Struct.hpp"
struct item;

int main() {
  int PlayStruct = shmget(0x1234, sizeof(item), 0666 | IPC_CREAT);
  int ShouldPlay = shmget(0x1235, sizeof(bool), 0666 | IPC_CREAT);
  item* p = (item*)shmat(PlayStruct, NULL, 0);
  bool* s = (bool*)shmat(ShouldPlay, NULL, 0);

  cout << *s << endl;

  while (!(*s)) {
    // Wait for the server to set the flag
    usleep(100000);
  }
  p->Played = true;
  p->PlayerID = 198765;

  cout << "Player 1: " << p->Played << endl;
  cout << "Player ID " << p->PlayerID << endl;

  *s = false;

  return 0;
}