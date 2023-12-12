#include "weather.h"

void Weather_Simulate(int *weather, int day) 
{
    if(((day % 4) + 1) != 1 && ((day % 4) + 1) != 4)
    {
        *weather = ((rand() % 2) + 1);
    }
    else
    {
        *weather = rand() % 4;
    }
}

void printNews(int weather)
{
    switch (weather) {
            case SUNNY:
                printf("앵커: 오늘은 날씨가 매우 맑군요!\n");
                printf("앵커: 농사가 잘 될 것 같은 날입니다.\n");
                break;
            case CLOUDY:
                printf("앵커: 오늘은 구름이 꽤 많아 해가 잘 보이지 않습니다.\n");
                printf("앵커: 이럴때일수록 더 힘을 내야겠죠!.\n");
                break;
            case RAINY:
                printf("앵커: 현재 국내에는 비구름이 가득합니다.\n");
                printf("앵커: 우산을 꼭 챙기고 다니세요.\n");
                break;
            case SNOWY:
                printf("앵커: 오늘은 눈이 내리고 있습니다.\n");
                printf("앵커: 농작물에 피해가 가지 않게 조심하시기 바랍니다.\n");
                break;
            default:
                break;
        }
}

void Price_Show(Price price) {
    const char *crop_names[14] = {
        "사과", "복숭아", "샤인머스켓", "딸기", "버섯", "쌀", "옥수수", "밀", "콩", "고추", "고구마", "감자", "산삼", "칡"
    };
    printf("앵커: 씨앗의 가격입니다.\n");
    for (int i = 0; i < 14; i++) {
        printf("%s: %d원\n", crop_names[i], price.SeedPrice[i]);
    }
    printf("\n");
    printf("앵커: 농작물의 가격입니다.\n");
    for (int i = 0; i < 14; i++) {
        printf("%s: %d원\n", crop_names[i], price.ProductPrice[i]);
    }
    printf("\n");
}

void Price_Init(Price *price) {
	price->SeedPrice[APPLE] = rand() % (201) + 500;
	price->SeedPrice[PEACH] = rand() % (501) + 1000;
	price->SeedPrice[SHINE] = rand() % (1001) + 1500;
	price->SeedPrice[STRAW] = rand() % (301) + 300;
	price->SeedPrice[MUSHROOM] = rand() % (101) + 200;
	price->SeedPrice[RICE] = rand() % (101) + 200;
	price->SeedPrice[CORN] = rand() % (51) + 150;
	price->SeedPrice[WHEAT] = rand() % (51) + 100;
	price->SeedPrice[BEAN] = rand() % (101) + 50;
	price->SeedPrice[PEPPER] = rand() % (101) + 200;
	price->SeedPrice[SWEET] = rand() % (301) + 200;
	price->SeedPrice[POTATO] = rand() % (161) + 140;
	price->SeedPrice[SANSAM] = rand() % (201) + 500;
	price->SeedPrice[CHICK] = rand() % (501) + 2500;


    price->ProductPrice[APPLE] = rand() % (2001) + 5000;
	price->ProductPrice[PEACH] = rand() % (5001) + 10000;
	price->ProductPrice[SHINE] = rand() % (10001) + 15000;
	price->ProductPrice[STRAW] = rand() % (3001) + 3000;
	price->ProductPrice[MUSHROOM] = rand() % (1001) + 2000;
	price->ProductPrice[RICE] = rand() % (1001) + 2000;
	price->ProductPrice[CORN] = rand() % (501) + 1500;
	price->ProductPrice[WHEAT] = rand() % (501) + 1000;
	price->ProductPrice[BEAN] = rand() % (1001) + 500;
	price->ProductPrice[PEPPER] = rand() % (1001) + 2000;
	price->ProductPrice[SWEET] = rand() % (3001) + 2000;
	price->ProductPrice[POTATO] = rand() % (1601) + 1400;
	price->ProductPrice[SANSAM] = rand() % (5001) + 25000;
	price->ProductPrice[CHICK] = rand() % (10001) + 10000;	
}

void Price_Update(Price *price, int weather) {
    int randomseed;
    int randomprod;

    for (int i = 0; i < 14; i++) {
        randomseed = rand() % (61) - 30;
        price->SeedPrice[i] += randomseed;
        if (price->SeedPrice[i] < 0) {
            price->SeedPrice[i] = rand() % (101) + 50;
        }
    }

    for (int i = 0; i < 14; i++) {
        randomprod = rand() % (601) - 300;
        price->ProductPrice[i] += randomprod;

        switch (weather) {
            case SUNNY:
                price->ProductPrice[i] += 100;
                break;
            case CLOUDY:
                price->ProductPrice[i] += 50;
                break;
            case RAINY:
                price->ProductPrice[i] -= 50;
                break;
            case SNOWY:
                price->ProductPrice[i] -= 100;
                break;
            default:
                break;
        }

        if (price->ProductPrice[i] < 0) {
            price->ProductPrice[i] = rand() % (1001) + 500;
        }
    }
}

