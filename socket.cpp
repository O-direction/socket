#include <iostream>
#include <cstdio>
#include <Winsock2.h>
//��Ҫ����ws2_32.lib
#include <pthread.h>
//���߳�ͷ�ļ�

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define xPort 8000
#define xIP "127.0.0.1"
#define NUM_THREADS 2

void* client(void* args){
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
        cout << "client-WSAStartup failed!" << endl;
        return 0;
    }else{
        cout<<"client_WSAStartup success!\n"<<endl;
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
        cout << "client socket failed!" << endl;
        WSACleanup();//�ͷ��׽�����Դ;
        return 0;
    }else{
        cout<<"client socket success!\n"<<endl;
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
        cout << "client connect failed!" << endl;
        closesocket(xClient); //�ر��׽���
        WSACleanup(); //�ͷ��׽�����Դ
        return 0;
    }else{
        cout<<"client connect success!\n"<<endl;
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
    cout << "�ͻ��������ɹ�"<< endl;
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
            return 0;
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

void* server(void *args){
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
        cout << "server-WSAStartup failed!" << endl;
        return 0;
    }else{
        cout<<"server-WSAStartup success!"<<endl;
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
        cout << "server socket failed!" << endl;
        WSACleanup();//�ͷ��׽�����Դ;
        return 0;
    }else{
        cout<<"server socket success!"<<endl;
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
        return 0;
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
        return 0;
    }else{
        cout<<"listen success!\n"<<endl;
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
    if(INVALID_SOCKET == xClient)
    {
        cout << "accept failed!" << endl;
        closesocket(xServer);   //�ر��׽���
        WSACleanup();           //�ͷ��׽�����Դ;
       return 0;
    }else{
        cout<<"accept success!\n"<<endl;
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
    cout << "�����������ɹ�\n"<< endl;
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
            return 0;
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
int main(){
    pthread_t tids[NUM_THREADS];
    pthread_create(&tids[0],NULL,server,NULL);
    pthread_create(&tids[1],NULL,client,NULL);
    pthread_exit(NULL);
}
