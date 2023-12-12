#include "farm.h"

void select_farm(Player *player, int weather)
{
    int input;
    while (true)
    {
        printf("농장에 오신걸 환영합니다\n");
        printf("다음 중 선택지를 골라주세요\n");
        printf("[1] 소지품 씨앗 확인\n");
        printf("[2] 소지품 비료 확인\n");
        printf("[3] 씨앗 심기\n");
        printf("[4] 비료 주기\n");
        printf("[5] 농작물 수확하기\n");
        printf("[6] 농장 현황보기\n");
        printf("[7] 나가기\n");

        scanf("%d", &input);
        switch (input)
        {
        case 1:
            show_seed(player);
            break;
        case 2:
            show_boosted(player);
            break;
        case 3:
            select_seed(player);
            break;
        case 4:
            select_boosted(player);
            break;
        case 5:
            get_crop(player, weather);
            break;
        case 6:
            printf("\033[H\033[J");
            show_filed(player);
            break;
        case 7:
            printf("농장에서 나갑니다.\n");
            return;
        default:
            printf("다시 입력해주세요\n");
        }
    }
}

void show_seed(Player *player)
{
    printf("플레이어 %s님의 전체 씨앗 소지 현황입니다\n", player->name);
    printf("1.사과씨앗: %d\n", player->inventory.seed[APPLE]);
    printf("2.복숭아씨앗: %d\n", player->inventory.seed[PEACH]);
    printf("3.샤인머스캣씨앗: %d\n", player->inventory.seed[SHINE]);
    printf("4.딸기씨앗: %d\n", player->inventory.seed[STRAW]);
    printf("5.버섯씨앗: %d\n", player->inventory.seed[MUSHROOM]);
    printf("6.쌀씨앗: %d\n", player->inventory.seed[RICE]);
    printf("7.옥수수씨앗: %d\n", player->inventory.seed[CORN]);
    printf("8.밀씨앗: %d\n", player->inventory.seed[WHEAT]);
    printf("9.콩씨앗: %d\n", player->inventory.seed[BEAN]);
    printf("10.고추씨앗: %d\n", player->inventory.seed[PEPPER]);
    printf("11.고구마씨앗: %d\n", player->inventory.seed[SWEET]);
    printf("12.감자씨앗: %d\n", player->inventory.seed[POTATO]);
    printf("13.산삼씨앗: %d\n", player->inventory.seed[SANSAM]);
    printf("14.칡씨앗: %d\n", player->inventory.seed[CHICK]);
    printf("\n");
}

void show_boosted(Player *player)
{
    printf("플레이어 %s님의 전체 비료 소지 현황입니다\n", player->name);
    printf("비료: %d개\n", player->inventory.booster);
}

void select_seed(Player *player)
{
    printf("\033[H\033[J");
    show_seed(player);
    show_boosted(player);
        int select_num;
            while (true)
            {
                printf("씨앗 심기를 선택하셨습니다. 어떤 씨앗을 뿌리실 건가요(숫자로): ");
                scanf("%d", &select_num);
                if (select_num >= 1 || select_num <= 14)
                {
                    plant_seed(player, select_num - 1);
                    break;
                }
                else
                {
                    printf("올바르지 않은 씨앗 종류입니다. 다시 입력해주세요.\n");
                }
            }
}

// 완료
void plant_seed(Player *player, int seed_type)
{
    if (player->inventory.seed[seed_type] <= 0)
    {
        printf("해당 씨앗이 없습니다.\n");
        return;
    }

    int index;
    printf("\033[H\033[J");
    show_filed(player);
    while (true)
    {
        printf("씨앗을 심을 위치를 입력하세요(-1을 입력하여 나가기): ");
        scanf("%d", &index);
        if (index >= 1 || index <= 30)
        {
            if ((player->farm.cropSystem[index - 1].isFilled > -1) && (player->farm.cropSystem[index - 1].isFilled < 14))
            {
                printf("이미 농작물이 심어져있는 구역입니다\n");
            }
            else
            {
                player->farm.cropSystem[index - 1].isFilled = seed_type;
                player->inventory.seed[seed_type]--;
            }
        }
        else if (index == -1)
        {
            printf("이전으로 돌아갑니다.\n");
            return;
        }
        else
        {
            printf("올바르지 않은 위치입니다. 다시 입력해주세요.\n");
        }
    }
}

// 완료
void select_boosted(Player *player)
{
    if (player->inventory.booster <= 0)
    {
        printf("비료가 없습니다.\n");
        return;
    }

    int index;
    printf("\033[H\033[J");
    show_filed(player);

    while (true)
    {
        printf("비료를 뿌릴 위치를 입력하세요(-1을 입력하여 나가기): ");
        scanf("%d", &index);
        if (index >= 1 || index <= 30)
        {
            if (player->farm.isBoosted[index - 1])
            {
                printf("이미 비료가 뿌려져있는 구역입니다\n");
            }
            else
            {
                printf("%d번 구역에 비료를 뿌렸습니다.\n", index);
                player->farm.isBoosted[index - 1] = true;
                player->inventory.booster--;
            }
        }
        else if (index == -1)
        {
            printf("이전으로 돌아갑니다.\n");
            return;
        }
        else
        {
            printf("올바르지 않은 위치입니다. 다시 입력해주세요.\n");
        }
    }
}

// 완료
void get_crop(Player *player, int weather)
{
    int count = 0;
    for (int i = 0; i < 30; i++)
    {
        if (player->farm.cropSystem[i].isFilled > -1)
        {
            count++;
        }
    }
    if (count == 0)
    {
        printf("심겨진 작물이 없습니다.\n");
        return;
    }

    show_filed(player);
    printf("\033[H\033[J");
    char input;
    while (true)
    {
        printf("전체 수확을 하시겠습니까? Y/N : ");
        scanf(" %c", &input);
        if (input == 'Y' || input == 'y')
        {
            for (int i = 0; i < 30; i++)
            {
                if (player->farm.cropSystem[i].dayPlanting > MAXGROW)
                {
                    player->inventory.crop[player->farm.cropSystem[i].isFilled] += (random_weather(weather) + ((rand() % 2) + 1));
                    init_filled(player, i);
                    printf("수확을 완료했습니다.\n");
                    break;
                }
            }
        }
        else if (input == 'N' || input == 'n')
        {
            printf("수확을 하지 않고 돌아갑니다\n");
            break;
        }
        else
        {
            printf("다시 입력해주세요\n");
        }
    }
}

// 날씨함수확인
int random_weather(int weather)
{
    int num;
    switch (weather)
    {
    case 0:
        num = (rand() % 2);
        return num;
        break;
    case 3:
        num = (rand() % 2);
        if (num == 0)
        {
            return 0;
        }
        else
        {
            return -num;
        }
    }
}

void show_filed(Player *player)
{
    printf("비어있음 : 0, 사과 : 1, 복숭아 : 2, 샤인머스켓 : 3, 딸기 : 4, 버섯 : 5, 쌀 : 6, 옥수수 : 7, 밀 : 8, 콩 : 9, 고추 : 10, 고구마 : 11, 감자 : 12, 산삼 : 13, 칡 : 14\n");
    printf("---씨앗이 심어진 현황---\n");
    for (int i = 0; i < 30; i++)
    {
        printf("|%d", player->farm.cropSystem[i].isFilled + 1);
    }
    printf("|\n");
    printf("비어있음 : 0, 뿌려짐 : 1\n");
    printf("---비료가 뿌려진 현황---\n");
    for (int i = 0; i < 30; i++)
    {
        printf("|%d", player->farm.isBoosted[i]);
    }
    printf("|\n");
}

// 수확시 해당 인덱스 초기화
void init_filled(Player *player, int index)
{
    player->farm.isBoosted[index] = false;
    player->farm.cropSystem[index].isFilled = -1;
    player->farm.cropSystem[index].dayPlanting = 0;
}