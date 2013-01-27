#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

struct Buffer {
int ID; //identyfikator funkcji , patrz dalej dostepne klucze
int ID_USR; // nadawany przez serwer klucz dla każdego połączonego z serwerem użytkownika
int iKey[16]; // w tym polu mamy kolejne argumenty dla funkcji
char cChat[256];
};

void pack(Buffer buff, char (&ref)[512]) //pakowanie - klient
{
     int i,j=0;


        ref[0]=buff.ID;

        ref[1]=buff.ID_USR;


    for(i=2;i<sizeof(buff.iKey);i++)
    {
        ref[i]=buff.iKey[j]; j++;
    } ref[i]=' '; i++;

    for(j=0;j<sizeof(buff.cChat);i++)
    {
        ref[i]=buff.cChat[j]; j++;
    }


}

Buffer unpack(char ref[512]) //odpakowanie - klient
{
     int i,j=0;
     Buffer tempbuff;

        tempbuff.ID=ref[0];
        tempbuff.ID_USR=ref[1];


    for(i=2;i<sizeof(tempbuff.iKey);i++)
    {
        tempbuff.iKey[j]=ref[i]; j++;
    } i++;

    for(j=0;j<sizeof(tempbuff.cChat);i++)
    {
        tempbuff.cChat[j]=ref[i]; j++;
    }

return tempbuff;
}

/*int ClientThread()
{
        Buffer sbuffer;

        char buffer[sizeof(sbuffer)] = {0};



        for(;; Sleep(10))
        {
                // The server will send a struct to the client
                // containing message and ID
                // But send only accepts a char as buffer parameter
                // so here we need to recv a char buffer and then
                // we copy the content of this buffer to our struct
                if(recv(sConnect, buffer, sizeof(sbuffer), NULL))
                {
                        memcpy(&sbuffer, buffer, sizeof(sbuffer));
                        cout << "<Client " << sbuffer.ID << ":> " << sbuffer.cChat <<endl;
                }
        }*/

int main(int argc, char* argv[])
{
    //laczenie z serwerem
    Buffer sbuffer;
    int RetVal = 0;
        WSAData wsaData;
        WORD DllVersion = MAKEWORD(2,1);
        RetVal = WSAStartup(DllVersion, &wsaData);
        if(RetVal != 0)
        {
                MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
                exit(1);
        }

    SOCKADDR_IN saddr;
    SOCKET      sock;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        saddr.sin_port        = htons(27015);
        saddr.sin_family      = AF_INET;

    if ( connect(sock, (sockaddr*)&saddr, sizeof(sockaddr)) == SOCKET_ERROR )
    {
        cout<<"Odpal serwer czopie";
        sock = 0;
        return -1;
    }

    char pakiet[512]= {0};

  /*  sbuffer.ID=20; //test pakowania/wypakowania
    cout<<sbuffer.ID<<endl;
    sbuffer.ID_USR=1;
    sbuffer.iKey[0]='z';
    sbuffer.cChat[0]='x';

    pack(sbuffer,pakiet);
    cout<<(int)pakiet[0]<<" "<<(int)pakiet[1]<<pakiet[2]<<endl;

    sbuffer=unpack(pakiet);
    cout<<sbuffer.ID<<" "<<sbuffer.cChat[0]<<endl;*/



  //  send(sock,pakiet,sizeof(pakiet),0);
  //  recv(sock,pakiet,sizeof(pakiet),0);


    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}
