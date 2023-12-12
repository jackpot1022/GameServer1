#include "server.h"
#include "trade.h"
#include "farm.h"
#include "weather.h"

int num;
int nowIndex;

struct Player *sharedMemory[MAX_PLAYERS];
struct Server *serverData;

void select_conversation();
// 게임시작시그널받음
void handleStartGame(int signum);
// 게임엔딩받는시그널
void handleEndGame(int signum);
// 플레이어리스트시그널
void handlePlayerList(int signum);
// 준비현황 출력하는 시그널
void printReady(int signum);

void bubble_sort(int list[], char *name[], int n);

void printRank(struct Player *playerData[], int playerCount);

int getIndex(Player *PlayerData[], int playerCount);

int main()
{

    key_t key;
    key_t key1;
    int shmid;
    int shmid1;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        key = ftok("/home/g_202010953/FARM/real", first_player + i);
        shmid = shmget(key, sizeof(struct Player) * MAX_PLAYERS, 0666 | IPC_CREAT);
        if (shmid == -1)
        {
            perror("shmget");
            exit(EXIT_FAILURE);
        }
        sharedMemory[i] = (struct Player *)shmat(shmid, NULL, 0666 | IPC_CREAT);
        if (sharedMemory[i] == (struct Player *)-1)
        {
            perror("shared_shmat");
            exit(EXIT_FAILURE);
        }
    }

    key1 = ftok("/home/g_202010953/FARM/real", server_key);

    shmid1 = shmget(key1, sizeof(struct Server), 0);
    if (shmid1 == -1)
    {
        perror("shmget1");
        exit(EXIT_FAILURE);
    }

    serverData = (struct Server *)shmat(shmid1, NULL, 0);
    if (serverData == (struct Server *)-1)
    {
        perror("server_shmat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        printf("플레이어명[%d] : %s\n", i + 1, sharedMemory[i]->name);
    }
    char playerName[MAXLINE];
    printf("플레이어명을 입력하세요: ");
    fgets(playerName, MAXLINE, stdin);
    playerName[strcspn(playerName, "\n")] = '\0';

    strcpy(sharedMemory[serverData->playerCount]->name, playerName);

    sharedMemory[serverData->playerCount]->pid = getpid();
    printf("플레이어 입장전 플레이어 수: %d\n", serverData->playerCount);
    printf("서버에 내 클라이언트 pid: %d를 저장합니다\n", sharedMemory[serverData->playerCount]->pid);

    kill(serverData->server_pid, SIGUSR1);
    signal(SIGUSR2, handlePlayerList);
    signal(SIGUSR1, handleStartGame);
    signal(SIGALRM, printReady);
    while (1)
    {
        if (serverData->day == MAXDAY)
        {
            signal(SIGUSR1, SIG_IGN);
            signal(SIGUSR2, handleEndGame);
            break;
        }
    }
    //마지막날 플레이어 행동 마무리까지 대기
    while (1)
    {
        if(sharedMemory[0]->isEnd == true && sharedMemory[1]->isEnd == true && sharedMemory[2]->isEnd == true)
        {
            break;
        }
    }
    

    // 공유메모리 해제
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        shmdt(sharedMemory[i]);
    }
    shmdt(serverData);
    return 0;
}

void select_conversation()
{
    nowIndex = getIndex(sharedMemory, serverData->playerCount);
    while (sharedMemory[nowIndex]->isActive == true)
    {
        // printf("%d", nowIndex);
        printf("오늘은 200%d년 %d분기 입니다.\n", (serverData->day / 4) + 1, (serverData->day % 4) + 1);
        printf("게임 종료까지 앞으로 %d턴 남았습니다.\n", MAXDAY - serverData->day);
        printf("------------------------------\n");
        printf("다음 중 번호를 입력하세요\n");
        printf("1. 두근두근 이장과의 대화(튜토리얼)\n");
        printf("2. 거래소 입장\n");
        printf("3. 최신 뉴스\n");
        printf("4. 플레이어 랭킹\n");
        printf("5. 농장\n");
        printf("6. 분기 넘어가기(준비)\n");
        printf("------------------------------\n");

        scanf("%d", &num);
        getchar();
        switch (num)
        {
        case 1:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            printf("이장: 허허 반갑구만. 새로운 청년이 왔구만.\n");
            printf("이장: 마을에 대해 소개해주지.\n");
            printf("이장: 먼저 나랑 대화하면서 마을에 대한 정보를 얻는 걸세\n");
            printf("이장: 그외에도 거래소에서 농사에 필요한 씨앗, 비료를 살 수 있고 다 만든 농작물을 팔 수도 있다네. 농작물은 분기별로 시세가 달라지기 때문에 사고 팔 때 잘 판단해야할걸세.\n");
            printf("이장: 또한 날씨가 농작물의 시세에 영향을 주기도 한다네.\n");
            printf("이장: 뉴스에서는 날씨 정보와 농작물 가격 등 변동된 내용을 들을 수 있네\n");
            printf("이장: 그리고 플레이어 랭킹은 참여한 농부들끼리 재화순위를 확인할 수 있고, 농장에 가면 산 씨앗과 비료를 사용해서 농사를 짓고 심은지 3분기가 지나면 수확할 수 있네. 작물의 수확량에 날씨가 영향을 끼친다네\n");
            printf("분기 넘어가기의 경우에는 모든 플레이어가 분기 넘어가기 상태가 되어있어야 다음 분기로 넘어갈 수 있다네\n");
            printf("다른 농부들을 이기고 승리하길 바라네\n");
            printf("------------------------------\n");
            break;
        case 2:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            trade(sharedMemory[nowIndex], serverData->price);
            break;
        case 3:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            printf("앵커: 오늘의 뉴스를 전해드립니다.\n");
            printf("앵커: 우선 오늘의 날씨입니다.\n");
            printNews(serverData->weather);
            printf("앵커: 다음으로는 농작물 시세입니다.\n");
            Price_Show(serverData->price);
            printf("앵커: 이상으로 뉴스를 마치겠습니다. 감사합니다.\n");
            break;
        case 4:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            printf("플레이어 랭킹을 확인합니다.\n");
            printRank(sharedMemory, serverData->playerCount);
            break;
        case 5:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            select_farm(sharedMemory[nowIndex], serverData->weather);
            break;
        case 6:
            printf("\033[H\033[J");
            printf("------------------------------\n");
            printf("이번분기의 행동을 마치고 다음 분기를 준비합니다.\n");
            printf("정말 마무리하시겠습니까? Y / N :");
            while (true)
            {
                char input = ' ';
                scanf(" %c", &input);
                getchar();
                if (input == 'Y' || input == 'y')
                {
                    printf("내일도 좋은일만 가득하길 바라겠습니다.\n");
                    sharedMemory[nowIndex]->isActive = false;
                    kill(serverData->server_pid, SIGUSR2);
                    return;
                }
                else if (input == 'N' || input == 'n')
                {
                    printf("확실하게 준비를 마치고 분기를 마무리해주세요\n");
                    break;
                }
                else if(input != ' ');
                {
                    printf("Y / N 으로 다시 입력해주세요\n");
                }
            }
            break;

        default:
            printf("다시 입력해주세요\n");
            break;
        }
    }
}

void handleStartGame(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("\033[H\033[J");
        select_conversation();
    }
}

void handleEndGame(int signum)
{
    if (signum == SIGUSR2)
    {
        printf("\033[H\033[J");
        printf("정해진 날이 모두 지났습니다.\n");
        printf("플레이어들의 랭킹.\n");
        printRank(sharedMemory, serverData->playerCount);
        sharedMemory[nowIndex]->isEnd = true;
    }
}

void printReady(int signum)
{
    if(signum == SIGALRM)
    {
        printf("**********알림**********\n");
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if(sharedMemory[i]->isActive == false)
            {
                printf("*알림* %s님께서 행동을 완료하셨습니다.\n", sharedMemory[i]->name);
            }
        }
    }
}

void handlePlayerList(int signum)
{
    printf("\033[H\033[J");
    if (signum == SIGUSR2)
    {
        printf("현재 접속한 유저 목록:\n");
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (sharedMemory[i] != NULL)
            {
                if (sharedMemory[i]->isActive == true)
                {
                    printf("플레이어[%d] : %s\n", i + 1, sharedMemory[i]->name);
                }
                else
                {
                    printf("플레이어[%d] : \n", i + 1);
                }
            }
            else
            {
                printf("플레이어[%d] : (제대로 초기화되지 않음)\n", i + 1);
            }
        }
    }
}

void bubble_sort(int list[], char *name[], int n)
{
    int i, j, temp;
    char tempstr[MAXLINE];
    for (i = n - 1; i > 0; i--)
    {
        // 0 ~ (i-1)까지 반복
        for (j = 0; j < i; j++)
        {
            // j번째와 j+1번째의 요소가 크기 순이 아니면 교환
            if (list[j] < list[j + 1])
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;

                strcpy(tempstr, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], tempstr);
            }
        }
    }
}

void printRank(struct Player *playerData[], int playerCount)
{
    int rank[3];
    char *name[3];
    for (int i = 0; i < playerCount; i++)
    {
        rank[i] = playerData[i]->money;
        name[i] = playerData[i]->name;
    }
    bubble_sort(rank, name, playerCount);
    printf("1위 : %s, %d원\n", name[0], rank[0]);
    printf("2위 : %s, %d원\n", name[1], rank[1]);
    printf("3위 : %s, %d원\n", name[2], rank[2]);
    //printf("4위 : %s, %d원\n", name[3], rank[3]);
}

int getIndex(Player *PlayerData[], int playerCount)
{
    for (int i = 0; i < playerCount; i++)
    {
        if (PlayerData[i]->pid == getpid())
        {
            return i;
        }
    }
}