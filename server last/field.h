typedef struct Field
{
    bool isFilled[250]; //땅이 사용중인지 체크
    bool isBoosted[250]; //비료를 줬는지 체크
    int cropType[250]; //심겨져있는 작물의 이름이 안에 담겨있음
    int growLevel[250]; //성장단계
}Field;
