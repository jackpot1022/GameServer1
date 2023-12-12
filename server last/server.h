// server.h
#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include "weather.h"

#define MAXLINE 256
#define MAX_PLAYERS 3
#define MAXDAY 30

#define server_key 34
#define first_player 1

typedef struct CropSystem {
    int isFilled;
    int dayPlanting;
} CropSystem;

typedef struct Farm {
    CropSystem cropSystem[30];
    bool isBoosted[30];
}Farm;

typedef struct Inventory {
    int seed[14];
    int crop[14];
    int booster;
}Inventory;

typedef struct Player {
    char name[MAXLINE];
    Inventory inventory;
    Farm farm;
    int money;
	bool isActive;
    bool isEnd;
    int pid;
}Player;

struct Server {
    Price price;
    bool isGameStart;
    int day;
    int weather;
    int playerCount;
    pid_t server_pid;
};

#endif

