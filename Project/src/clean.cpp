#include <sys/shm.h>

#include <iostream>

struct Keys {
  int keyPlay[2];
  int keyExchange[2];
  int keyJarnac;
  int keyLetters;
  int keyPlayMove;
  int keyConnected[2];
  int keyBoard;
};

int main(int argc, char const *argv[]) {
  int keyAdrr = atoi(argv[1]);
  Keys *key = (Keys *)shmat(keyAdrr, NULL, 0);
  shmctl(keyAdrr, IPC_RMID, 0);
  shmctl(key->keyPlay[0], IPC_RMID, 0);
  shmctl(key->keyPlay[1], IPC_RMID, 0);
  shmctl(key->keyExchange[0], IPC_RMID, 0);
  shmctl(key->keyExchange[1], IPC_RMID, 0);
  shmctl(key->keyJarnac, IPC_RMID, 0);
  shmctl(key->keyLetters, IPC_RMID, 0);
  shmctl(key->keyPlayMove, IPC_RMID, 0);
  shmctl(key->keyConnected[0], IPC_RMID, 0);
  shmctl(key->keyConnected[1], IPC_RMID, 0);
  shmctl(key->keyBoard, IPC_RMID, 0);
}