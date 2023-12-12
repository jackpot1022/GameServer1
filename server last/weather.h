#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define APPLE 0
#define PEACH 1
#define SHINE 2
#define STRAW 3
#define MUSHROOM 4
#define RICE 5
#define CORN 6
#define WHEAT 7
#define BEAN 8
#define PEPPER 9
#define SWEET 10
#define POTATO 11
#define SANSAM 12
#define CHICK 13

#define SUNNY 0
#define CLOUDY 1
#define RAINY 2
#define SNOWY 3

typedef struct Price {
    int ProductPrice[14];
    int SeedPrice[14];
} Price;

void Price_Show(Price price);
void Price_Init(Price *price);
void Price_Update(Price *price, int weather);

void Weather_Simulate(int *weather, int day);
void printNews(int weather);
