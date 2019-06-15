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
    WSADATA         wsd;                //WSADATA����
    SOCKET          xServer;            //�������׽���
    SOCKET          xClient;            //�ͻ����׽���
    SOCKADDR_IN     xaddrServ;          //��������ַ

    char            recBuf[xBUF_SIZE];  //�������ݻ�����
    char            sendBuf[xBUF_SIZE]; //�������ݻ�����
    int             retVal;             //����ֵ

    //��ʼ���׽��ֶ�̬��,����2.2�汾winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }else{
        cout<<"WSAStartup success!"<<endl;
    }

    /*
    //����socket����
    // SOCKET socket(int af, int type, int protocol);
    // ��һ��������ָ����ַ��(TCP/IPֻ����AF_INET��Ҳ��д��PF_INET)
    // �ڶ�����ѡ���׽��ֵ�����(��ʽ�׽���)�����������ض���ַ�������Э�飨0Ϊ�Զ���
    */

//�����׽���
    xServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == xServer)
    {
        cout << "socket failed!" << endl;
        WSACleanup();//�ͷ��׽�����Դ;
        return  -1;
    }else{
        cout<<"socket success!"<<endl;
    }

//�������׽��ֵ�ַ
    xaddrServ.sin_family = AF_INET;
    xaddrServ.sin_port = htons(xPort);
    xaddrServ.sin_addr.s_addr = INADDR_ANY;
    /*
    //NADDR_ANYת����������0.0.0.0��ָ��������˼��Ҳ���Ǳ�ʾ����������IP��
    //��Ϊ��Щ���Ӳ�ֹһ��������������������£�����ͱ�ʾ��������ip��ַ����˼��
    */

    /*
    //�׽���xServer�뱾�ص�ַ����
    // int bind(SOCKET s, const struct sockaddr* name, int namelen);
    // ��һ��������ָ����Ҫ�󶨵��׽��֣�
    // �ڶ���������ָ�����׽��ֵı��ص�ַ��Ϣ���õ�ַ�ṹ�������õ�����Э��Ĳ�ͬ����ͬ
    // ������������ָ��������Э���ַ�ĳ���
    */

//���׽���
    retVal = bind(xServer, (SOCKADDR *)&xaddrServ, sizeof(SOCKADDR_IN));
    if(SOCKET_ERROR == retVal)
    {
        cout << "bind failed!" << endl;
        closesocket(xServer);   //�ر��׽���
        WSACleanup();           //�ͷ��׽�����Դ;
        return -1;
    }else{
        cout<<"bind success!"<<endl;
    }

    /*
    // ���׽�������Ϊ����ģʽ���������󣩣� listen()֪ͨTCP������׼���ý�������
    // int listen(SOCKET s,  int backlog);
    // ��һ������ָ����Ҫ���õ��׽��֣��ڶ�������Ϊ���ȴ����Ӷ��е���󳤶ȣ�
    */

//��ʼ����
    retVal = listen(xServer, 10);
    if(SOCKET_ERROR == retVal)
    {
        cout << "listen failed!" << endl;
        closesocket(xServer);   //�ر��׽���
        WSACleanup();           //�ͷ��׽�����Դ;
        return -1;
    }else{
        cout<<"listen success!"<<endl;
    }

    /*
    // accept()���������ӣ��ȴ��ͻ�������
    // SOCKET accept(  SOCKET s,  struct sockaddr* addr,  int* addrlen);
    // ��һ������������һ�����ڼ���״̬�µ��׽���
    // �ڶ���������sockaddr���ڱ���ͻ��˵�ַ����Ϣ
    // ����������������ָ�������ַ�ĳ���
    // ���ص��������������״̬�µ��׽���ͨ�ŵ��׽���
    */

//���ܿͻ�������
    sockaddr_in addrClient;
    int addrClientLen = sizeof(sockaddr_in);
    xClient = accept(xServer, (sockaddr*)&addrClient, &addrClientLen);
    cout<<"accepted"<<endl;
    if(INVALID_SOCKET == xClient)
    {
        cout << "accept failed!" << endl;
        closesocket(xServer);   //�ر��׽���
        WSACleanup();           //�ͷ��׽�����Դ;
        return -1;
    }else{
        cout<<"accept success!"<<endl;
    }

    //�����������

// �ͻ������û��˽���ͨ��
    /*
    // send(), ���׽����Ϸ�������
    // int send( SOCKET s,  const char* buf,  int len,  int flags);
    // ��һ����������Ҫ������Ϣ���׽��֣�
    // �ڶ�����������������Ҫ�����͵����ݣ�
    // ������������buffer�����ݳ��ȣ�
    // ���ĸ�������һЩ���Ͳ���������

    // recv(), ���׽����Ͻ�������
    // int recv(  SOCKET s,  char* buf,  int len,  int flags);
    // ��һ���������������Ӻ���׽��֣�
    // �ڶ�����������������
    // �������������������ݵĳ��ȣ�
    // ���ĸ�������һЩ���Ͳ���������
    */
    cout << "*************************������**********************"<< endl;
    while(true)
    {
        memset(recBuf, '/0', xBUF_SIZE);
        retVal = recv(xClient, recBuf, xBUF_SIZE, 0);
        if (SOCKET_ERROR == retVal)
        {
            cout << "recv failed!" << endl;
            closesocket(xServer);   //�ر��׽���
            closesocket(xClient);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ;
            return -1;
        }
        if(recBuf[0] == '0')
            break;
        cout << "�ͻ��˷��͵�����: " << recBuf <<endl;

        memset(sendBuf, '/0', xBUF_SIZE);
        cout << "��ͻ��˷�������: " ;
        cin >> sendBuf;
        cout << endl;

        send(xClient, sendBuf, xBUF_SIZE, 0);
    }

    //�˳�
    closesocket(xServer);   //�ر��׽���
    closesocket(xClient);   //�ر��׽���
    WSACleanup();           //�ͷ��׽�����Դ;

    return 0;
}
