#include <iostream>
#include <cstdio>
#include <Winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define xPort 8000
#define xIP "127.0.0.1"

int main()
{
    const int xBUF_SIZE = 64;
    WSADATA         wsd;                //WSADATA变量
    SOCKET          xServer;            //服务器套接字
    SOCKET          xClient;            //客户端套接字
    SOCKADDR_IN     xaddrServ;          //服务器地址

    char            recBuf[xBUF_SIZE];  //接受数据缓冲区
    char            sendBuf[xBUF_SIZE]; //返回数据缓冲区
    int             retVal;             //返回值

    //初始化套结字动态库,请求2.2版本winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }

    /*
    //创建socket操作
    // SOCKET socket(int af, int type, int protocol);
    // 第一个参数，指定地址簇(TCP/IP只能是AF_INET，也可写成PF_INET)
    // 第二个，选择套接字的类型(流式套接字)，第三个，特定地址家族相关协议（0为自动）
    */

//创建套接字
    xClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == xClient)
    {
        cout << "socket failed!" << endl;
        WSACleanup();//释放套接字资源;
        return  -1;
    }

//服务器套接字地址
    xaddrServ.sin_family = AF_INET;
    xaddrServ.sin_port = htons(xPort);
    xaddrServ.sin_addr.s_addr = inet_addr(xIP);

    /*
    // 将套接字xClient与远程主机相连
    // int connect( SOCKET s,  const struct sockaddr* name,  int namelen);
    // 第一个参数：需要进行连接操作的套接字
    // 第二个参数：设定所需要连接的地址信息
    // 第三个参数：地址的长度
    */
//连接服务器
    retVal = connect(xClient, (LPSOCKADDR)&xaddrServ, sizeof(xaddrServ));
    if(SOCKET_ERROR == retVal)
    {
        cout << "connect failed!" << endl;
        closesocket(xClient); //关闭套接字
        WSACleanup(); //释放套接字资源
        return -1;
    }
    //三次握手完成

// 客户端与用户端进行通信
    /*
    // send(), 在套接字上发送数据
    // int send( SOCKET s,  const char* buf,  int len,  int flags);
    // 第一个参数，需要发送信息的套接字，
    // 第二个参数，包含了需要被传送的数据，
    // 第三个参数是buffer的数据长度，
    // 第四个参数，一些传送参数的设置

    // recv(), 在套接字上接收数据
    // int recv(  SOCKET s,  char* buf,  int len,  int flags);
    // 第一个参数，建立连接后的套接字，
    // 第二个参数，接收数据
    // 第三个参数，接收数据的长度，
    // 第四个参数，一些传送参数的设置
    */
    cout << "*************************客户端**********************"<< endl;
    while(true)
    {
        memset(sendBuf, '/0', xBUF_SIZE);
        cout << "向服务器发送数据：";
        cin >> sendBuf;
        retVal = send(xClient, sendBuf, xBUF_SIZE, 0);
        if (SOCKET_ERROR == retVal)
        {
            cout << "send failed!" << endl;
            closesocket(xClient);   //关闭套接字
            WSACleanup();           //释放套接字资源;
            return -1;
        }
        if(sendBuf[0] == '0')
            break;
        memset(recBuf, '/0', sizeof(recBuf));
        recv(xClient, recBuf, xBUF_SIZE , 0); // 接收服务器端的数据， 只接收5个字符
        cout <<"从服务器接收数据："<< recBuf << endl;
        cout<<endl;
    }

    //退出
    closesocket(xClient);   //关闭套接字
    WSACleanup();           //释放套接字资源;

    return 0;
}
