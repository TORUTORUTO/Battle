#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <strings.h>
// 相手の ID を示すマクロ
#define OID (ID^1)
// 自作クライアントの名前に変更すること
#define CLIENT_NAME "Torancell"
/*
次の手を計算する関数のプロトタイプ宣言
名前は何でもよい。引数も必要に応じて変えること
この例では
自分の ID、今何回目か、現在までの点数、過去の手の配列
を渡している。
*/
int play(int ID, int n,int SC[2] , int *H);
int main(void)
{
    int sockfd ;
    int len ;
    int i=0,j,k,nexthand,er,N,T;
    struct sockaddr_un address ;
    int result ;
    char ID;// 0 or 1//
    char name[256];
    int SCT[2][2];//点数の表
    int *H;//1 回目の ID0 の手、ID1 の手、2 回目の ID0 の手、ID1 の手...
    int SC[2];//合計の点数
    FILE *LOG;
    // ソケットの用意
    sockfd = socket(AF_UNIX,SOCK_STREAM,0);
    address.sun_family = AF_UNIX ;
    strcpy(address.sun_path , "server_socket");
    len = sizeof(address);
    result = connect(sockfd , (struct sockaddr *)&address , len);
    if ( result == -1 ) {
        perror("oops: client1");
        exit(1);
    }
    // ソケットプログラミングでは有名な話なんですが、
    // (Unix 系 OS のサーバだと)サーバのプロセスが accept(2)を発行する前に
    // クライアントの connect(2)は成功します。

    // # カーネルが気を利かせて勝手に先を進めてしまうため。
    /* だからエラーが出るかもしれない */
    setbuf(stderr,NULL);
    // クライアント名を送る用意
    strcpy(name,CLIENT_NAME);
    // クライアント名の長さを求めて送る
    k = strlen(name);k++;/* strlen()は末尾の\0 を含まない文字数を返すため */
    er = send(sockfd,&k,sizeof(k),0);
    fprintf(stderr,"cli er %d\n",er);
    // 自分の ID を受け取る
    recv(sockfd,&ID,sizeof(ID),0);
    // クライアント名を送る
    send(sockfd,name,k,0);
    // ここからログファイルを作る 必要なければ省略可
    sprintf(name,"logcli_%d",ID);
    LOG=fopen(name,"w");
    fprintf(LOG,"Client start\n");
    fprintf(LOG,"<%s>\n",name);
    // 全部で何回戦かを受け取る
    recv(sockfd,&N,sizeof(N),0);
    fprintf(LOG,"turn is %d \n",N);
    // 回数がわかったので手を保存するメモリを確保
    H=(int *)calloc(2*N,sizeof(int));
    // 点数表を受け取る
    recv(sockfd,SCT,sizeof(SCT),0);
    for(i=0;i<2;i++){
        for(j=0;j<2;j++){
            fprintf(LOG,"SCT[%d][%d]=%d\n",i,j,SCT[i][j]);
        }
    }
    // 対戦開始
    for(T=0;T<N;T++){
        // 現在までの合計点数を受け取る
        recv(sockfd,SC,sizeof(SC),0);
        // 過去の双方の手を受け取る
        recv(sockfd,H,2*N*sizeof(int),0);
        // 今回の手(0 or 1)を決める (ここを作成する)
        nexthand = play(ID,T,SC,H);
        // 決めた手を送る
        send(sockfd,&nexthand,sizeof(nexthand),0);

        fprintf(LOG,"send %d\n",nexthand);
    }
    // 対戦終了、ソケットを閉じる
    shutdown(sockfd,2);
    close(sockfd);
    // 確保したメモリの開放、ログファイルのクローズ
    free(H);
    fclose(LOG);
    exit(0);
}
int play(int ID,int N,int SC[2] , int *H)
{
    int nexthand=1;
    /* この部分を作成する */
    return nexthand;
}