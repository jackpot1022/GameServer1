#include "trade.h"

void trade(Player *players, Price price)
{
    int playerInput;
    while (true)
    {
        printf("종업원: 거래소에 오신것을 환영합니다.\n");
        printf("종업원: 여기서는 씨앗, 비료를 사거나 재배된 농작물을 팔 수 있습니다.\n");
        printf("종업원: 무엇을 도와드릴까요?\n");
        printf("[1] 씨앗 구매\n");
        printf("[2] 비료 구매\n");
        printf("[3] 작물 판매\n");
        printf("[4] 나가기\n");
        scanf("%d", &playerInput);

        switch (playerInput)
        {
        case 1:
            purchaseSeed(players, price);
            printf("종업원: 거래 감사합니다!\n");
            printf("%s님의 잔액: %d\n", players->name, players->money);
            break;
        case 2:
            purchaseBooster(players);
            printf("종업원: 거래 감사합니다!\n");
            printf("%s님의 잔액: %d\n", players->name, players->money);
            break;
        case 3:
            sellProduct(players, price);
            printf("종업원: 거래 감사합니다!\n");
            printf("%s님의 잔액: %d\n", players->name, players->money);
            break;
        case 4:
            printf("거래를 마치고 돌아갑니다.\n");
            return;
        }
    }
}

void sellProduct(Player *player, Price price)
{
    int crop;       // 판매할 작물 번호
    int cropNumber; // 작물 개수

    while (true)
    {
        printf("사과:0, 복숭아:1, 샤인머스켓:2, 딸기:3, 버섯:4, 쌀:5, 옥수수:6, 밀:7, 콩:8, 고추:9, 고구마:10, 감자:11, 산삼:12, 칡:13(-1을 입력하여 나가기)\n");
        printf("판매할 농작물 종류를 숫자로 입력하세요: \n");
        scanf("%d", &crop);
        if (crop >= 0 || crop <= 13)
        {
            printf("판매할 농작물의 개수를 입력하세요: \n");
            scanf("%d", &cropNumber);
            if (player->inventory.crop[crop] < cropNumber)
            {
                printf("농작물이 부족합니다\n");
            }
            else
            {
                printf("판매를 완료했습니다.\n");
                player->money += price.ProductPrice[crop] * cropNumber;
                player->inventory.crop[crop] -= cropNumber;
            }
        }
        else if (crop == -1)
        {
            printf("판매를 그만둡니다\n");
            return;
        }
        else
        {
            printf("올바르지 않은 농작물 종류입니다. 다시 입력해주세요.\n");
        }
    }
}

void purchaseSeed(Player *player, Price price)
{
    int crop;       // 구매할 씨앗 번호
    int seedNumber; // 씨앗 개수

    while (true)
    {
        printf("사과:0, 복숭아:1, 샤인머스켓:2, 딸기:3, 버섯:4, 쌀:5, 옥수수:6, 밀:7, 콩:8, 고추:9, 고구마:10, 감자:11, 산삼:12, 칡:13(-1을 입력하여 나가기)\n");
        printf("구매할 씨앗 종류를 숫자로 입력하세요: \n");
        scanf("%d", &crop);
        if (crop >= 0 || crop <= 13)
        {
            printf("씨앗 구매 개수를 입력하세요: \n");
            scanf("%d", &seedNumber);
            if (player->money < price.SeedPrice[crop] * seedNumber)
            {
                printf("잔액이 부족합니다\n");
            }
            else
            {
                printf("구매를 완료했습니다.\n");
                player->money -= price.SeedPrice[crop] * seedNumber;
                player->inventory.seed[crop] += seedNumber;
            }
        }
        else if (crop == -1)
        {
            printf("구매를 그만둡니다\n");
            return;
        }
        else
        {
            printf("올바르지 않은 씨앗 종류입니다. 다시 입력해주세요.\n");
        }
    }
}

void purchaseBooster(Player *player)
{
    int input;
    printf("비료를 몇개 구매하시겠습니까?\n");
    scanf("%d", &input);
    if (player->money < FERTILIZER_PRICE * input)
    {
        printf("잔액이 부족하여 비료를 구매할 수 없습니다. \n");
    }
    else
    {
        printf("비료를 %d개 구매했습니다.\n", input);
        player->money -= FERTILIZER_PRICE * input;
        player->inventory.booster += input;
        printf("현재 보유한 비료는 %d개 입니다.\n", player->inventory.booster);
    }
}
