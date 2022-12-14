#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    int serv_sock, clnt_sock, str_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char file_name[BUF_SIZE];
    char message[BUF_SIZE];
    char name_len[BUF_SIZE];

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;       
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
        error_handling("accept() error");
    //연결 시작 
    //txt 파일의 파일명의 길이를 클라이언트로부터 받는다. 
    if(read(clnt_sock, name_len, sizeof(char)+1) == -1)
        error_handling("read() error!");
    // printf("len = %d", (int)name_len[0]);
    // 클라이언트로 부터 받은 파일명의 길이를 이용해 파일의 이름을 받는다.
    if(read(clnt_sock, file_name, (int)name_len[0]+1) == -1)
        error_handling("read() error!");
    // 클라이언트로부터 받은 파일의 이름을 확인하는 디버깅용
    // printf("Message from client : %s \n", file_name);
    
    // 클라이언트로부터 받은 파일을 쓰기모드로 오픈 
    fp = fopen(file_name, "w");
    if(fp == NULL)
        error_handling("fopen() error!");
    // 클라이언트로부터 받은 파일의 내용이 버퍼의 크기를 초과할 수 있기때문에
    // 서버는 클라이언트가 write함수가 끝날때까지 받는다. 
    while((str_len=read(clnt_sock, message, BUF_SIZE)) != 0){
        // 내용을 확인하는 디버깅용
        // printf("rev : %s\n", message);
        fputs(message, fp);
    }
    // 클라이언트가 전송하는 파일명의 길이를 확인하는 디버깅 용
    // int name_len = 0;
    // for(int i = 0; file_name[i] != '\0'; i++)
    //     name_len++;
    // printf("name size = %d", name_len);
    //연결 해제
    close(clnt_sock);
    close(serv_sock);
    fclose(fp);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
