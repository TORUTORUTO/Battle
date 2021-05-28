/* ID 0,1 に対応した2つの関数とクライアント名を使うようになっているだけ */
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#define N 500 /* 試行回数 */
#define OID (id^1) /* 相手の id */
#define CLIENT_A "client_0"
#define CLIENT_B "client_1"
/*
int sct[2][2]:点数表

自分 相手
[0][0]: 協調 協調
[0][1]: 協調 裏切り
[1][0]: 裏切り 協調
[1][1]: 裏切り 裏切り
int *h:過去の手
*(h+2*i+ID):i 回目の ID の手
int sc[2]:点数
sc[ID]:ID の点
*/
/*
次の手を計算する関数のプロトタイプ宣言
名前は何でもよい。引数も必要に応じて変えること
この例では
自分の ID、今何回目か、現在までの点数、過去の手の配列
を渡している。
*/
int play_0(int ID, int n,int SC[2] , int *H);
int play_1(int ID, int n,int SC[2] , int *H);
const int SCT[2][2]={{5,0},{10,2}};

int main(void){
    int i,j,k,t,n = N,rh[2],er ;
    char c_name[2][255];
    int server_len , client_len[2] ;
    // const int SCT[2][2]={{5,0},{10,2}};
    int *h; //int h[2][N];
    int sc[2]={0,0};
    FILE *LOG;
    LOG=fopen("log_tai","w");
    setbuf(stderr,NULL);
    // Initialize
    srand((unsigned int)time(NULL));

    //calloc
    h=(int *)calloc(2*n,sizeof(int));
    /////////////////////////////////////////
    for(j=0;j<n;j++){

        rh[0]=play_0(0, j, sc, h);
        rh[1]=play_1(1, j, sc, h);
        for(k=0;k<2;k++){
            *(h+(2*j)+k)=rh[k];
            sc[k] += SCT[rh[k]][rh[k^1]];
        }
        fprintf(LOG,"Turn %3d,h: %d %d, sc:%3d %3d,total: %3d%3d\n",j+1,*(h+(2*j)),*(h+(2*j)+1),SCT[*(h+(2*j))][*(h+(2*j)+1)],SCT[*(h+(2*j)+1)][*(h+(2*j))],sc[0],sc[1]);
    }
    fprintf(LOG," %s : %s .\n",CLIENT_A,CLIENT_B);
    fprintf(LOG,"score %d : %d . %d\n",sc[0],sc[1],sc[0]+sc[1]);
    fprintf(stderr,"\n THE END\n");
    fclose(LOG);

    printf("%d %d\n\n",sc[0],sc[1]);

    return 0;
}
int play_0(int ID,int n,int SC[2] , int *H)
{
    int sum=0,i=1,count=0;
    if(n==0){
        return 1;
    }
    while(count!=n){  //初回から現在にいたるまで協調を持ちかけてられているのなら協調する
        if(ID==0){    //ホーム戦、アウェイ線戦に対応
            sum += H[2*n-i];
        }else{
            sum += H[2*n-i-1];
        }
        i = i+2;
        count++;
    }
    if(sum==0){     //ずっと協調を持ちかけられているのなら協調を選択
        return 0;
    }
    else{           //一度でも裏切られたなら裏切りを選択
        return 1;
    }
}

int play_1(int ID,int n,int SC[2] , int *H)
{
    if(n==0){
        return 0;   //return 0ならしっぺ返し
    }
    if(ID==0){      //ホーム戦、アウェイ戦に対応
        return H[2*n-1];
    }else{
        return H[2*n-2];
    }
}





int play_sippe(int ID,int n,int SC[2] , int *H)
{
    if(n==0){
        return 0;   //0ならしっぺ返し、1なら逆しっぺ返し
    }
    if(ID==0){      //ホーム戦、アウェイ戦に対応
        return H[2*n-1];
    }else{
        return H[2*n-2];
    }
}