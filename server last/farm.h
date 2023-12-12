#pragma once
#include "ProgramBase.h"
#include "constant.h"
#include "server.h"

#define MAXGROW 3

void select_farm(Player *player, int weather);
void show_seed(Player *player);
void show_boosted(Player *player);
void select_seed(Player *player);
void plant_seed(Player *player, int seed_type);
void select_boosted(Player *player);
void get_crop(Player *player, int weather);
int random_weather(int weather);
void show_filed(Player *player);
void init_filled(Player *player, int index);
