#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string>

#define TIME_PORT	27015
#define REQUESTS_NUMBER	100
#define MAX_SIZE_BUFF	255

void printMenu();
void printCities();
bool isValid(int userChoise);
bool isValidCities(int userChoise);

void main()
{
	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);


	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[MAX_SIZE_BUFF];
	char recvBuff[MAX_SIZE_BUFF];
	bool toContinue = true;
	int userChoice;
	int startReq, endReq, sumOfStumpsDiff = 0;
	double average;
	int startRttCount, endRttCount;

	while (toContinue) {
		printMenu();
		cin >> userChoice;

		if (isValid(userChoice)) {
			switch (userChoice)
			{
			case 13: // exit
				cout << "Bye Bye"<< endl;
				toContinue = false;
				break;
			case 4:  //Delay Estimation
				
				for (int i = 0; i < REQUESTS_NUMBER; i++) {
					userChoice = htonl(userChoice);
					bytesSent = sendto(connSocket, (char*)&userChoice, sizeof(int), 0, (const sockaddr *)&server, sizeof(server));
					if (SOCKET_ERROR == bytesSent)
					{
						cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
				}
				for (int i = 0; i < REQUESTS_NUMBER; i++) {
					bytesRecv = recv(connSocket, recvBuff, MAX_SIZE_BUFF, 0);
					if (SOCKET_ERROR == bytesRecv)
					{
						cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					recvBuff[bytesRecv] = '\0';

					
					endReq = atoi(recvBuff);
					if (i == 0)
					{
						startReq = endReq;
					}
					else
					{
					sumOfStumpsDiff += endReq - startReq;
					startReq = endReq;
					}
				}
				average = ((double)sumOfStumpsDiff / (double)(REQUESTS_NUMBER - 1));
				cout << endl << "Time Client: Delay Estimation:  " << average << " ms " << endl;
				sumOfStumpsDiff = 0;
				average = 0;
				break;

			case 5:  //RTT
				for (int i = 0; i < REQUESTS_NUMBER; i++)
				{
					startRttCount = GetTickCount();

					userChoice = htonl(userChoice);
					bytesSent = sendto(connSocket, (char*)&userChoice, sizeof(int), 0, (const sockaddr *)&server, sizeof(server));
					if (SOCKET_ERROR == bytesSent)
					{
						cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}

					bytesRecv = recv(connSocket, recvBuff, MAX_SIZE_BUFF, 0);
					if (SOCKET_ERROR == bytesRecv)
					{
						cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
						closesocket(connSocket);
						WSACleanup();
						return;
					}
					recvBuff[bytesRecv] = '\0';
					endRttCount = GetTickCount();
					sumOfStumpsDiff += (endRttCount - startRttCount);
				}
				average = ((double)sumOfStumpsDiff / (double)REQUESTS_NUMBER);
				cout << endl << "Time Client: RTT: " << average << " ms " << endl;
				sumOfStumpsDiff=0;
				average = 0;
				break;

			default:
				if (userChoice == 12)
				{
					printCities();
					cin >> userChoice;
					if (!isValidCities(userChoice)) {
						break;
					}
				}
				userChoice = htonl(userChoice);
				bytesSent = sendto(connSocket, (char*)&userChoice, sizeof(int), 0, (const sockaddr *)&server, sizeof(server));
				if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}


				bytesRecv = recv(connSocket, recvBuff, MAX_SIZE_BUFF, 0);
				if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}

				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
				break;
			}
		}
	}


	// Closing connections and Winsock.
	cout << "Time Client: Closing Connection.\n";
	closesocket(connSocket);

	system("pause");
}

void printMenu()
{
	cout << "===============================================================\n";
	cout << "Enter Your Choice Please:\n";
	cout << "   1 - Get Time\n";
	cout << "   2 - Get Time Without Date\n";
	cout << "   3 - Get Time Since Epoch\n";
	cout << "   4 - Get Client To Server Delay Estimation\n";
	cout << "   5 - Measure RTT\n";
	cout << "   6 - Get Time Without Date Or Seconds\n";
	cout << "   7 - Get Year\n";
	cout << "   8 - Get Month And Day\n";
	cout << "   9 - Get Seconds Since Begining Of Month\n";
	cout << "  10 - Get How Many Days Were Passed since Begining Of Current Year\n";
	cout << "  11 - Get Day Light Savings\n";
	cout << "  12 - Get Time Without Date In City\n";
	cout << "  13 - Exit\n";
	cout << "===============================================================\n";
	cout << ">> ";
}

void printCities()
{	
		cout << "===============================================================\n";
		cout << "Choose City Please:\n";
		cout << "14 - China - Baijing\n";
		cout << "15 - Australia - Sydney\n";
		cout << "16 - USA - Washington\n";
		cout << "17 - Italy - Rome\n";
		cout << "18 - UTC\n";
		cout << "===============================================================\n";
	cout << ">> ";
}

bool isValidCities(int userChoise)
{
	if (userChoise > 18 || userChoise < 14)
	{
		cout << "INPUT ERROR: City Choice should be between 14-18\n";
		return false;
	}
	return true;
}

bool isValid(int userChoise)
{
	if (userChoise > 13 || userChoise < 1)
	{
		cout << "INPUT ERROR: Your Choice Should Be Between 1 to 13, Please Try Again\n";
		return false;
	}
	return true;
}
