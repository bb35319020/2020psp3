#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#if 0
//  デバッグ用データ
#define NAP_SIZE    15

#define NUM_ITEMS   6
typedef struct {
    int price;
    int calorie;    
} Menu;

Menu ArrayItem[] = {
    {1, 2},
    {2, 3},
    {3, 1},
    {4, 4},
    {5, 8},
    {11, 15}
};
#endif

#if 1
#define NAP_SIZE    1000    //  所持金(1000円)
#define NUM_ITEMS   135     //  メニュー数   
typedef struct tagMenu{
    int id;         //  メニューのID
    char name[91];  //  メニューの名前
    char category[10];  //  カテゴリ
    char type[15];  //  タイプ
    int price;      //  価格
    int calorie;    //  カロリー
    float salt;     //  塩分量
} Menu;


int LoadData(Menu arrayItem[])
{
    FILE* fp;
    int cn = 0;
    char buf[256];

    fp = fopen("saizeriya.csv", "r");
    if(fp==NULL){
        fputs("File open error\n",stderr);
        return -1;
    }

    while(fgets(buf,sizeof(buf),fp) != NULL){
        sscanf(buf,"%d,%[^,],%[^,],%[^,],%d,%d,%f", 
            &(arrayItem[cn].id), arrayItem[cn].name, arrayItem[cn].category, arrayItem[cn].type, 
            &(arrayItem[cn].price), &(arrayItem[cn].calorie), &(arrayItem[cn].salt));
        cn++;
    }

    if(fclose(fp) == EOF){
        fputs("file close error\n",stderr);
        return -1;
    }

    return cn;
}
#endif


void DynamicProgLimited(Menu arrayItem[], int items, int nap_size)
{
    int nap_value[items+1][nap_size + 1];   //  動的計画法で作成するテーブル
    int history[items+1][nap_size + 1];     //  履歴を保存するテーブル(選択したメニューを探すときに使用)

    //　ここを実装する
    int i,j,k;

    for(i=0;i<=items;i++)/*配列の初期化*/
    {
        for(j=0;j<=nap_size;j++)
        {
            nap_value[i][j]=0;
        }
    }/*ここまでが初期化*/

    for(i=1;i<=items;i++)
    {
        for(k=1;k<arrayItem[i-1].price;k++)
        {
            nap_value[i][k]=nap_value[i-1][k];
        }
        for(j=arrayItem[i-1].price;j<=nap_size;j++)
        {
            if(nap_value[i-1][j]>nap_value[i-1][j-arrayItem[i-1].price]+arrayItem[i-1].calorie)
            {
                nap_value[i][j]=nap_value[i-1][j];
            }
            else
            {
                nap_value[i][j]=nap_value[i-1][j-arrayItem[i-1].price]+arrayItem[i-1].calorie;
            }
        }
    }
    printf("最大取得可能カロリー:%d",nap_value[items][nap_size]);
}


int main(void)
{
    Menu* arrayItem;
    int cnt;

    arrayItem = (Menu*)malloc(sizeof(Menu) * NUM_ITEMS);
    cnt = LoadData(arrayItem);

    //  動的プログラムで最大摂取カロリーを求める
    DynamicProgLimited(arrayItem, cnt, NAP_SIZE);
    return 0;
}