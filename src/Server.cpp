#include <sys/shm.h>

#include <iostream>

using namespace std;
#include <unistd.h>

#include "Struct.hpp"
struct item;

int main() {
  int PlayStruct = shmget(0x1234, sizeof(item), 0666 | IPC_CREAT);
  int ShouldPlay = shmget(0x1235, sizeof(bool), 0666 | IPC_CREAT);
  item* p = (item*)shmat(PlayStruct, 0, 0);
  p->Played = false;
  p->PlayerID = 1;

  cout << "Player 1: " << p->Played << endl;
  cout << "Player ID " << p->PlayerID << endl;

  bool* s = (bool*)shmat(ShouldPlay, 0, 0);

  cout << "Waiting for player 2..." << endl;

  *s = true;

  while ((*s)) {
    // Wait for the server to set the flag
    usleep(100000);
  }
  cout << "Player 1: " << p->Played << endl;
  cout << "Player ID " << p->PlayerID << endl;

  return 0;
}