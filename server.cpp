#include <iostream>
#include <cstdio>
#include <Winsock2.h>
#include <cstring>
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
    }else{
        cout<<"WSAStartup success!"<<endl;
    }

    /*
    //创建socket操作
    // SOCKET socket(int af, int type, int protocol);
    // 第一个参数，指定地址簇(TCP/IP只能是AF_INET，也可写成PF_INET)
    // 第二个，选择套接字的类型(流式套接字)，第三个，特定地址家族相关协议（0为自动）
    */

//创建套接字
    xServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == xServer)
    {
        cout << "socket failed!" << endl;
        WSACleanup();//释放套接字资源;
        return  -1;
    }else{
        cout<<"socket success!"<<endl;
    }

//服务器套接字地址
    xaddrServ.sin_family = AF_INET;
    xaddrServ.sin_port = htons(xPort);
    xaddrServ.sin_addr.s_addr = INADDR_ANY;
    /*
    //NADDR_ANY转换过来就是0.0.0.0泛指本机的意思，也就是表示本机的所有IP，
    //因为有些机子不止一块网卡，多网卡的情况下，这个就表示所有网卡ip地址的意思。
    */

    /*
    //套接字xServer与本地地址相连
    // int bind(SOCKET s, const struct sockaddr* name, int namelen);
    // 第一个参数，指定需要绑定的套接字；
    // 第二个参数，指定该套接字的本地地址信息，该地址结构会随所用的网络协议的不同而不同
    // 第三个参数，指定该网络协议地址的长度
    */

//绑定套接字
    retVal = bind(xServer, (SOCKADDR *)&xaddrServ, sizeof(SOCKADDR_IN));
    if(SOCKET_ERROR == retVal)
    {
        cout << "bind failed!" << endl;
        closesocket(xServer);   //关闭套接字
        WSACleanup();           //释放套接字资源;
        return -1;
    }else{
        cout<<"bind success!"<<endl;
    }

    /*
    // 将套接字设置为监听模式（连接请求）， listen()通知TCP服务器准备好接收连接
    // int listen(SOCKET s,  int backlog);
    // 第一个参数指定需要设置的套接字，第二个参数为（等待连接队列的最大长度）
    */

//开始监听
    retVal = listen(xServer, 10);
    if(SOCKET_ERROR == retVal)
    {
        cout << "listen failed!" << endl;
        closesocket(xServer);   //关闭套接字
        WSACleanup();           //释放套接字资源;
        return -1;
    }else{
        cout<<"listen success!"<<endl;
    }

    /*
    // accept()，接收连接，等待客户端连接
    // SOCKET accept(  SOCKET s,  struct sockaddr* addr,  int* addrlen);
    // 第一个参数，接收一个处于监听状态下的套接字
    // 第二个参数，sockaddr用于保存客户端地址的信息
    // 第三个参数，用于指定这个地址的长度
    // 返回的是向与这个监听状态下的套接字通信的套接字
    */

//接受客户端请求
    sockaddr_in addrClient;
    int addrClientLen = sizeof(sockaddr_in);
    xClient = accept(xServer, (sockaddr*)&addrClient, &addrClientLen);
    cout<<"accepted"<<endl;
    if(INVALID_SOCKET == xClient)
    {
        cout << "accept failed!" << endl;
        closesocket(xServer);   //关闭套接字
        WSACleanup();           //释放套接字资源;
        return -1;
    }else{
        cout<<"accept success!"<<endl;
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
    cout << "*************************服务器**********************"<< endl;
    while(true)
    {
        memset(recBuf, '/0', xBUF_SIZE);
        retVal = recv(xClient, recBuf, xBUF_SIZE, 0);
        if (SOCKET_ERROR == retVal)
        {
            cout << "recv failed!" << endl;
            closesocket(xServer);   //关闭套接字
            closesocket(xClient);   //关闭套接字
            WSACleanup();           //释放套接字资源;
            return -1;
        }
        if(recBuf[0] == '0')
            break;
        cout << "客户端发送的数据: " << recBuf <<endl;

        memset(sendBuf, '/0', xBUF_SIZE);
        cout << "向客户端发送数据: " ;
        cin >> sendBuf;
        cout << endl;

        send(xClient, sendBuf, xBUF_SIZE, 0);
    }

    //退出
    closesocket(xServer);   //关闭套接字
    closesocket(xClient);   //关闭套接字
    WSACleanup();           //释放套接字资源;

    return 0;
}
