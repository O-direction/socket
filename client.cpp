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
    }

    /*
    //����socket����
    // SOCKET socket(int af, int type, int protocol);
    // ��һ��������ָ����ַ��(TCP/IPֻ����AF_INET��Ҳ��д��PF_INET)
    // �ڶ�����ѡ���׽��ֵ�����(��ʽ�׽���)�����������ض���ַ�������Э�飨0Ϊ�Զ���
    */

//�����׽���
    xClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(INVALID_SOCKET == xClient)
    {
        cout << "socket failed!" << endl;
        WSACleanup();//�ͷ��׽�����Դ;
        return  -1;
    }

//�������׽��ֵ�ַ
    xaddrServ.sin_family = AF_INET;
    xaddrServ.sin_port = htons(xPort);
    xaddrServ.sin_addr.s_addr = inet_addr(xIP);

    /*
    // ���׽���xClient��Զ����������
    // int connect( SOCKET s,  const struct sockaddr* name,  int namelen);
    // ��һ����������Ҫ�������Ӳ������׽���
    // �ڶ����������趨����Ҫ���ӵĵ�ַ��Ϣ
    // ��������������ַ�ĳ���
    */
//���ӷ�����
    retVal = connect(xClient, (LPSOCKADDR)&xaddrServ, sizeof(xaddrServ));
    if(SOCKET_ERROR == retVal)
    {
        cout << "connect failed!" << endl;
        closesocket(xClient); //�ر��׽���
        WSACleanup(); //�ͷ��׽�����Դ
        return -1;
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
    cout << "*************************�ͻ���**********************"<< endl;
    while(true)
    {
        memset(sendBuf, '/0', xBUF_SIZE);
        cout << "��������������ݣ�";
        cin >> sendBuf;
        retVal = send(xClient, sendBuf, xBUF_SIZE, 0);
        if (SOCKET_ERROR == retVal)
        {
            cout << "send failed!" << endl;
            closesocket(xClient);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ;
            return -1;
        }
        if(sendBuf[0] == '0')
            break;
        memset(recBuf, '/0', sizeof(recBuf));
        recv(xClient, recBuf, xBUF_SIZE , 0); // ���շ������˵����ݣ� ֻ����5���ַ�
        cout <<"�ӷ������������ݣ�"<< recBuf << endl;
        cout<<endl;
    }

    //�˳�
    closesocket(xClient);   //�ر��׽���
    WSACleanup();           //�ͷ��׽�����Դ;

    return 0;
}
