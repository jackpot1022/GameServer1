#include "server.h"
#include "weather.h"

struct Player *sharedMemory[MAX_PLAYERS];
struct Server *serverData;

void sendSignalToClients(int sig); // 클라이언트로 신호보냄
void handleSignal(int signum);     // 플레이어 최신화 신호받음
void handleReady(int signum);

int main()
{
    int shmid, shmid1;
    int playerIndex = 0;
    key_t key;
    key_t key1;

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        key = ftok("/home/g_202010953/FARM/real", first_player + i);
        shmid = shmget(key, sizeof(struct Player) * MAX_PLAYERS, 0666 | IPC_CREAT);
        if (shmid == -1)
        {
            perror("shmget");
            exit(EXIT_FAILURE);
        }
        sharedMemory[i] = (struct Player *)shmat(shmid, NULL, 0);
        if (sharedMemory[i] == (struct Player *)-1)
        {
            perror("shared_shmat");
            exit(EXIT_FAILURE);
        }
    }

    key1 = ftok("/home/g_202010953/FARM/real", server_key);

    shmid1 = shmget(key1, sizeof(struct Server), 0666 | IPC_CREAT);
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

    // 서버 공유메모리 변수 초기화
    serverData->playerCount = 0;
    serverData->day = 0;
    serverData->weather = SUNNY;
    serverData->isGameStart = false;
    Price_Init(&serverData->price);

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        strcpy(sharedMemory[i]->name, "");
            sharedMemory[i]->isActive = false;
            sharedMemory[i]->isEnd = false;
            sharedMemory[i]->pid = 0;
            sharedMemory[i]->money = 10000;
        for (int j = 0; j < 30; j++)
        {
            sharedMemory[i]->farm.cropSystem[j].isFilled = -1;
        }
    }

    serverData->server_pid = getpid();
    printf("서버pid : %d\n", serverData->server_pid);
    // 플레이어 접속 알림
    signal(SIGUSR1, handleSignal);
    // 플레이어 준비 알림
    signal(SIGUSR2, handleReady);
    // 맨 처음 플레이어가 모두 모였을때 게임 시작
    while (true)
    {
        if (serverData->playerCount == MAX_PLAYERS)
        {
            serverData->isGameStart = true;
            sendSignalToClients(1);
            break;
        }
    }

    // 플레이어들이 모두 분기를 마치면 다음 분기 진행
    while (serverData->day < MAXDAY)
    {
        if (sharedMemory[0]->isActive == false && sharedMemory[1]->isActive == false && sharedMemory[2]->isActive == false)
        {
            sharedMemory[0]->isActive = true;
            sharedMemory[1]->isActive = true;
            sharedMemory[2]->isActive = true;
            // 분기 넘기고, 날씨 및 시세 업데이트 후 다시 게임 불러옴
            serverData->day++;
            Weather_Simulate(&serverData->weather, serverData->day);
            Price_Update(&serverData->price, serverData->weather);
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    if (sharedMemory[i]->farm.cropSystem[j].isFilled != -1)
                    {
                        if (sharedMemory[i]->farm.isBoosted[j])
                        {
                            sharedMemory[i]->farm.cropSystem[j].dayPlanting += 2;
                        }
                        else
                        {
                            sharedMemory[i]->farm.cropSystem[j].dayPlanting++;
                        }
                    }
                }
            }
            sendSignalToClients(1);
        }
    }

    // 엔딩
    sendSignalToClients(3);
    while (true)
    {
        if (sharedMemory[0]->isEnd == true && sharedMemory[1]->isEnd == true && sharedMemory[2]->isEnd == true)
        {
            break;
        }
    }

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        shmdt(sharedMemory[i]);
    }
    shmdt(serverData);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(shmid1, IPC_RMID, NULL);
    return 0;
}

void sendSignalToClients(int sig)
{
    switch (sig)
    {
    case 1:
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (sharedMemory[i]->isActive == true)
            {
                printf("클라이언트로 게임시작 신호보냄: %d\n", sharedMemory[i]->pid);
                kill(sharedMemory[i]->pid, SIGUSR1);
            }
        }
        break;
    case 2:
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (sharedMemory[i]->isActive == true)
            {
                printf("클라이언트로 명단 출력 신호보냄: %d\n", sharedMemory[i]->pid);
                kill(sharedMemory[i]->pid, SIGUSR2);
            }
        }
        break;
    case 3:
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            kill(sharedMemory[i]->pid, SIGUSR2);
        }
        break;
    case 4:
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            kill(sharedMemory[i]->pid, SIGALRM);
        }
        break;
    }
}

// 서버가 사용자명 출력 신호
void server_info()
{
    printf("플레이어[%d] 입장: %s\n", serverData->playerCount, sharedMemory[serverData->playerCount]->name);
    sharedMemory[serverData->playerCount]->isActive = true;
    serverData->playerCount = serverData->playerCount + 1;
    printf("현재 플레이어 수: %d\n", serverData->playerCount);
    sendSignalToClients(2);
}

void handleSignal(int signum)
{
    if (signum == SIGUSR1)
    {
        printf("SIGUSR1 시그널을 받았습니다. 플레이어카운트를 출력합니다.\n");
        server_info();
    }
}

void handleReady(int signum)
{
    if (signum == SIGUSR2)
    {
        printf("SIGUSR2 시그널을 받았습니다. 준비현황을 출력합니다.\n");
        sendSignalToClients(4);
    }
}