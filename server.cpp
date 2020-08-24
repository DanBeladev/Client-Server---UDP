#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include<Windows.h>

#define TIME_PORT	27015
#define MAX_SIZE_BUFF    255

void GetTime(char* resBuff);
void GetTimeWithoutDate(char* resBuff);
void GetTimeSinceEpoch(char* resBuff);
void GetClientToServerDelayEstimation(char *resBuff);
void MeasureRTT(char *resBuff);
void GetTimeWithoutDateOrSeconds(char* resBuff);
void GetYear(char* resBuff);
void GetMonthAndDay(char* resBuff);
void GetSecondsSinceBeginingOfMonth(char* resBuff);
void GetDayOfYear(char* resBuff);
void GetDaylightSavings(char* resBuff);
void GetBaijingTime(char* responseBuff);
void GetSydneyTime(char* responseBuff);
void GetWashingtonTime(char* responseBuff);
void GetRomeTime(char* responseBuff);
void GetUTC(char* responseBuff);

void main()
{
	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_socket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(TIME_PORT);

	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR *)&serverService, sizeof(serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	sockaddr client_addr;
	int client_addr_len = sizeof(client_addr);
	int bytesSent = 0;
	int bytesRecv = 0;
	char responseBuff[MAX_SIZE_BUFF];
	int userChoice;

	cout << "Time Server: Waiting for clients' requests.\n";

	while (true)
	{
		bytesRecv = recvfrom(m_socket, (char*)&userChoice, sizeof(int), 0, &client_addr, &client_addr_len);
		if (SOCKET_ERROR == bytesRecv)
		{
			cout << "Time Server: Error at recvfrom(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}

		userChoice = ntohl(userChoice);

		switch (userChoice)
		{
		case 1:
			GetTime(responseBuff);
			break;
		case 2:
			GetTimeWithoutDate(responseBuff);
			break;
		case 3:
			GetTimeSinceEpoch(responseBuff);
			break;
		case 4:
			GetClientToServerDelayEstimation(responseBuff);
			break;
		case 5:
			MeasureRTT(responseBuff);
			break;
		case 6:
			GetTimeWithoutDateOrSeconds(responseBuff);
			break;
		case 7:
			GetYear(responseBuff);
			break;
		case 8:
			GetMonthAndDay(responseBuff);
			break;
		case 9:
			GetSecondsSinceBeginingOfMonth(responseBuff);
			break;
		case 10:
			GetDayOfYear(responseBuff);
			break;
		case 11:
			GetDaylightSavings(responseBuff);
			break;
		case 14:
			GetBaijingTime(responseBuff);
			break;
		case 15:
			GetSydneyTime(responseBuff);
			break;
		case 16:
			GetWashingtonTime(responseBuff);
			break;
		case 17:
			GetRomeTime(responseBuff);
			break;
		case 18:
			GetUTC(responseBuff);
			break;
		default:
			break;
		}


		bytesSent = sendto(m_socket, responseBuff, (int)strlen(responseBuff), 0, (const sockaddr *)&client_addr, client_addr_len);
		if (SOCKET_ERROR == bytesSent)
		{
			cout << "Time Server: Error at sendto(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}
		cout << "Time Server: Sent: " << bytesSent << "\\" << strlen(responseBuff) << " bytes of \"" << responseBuff << "\" message.\n";
	}

	cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}

void GetTime(char* resBuff)
{
	time_t rawtime;
	tm *info;
	time(&rawtime);
	info = localtime(&rawtime);

	strcpy(resBuff, asctime(info));
	resBuff[strlen(resBuff) - 1] = '\0';
}

void GetTimeWithoutDate(char* resBuff)
{
	time_t myTime;
	time(&myTime);
	tm * timeinfo;
	timeinfo = localtime(&myTime);

	strftime(resBuff, MAX_SIZE_BUFF, "%H:%M:%S", timeinfo);
}

void GetTimeSinceEpoch(char* resBuff)
{
	time_t seconds = time(0);
	sprintf(resBuff, "%d Seconds have passed since 1970 ", seconds);
}

void GetClientToServerDelayEstimation(char *resBuff)
{
	sprintf(resBuff, "%d", (int)GetTickCount());
}

void MeasureRTT(char *resBuff)
{
	sprintf(resBuff, "%d", (int)GetTickCount());
}

void GetTimeWithoutDateOrSeconds(char* resBuff)
{
	time_t myTime;
	time(&myTime);
	tm * timeinfo;
	timeinfo = localtime(&myTime);

	strftime(resBuff, MAX_SIZE_BUFF, "%H:%M", timeinfo);
}

void GetYear(char* resBuff)
{
	time_t myTime;
	time(&myTime);
	tm * timeinfo;
	timeinfo = localtime(&myTime);

	strftime(resBuff, MAX_SIZE_BUFF, "%Y", timeinfo);
}

void GetMonthAndDay(char* resBuff)
{
	time_t myTime;
	time(&myTime);
	tm * timeinfo;
	timeinfo = localtime(&myTime);

	strftime(resBuff, MAX_SIZE_BUFF, "%e/%m", timeinfo);
}

void GetSecondsSinceBeginingOfMonth(char* resBuff)
{
	time_t currentTime;
	tm* beginingOfMonthTime;
	int seconds;
	time(&currentTime);
	beginingOfMonthTime = localtime(&currentTime);
	beginingOfMonthTime->tm_hour = 0;
	beginingOfMonthTime->tm_min = 0;
	beginingOfMonthTime->tm_sec = 0;
	beginingOfMonthTime->tm_mday = 1;
	seconds = difftime(currentTime, mktime(beginingOfMonthTime));
	sprintf(resBuff, "%d", seconds);
}

void GetDayOfYear(char* resBuff)
{
	time_t myTime;
	time(&myTime);
	tm * timeinfo;
	timeinfo = localtime(&myTime);

	strftime(resBuff, MAX_SIZE_BUFF, "%j", timeinfo);
}

void GetDaylightSavings(char* resBuff)
{
	time_t timeNow;
	tm* timeInfo;
	time(&timeNow);
	timeInfo = localtime(&timeNow);

	sprintf(resBuff, "%d", timeInfo->tm_isdst);
}

void GetBaijingTime(char* responseBuff) {
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	sprintf(responseBuff, "China - Baijing: %2d:%02d", (timeinfo->tm_hour + 8) % 24, timeinfo->tm_min);
}
void GetSydneyTime(char* responseBuff) {
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	sprintf(responseBuff, "Australia - Sydney: %2d:%02d", (timeinfo->tm_hour + 10) % 24, timeinfo->tm_min);

}
void GetWashingtonTime(char* responseBuff) {
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	int hours = timeinfo->tm_hour - 7;
	if (hours < 0)
	{
		hours += 24;
	}
	sprintf(responseBuff, "USA - Washington: %2d:%02d", (hours) % 24, timeinfo->tm_min);

}
void GetRomeTime(char* responseBuff) {
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	sprintf(responseBuff, "Italy - Rome: %2d:%02d", (timeinfo->tm_hour + 2) % 24, timeinfo->tm_min);

}
void GetUTC(char* responseBuff) {
	time_t rawtime;
	tm * timeinfo;
	time(&rawtime);
	timeinfo = gmtime(&rawtime);
	sprintf(responseBuff, "UTC: %2d:%02d", (timeinfo->tm_hour) % 24, timeinfo->tm_min);

}