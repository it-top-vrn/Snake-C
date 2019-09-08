#include "pch.h" 
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include <conio.h>
#pragma warning(disable: 4996)
using namespace std;

#define a char(219)
#define b char(254)

int xROW; // координата х
int yCOL; // координата у
int counte = 0; // счетчик подсчета очков 1го игрока
int counteClient = 0; // счетчик подсчета очков 2го игрока
int IndicatorRecv = 0;
bool currentGamePlay = false;
void Send(string xy);
void ClientHandler();
void ClientHandlerClient();
void GetCoordinat(string massage);
void ServerForPlay();
int ClientForPlay();
int GamePlayServer();
int GamePlayClient();
int GamePlayOffline();
int PlayAgaine();
int GameMode();
int ChoosePlayer();
void SendClient(string xy);
void CleanMap();
string CurrentMyIP();
SOCKET newConnection;
SOCKET Connection;
int MapW, MapH;
int MapWclient, MapHclient;

char Map[100][100] = {
"########################################################",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"#                                                      #",
"########################################################" 
};

struct snakeBlock {
	int x, y;
};

struct snakeBlockClient {
	int xClient, yClient;
};

void Firstfryct() {
	int rx, ry;
	rx = 2 + rand() % 15;
	ry = 2 + rand() % 15;
	Map[ry][rx] = a;
}

void gotoxy(int x, int y)
{
	COORD coord = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void gotoxyClient(int x, int y)
{
	COORD coordClient = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordClient);
}

void drawMap(vector <snakeBlock>snake)
{

	MapH = 0;
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPlayer 1 = " << counte;
	cout << "\nPlayer 2 = " << counteClient << endl;

	for (int i = 0; Map[i][0]; i++)
	{
		MapW = 0;
		for (int j = 0; Map[i][j]; j++)
		{
			MapW++;
			if (Map[i][j] != ' ')
			{
				gotoxy(j, i);
				cout << Map[i][j];
			}
		}
		MapH++;
	}
	for (int i = 0; i < snake.size(); i++)
	{
		gotoxy(snake[i].x, snake[i].y);
		cout << a;

	}
}

void drawMapClient(vector <snakeBlockClient>snakeclient)
{
	MapHclient = 0;
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPlayer 1 = " << counte;
	cout << "\nPlayer 2 = " << counteClient << endl;

	for (int i = 0; Map[i][0]; i++)
	{
		MapWclient = 0;
		for (int j = 0; Map[i][j]; j++)
		{
			MapWclient++;
			if (Map[i][j] != ' ')
			{
				gotoxyClient(j, i);
				cout << Map[i][j];
			}
		}
		MapHclient++;
	}
	for (int i = 0; i < snakeclient.size(); i++)
	{
		gotoxyClient(snakeclient[i].xClient, snakeclient[i].yClient);
		cout << b;
	}
}

bool checkLose(int x, int y, vector <snakeBlock>&snake)
{

	if (Map[y][x] == '#' || Map[y][x] == b)
		return true;
	if (snake.size() > 3)
	{
		for (int i = 3; i < snake.size(); i++)
			if (snake[i].x == x && snake[i].y == y)
				return true;
	}
	if (Map[y][x] == a)
	{
		counte++;
		Map[y][x] = ' ';
		snakeBlock newSnake;
		newSnake.x = snake[snake.size() - 1].x;
		newSnake.y = snake[snake.size() - 1].y;
		snake.push_back(newSnake);
		int rx, ry;
		do {
			rx = rand() % MapW;
			ry = rand() % MapH;
		} while (checkLose(rx, ry, snake));
		Map[ry][rx] = a;
		drawMap(snake);
	}
	return false;
}

bool checkLoseClient(int x, int y, vector <snakeBlockClient>&snakeclient)
{
	if (Map[y][x] == '#' || Map[y][x] == b)
		return true;
	if (snakeclient.size() > 3)
	{
		for (int i = 3; i < snakeclient.size(); i++)
			if (snakeclient[i].xClient == x && snakeclient[i].yClient == y)
				return true;
	}

	if (Map[y][x] == a)
	{
		counteClient++;
		Map[y][x] = ' ';
		snakeBlockClient newSnakeClient;
		newSnakeClient.xClient = snakeclient[snakeclient.size() - 1].xClient;
		newSnakeClient.yClient = snakeclient[snakeclient.size() - 1].yClient;
		snakeclient.push_back(newSnakeClient);
		int rx, ry;
		do {
			rx = rand() % MapWclient;
			ry = rand() % MapHclient;
		} while (checkLoseClient(rx, ry, snakeclient));
		Map[ry][rx] = a;
		drawMapClient(snakeclient);
	}
	return false;
}

void snakeInit(int x, int y, vector<snakeBlock> &snake)
{
	snakeBlock newSnake;
	newSnake.x = x;
	newSnake.y = y;
	snake.push_back(newSnake);
}

void snakeInitClient(int x, int y, vector<snakeBlockClient> &snake)
{
	snakeBlockClient newSnakeClient;
	newSnakeClient.xClient = x;
	newSnakeClient.yClient = y;
	snake.push_back(newSnakeClient);
}

bool snakeMove(vector<snakeBlock>&snake, short dire[2])
{
	int oldx, oldy, x, y;
	gotoxy(snake[snake.size() - 1].x, snake[snake.size() - 1].y);
	cout << " ";
	oldx = snake[0].x;
	oldy = snake[0].y;
	snake[0].x += dire[0];
	snake[0].y += dire[1];
	gotoxy(snake[0].x, snake[0].y);
	cout << a;
	if (snake.size() > 1)
	{
		for (int i = 1; i < snake.size(); i++)
		{
			x = snake[i].x;
			y = snake[i].y;
			snake[i].x = oldx;
			snake[i].y = oldy;
			oldx = x;
			oldy = y;
		}
	}
	if (checkLose(snake[0].x, snake[0].y, snake))
		return true;
	return false;
}

bool snakeMoveClient(vector<snakeBlockClient>&snakeclient, short direclient[2])
{
	int oldxclient, oldyclient, xclient, yclient;
	gotoxyClient(snakeclient[snakeclient.size() - 1].xClient, snakeclient[snakeclient.size() - 1].yClient);
	cout << " ";
	oldxclient = snakeclient[0].xClient;
	oldyclient = snakeclient[0].yClient;
	snakeclient[0].xClient += direclient[0];
	snakeclient[0].yClient += direclient[1];
	gotoxyClient(snakeclient[0].xClient, snakeclient[0].yClient);
	cout << b;
	if (snakeclient.size() > 1)
	{
		for (int i = 1; i < snakeclient.size(); i++)
		{
			xclient = snakeclient[i].xClient;
			yclient = snakeclient[i].yClient;
			snakeclient[i].xClient = oldxclient;
			snakeclient[i].yClient = oldyclient;
			oldxclient = xclient;
			oldyclient = yclient;
		}
	}
	if (checkLoseClient(snakeclient[0].xClient, snakeclient[0].yClient, snakeclient))
		return true;
	return false;
}

int main()
{
	switch (GameMode()) {
	case 1:
		Sleep(100);
		switch (ChoosePlayer())
		{
		case 1:
			ServerForPlay();
			do {
				GamePlayServer();
			} while (!currentGamePlay);
			break;

		case 2:
			ClientForPlay();
			do {
				system("cls");
				GamePlayClient();
			} while (!currentGamePlay);
			break;
		}
		break;
	case 2:
		do {
			system("cls");
			GamePlayOffline();
		} while (!currentGamePlay);
		break;
	}
}

int GamePlayServer() {
	int IndicatoOFsend = 0;
	int indicatorFirstFryct = 0;
	bool GameIsRunning = true;
	bool GameIsRunningClient = true;
	int GameSpeed = 50;
	int GameSpeedClient = 50;
	short dire[2] = { 0,0 };
	short direClient[2] = { 0,0 };
	vector<snakeBlock> snake;
	vector<snakeBlockClient> snakeclient;
	snakeInit(25, 1, snake);
	snakeInitClient(26, 1, snakeclient);

	if (indicatorFirstFryct == 0) {
		Firstfryct();
		indicatorFirstFryct = 1;
	}
	drawMap(snake);
	drawMapClient(snakeclient);

	while (GameIsRunning)
	{
		IndicatoOFsend = 0;


		if (GetAsyncKeyState(VK_UP))
		{
			if (dire[1] == 0)
			{
				dire[1] = -1;
				dire[0] = 0;
				Send("20");
				IndicatoOFsend = 1;
			}

		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (dire[1] == 0)
			{
				dire[1] = 1;
				dire[0] = 0;

				Send("10");
				IndicatoOFsend = 1;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (dire[0] == 0)
			{
				dire[1] = 0;
				dire[0] = -1;
				Send("02");
				IndicatoOFsend = 1;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (dire[0] == 0)
			{
				dire[1] = 0;
				dire[0] = 1;
				Send("01");
				IndicatoOFsend = 1;
			}
		}
		if (IndicatoOFsend == 0) {
			Send("30");
		}
		ClientHandler();

		if ((xROW == 1 && yCOL == 0) && IndicatorRecv == 1) {
			direClient[1] = xROW;
			direClient[0] = yCOL;
			IndicatorRecv = 0;

		}//left
		else if ((xROW == 0 && yCOL == -1) && IndicatorRecv == 1) {
			direClient[1] = xROW;
			direClient[0] = yCOL;
			IndicatorRecv == 0;
		}//right
		else if ((xROW == 0 && yCOL == 1) && IndicatorRecv == 1) {
			direClient[1] = xROW;
			direClient[0] = yCOL;
			IndicatorRecv == 0;
		}// up
		else if ((xROW == -1 && yCOL == 0) && IndicatorRecv == 1) {
			direClient[1] = xROW;
			direClient[0] = yCOL;
			IndicatorRecv == 0;
		}

		if (snakeMove(snake, dire))
		{
			system("cls");
			cout << "\n\tGame Over" << endl;
			Sleep(2000);
			int GameAgaine = PlayAgaine();
			if (GameAgaine == 1) {
				CleanMap();
				currentGamePlay = false;
				break;
			}
			else if (GameAgaine == 2) {
				currentGamePlay = true;
				return 0;
			}
		}
		if (snakeMoveClient(snakeclient, direClient))
		{
			system("cls");
			cout << "\n\tGame Over" << endl;
			Sleep(2000);
			int GameAgaine = PlayAgaine();
			if (GameAgaine == 1) {
				CleanMap();
				currentGamePlay = false;
				break;
			}
			else if (GameAgaine == 2) {
				currentGamePlay = true;
				return 0;
			}
		}
		Sleep(GameSpeed);
	}
}

int GamePlayClient() {
	int IndicatoOFsend = 0;
	int indicatorFirstFryct = 0;
	bool GameIsRunning = true;
	bool GameIsRunningClient = true;
	int GameSpeed = 50;
	int GameSpeedClient = 50;
	short dire[2] = { 0,0 };
	short direClient[2] = { 0,0 };
	vector<snakeBlock> snake;
	vector<snakeBlockClient> snakeclient;
	snakeInit(25, 1, snake);
	snakeInitClient(26, 1, snakeclient);

	if (indicatorFirstFryct == 0) {
		Firstfryct();
		indicatorFirstFryct = 1;
	}
	drawMap(snake);
	drawMapClient(snakeclient);

	while (GameIsRunning)
	{
		IndicatoOFsend = 0;
		ClientHandlerClient();
		//down
		if ((xROW == 1 && yCOL == 0) && IndicatorRecv == 1) {
			dire[1] = xROW;
			dire[0] = yCOL;
			IndicatorRecv = 0;

		}//left
		else if ((xROW == 0 && yCOL == -1) && IndicatorRecv == 1) {
			dire[1] = xROW;
			dire[0] = yCOL;
			IndicatorRecv == 0;
		}//right
		else if ((xROW == 0 && yCOL == 1) && IndicatorRecv == 1) {
			dire[1] = xROW;
			dire[0] = yCOL;
			IndicatorRecv == 0;
		}// up
		else if ((xROW == -1 && yCOL == 0) && IndicatorRecv == 1) {
			dire[1] = xROW;
			dire[0] = yCOL;
			IndicatorRecv == 0;
		}

		if (GetAsyncKeyState(VK_UP))
		{
			if (direClient[1] == 0)
			{
				direClient[1] = -1;
				direClient[0] = 0;
				SendClient("20");
				IndicatoOFsend = 1;
			}

		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (direClient[1] == 0)
			{
				direClient[1] = 1;
				direClient[0] = 0;
				SendClient("10");
				IndicatoOFsend = 1;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (direClient[0] == 0)
			{
				direClient[1] = 0;
				direClient[0] = -1;
				SendClient("02");
				IndicatoOFsend = 1;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (direClient[0] == 0)
			{
				direClient[1] = 0;
				direClient[0] = 1;
				SendClient("01");
				IndicatoOFsend = 1;
			}
		}
		if (IndicatoOFsend == 0) {
			SendClient("30");
		}
		if (snakeMove(snake, dire))
		{
			system("cls");
			cout << "\n\tGame Over" << endl;
			Sleep(2000);
			int GameAgaine = PlayAgaine();
			if (GameAgaine == 1) {
				CleanMap();
				currentGamePlay = false;
				break;
			}
			else if (GameAgaine == 2) {
				currentGamePlay = true;
				return 0;
			}
		}
		if (snakeMoveClient(snakeclient, direClient))
		{
			system("cls");
			cout << "\n\tGame Over" << endl;
			Sleep(2000);
			int GameAgaine = PlayAgaine();
			if (GameAgaine == 1) {
				CleanMap();
				currentGamePlay = false;
				break;
			}
			else if (GameAgaine == 2) {
				currentGamePlay = true;
				return 0;
			}
		}
		Sleep(GameSpeed);
	}
}

int GamePlayOffline() {
	int indicatorFirstFryct = 0;
	bool GameIsRunning = true;
	int GameSpeed = 100;
	short dire[2] = { 0,0 };
	vector<snakeBlock> snake;
	snakeInit(25, 1, snake);
	if (indicatorFirstFryct == 0) {
		Firstfryct();
		indicatorFirstFryct = 1;
	}
	drawMap(snake);
	while (GameIsRunning)
	{
		if (GetAsyncKeyState(VK_UP))
		{
			if (dire[1] == 0)
			{
				dire[1] = -1;
				dire[0] = 0;
			}

		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			if (dire[1] == 0)
			{
				dire[1] = 1;
				dire[0] = 0;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			if (dire[0] == 0)
			{
				dire[1] = 0;
				dire[0] = -1;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			if (dire[0] == 0)
			{
				dire[1] = 0;
				dire[0] = 1;
			}
		}
		if (snakeMove(snake, dire))
		{
			system("cls");
			cout << "\n\tGame Over" << endl;
			Sleep(2000);
			int GameAgaine = PlayAgaine();
			if (GameAgaine == 1) {
				CleanMap();
				currentGamePlay = false;
				break;
			}
			else if (GameAgaine == 2) {
				currentGamePlay = true;
				return 0;
			}
		}
		Sleep(GameSpeed);
	}
}

void SendClient(string xy) {
	string msg1 = xy;
	int msg_size = msg1.size();
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, msg1.c_str(), msg_size, NULL);
	Sleep(10);
}

void Send(string xy) {
	string msg1 = xy;
	int msg_size = msg1.size();
	send(newConnection, (char*)&msg_size, sizeof(int), NULL);
	send(newConnection, msg1.c_str(), msg_size, NULL);
}

void ServerForPlay() {
	string MyIP;
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}
	MyIP = CurrentMyIP();
	system("cls");
	cout << "\n\tServer IP: " << MyIP << endl;
	cout << "\n\twaiting for player 2 to connect ... " << endl;
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1111);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);;
	if (newConnection == 0) {
		cout << "Error #2\n";
	}
	else {
		system("cls");
		cout << "\n\tClient Connected!\n";
		Sleep(1500);
		system("cls");
		string msg1;
	}
}

string CurrentMyIP() {
	string line;
	string line2;
	ifstream IPFile;
	int offset;
	const char*  search0 = "IPv4";
	const char* search1 = "Ethernet adapter Ethernet";
	system("ipconfig > ip.txt");
	IPFile.open("ip.txt");
	if (IPFile.is_open()) {
		while (!IPFile.eof())
		{
			getline(IPFile, line2);
			if (offset = line2.find(search1) != string::npos)
			{
				while (!IPFile.eof())
				{
					getline(IPFile, line);
					if ((offset = line.find(search0)) != string::npos) {
						line.erase(0, 39);
						return line;
						IPFile.close();
						break;
					}
				}
			}
		}
	}
}

int ClientForPlay() {
	string InPutIPofServer;
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}
	system("cls");
	cout << "\n\tEnter the server IP:" << endl;
	cin >> InPutIPofServer;
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(InPutIPofServer.c_str());
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n";
		return 1;
	}
	system("cls");
	cout << "\n\tConnected!\n";
	Sleep(1500);
	system("cls");
	string msg1;
}

int GameMode() {
	int gameMode;
	cout << "\n\tWelcome to the Snake game" << endl << endl;
	cout << "\tPress UP or DOWN" << endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_UP))
		{  // вверх
			system("cls");
			gameMode = 1;
			cout << "\n\tChoose Game:\n" << endl;
			cout << "\t" << (char)254 << " Online " << (char)254 << endl;
			cout << "\t  Offline " << endl;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			system("cls");
			cout << "\n\tChoose Game:\n" << endl;
			cout << "\t  Online " << endl;
			cout << "\t" << (char)254 << " Offline " << (char)254 << endl;
			gameMode = 2;
		}
		else if (GetAsyncKeyState(VK_RETURN))
		{
			return gameMode;
		}
		Sleep(10);
	}
}

int ChoosePlayer() {
	int numberOfplayer;
	int jopa = 0;
	system("cls");
	cout << "\n\tChoose the player:\n" << endl;
	cout << "\t" << (char)254 << " Player 1(Server) " << (char)254 << endl;
	cout << "\t  Player 2(Client) " << endl;
	numberOfplayer = 1;
	while (true)
	{
		if (GetAsyncKeyState(VK_UP))
		{  // вверх
			system("cls");
			cout << "\n\tChoose the player:\n" << endl;
			cout << "\t" << (char)254 << " Player 1(Server) " << (char)254 << endl;
			cout << "\t  Player 2(Client) " << endl;
			numberOfplayer = 1;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			system("cls");
			cout << "\n\tChoose the player:\n" << endl;
			cout << "\t  Player 1(Server) " << endl;
			cout << "\t" << (char)254 << " Player 2(Client) " << (char)254 << endl;
			numberOfplayer = 2;
		}
		else if (GetAsyncKeyState(VK_RETURN) && jopa != 0)
		{
			return numberOfplayer;
		}

		Sleep(10);
		jopa++;
	}
}


void ClientHandler() {
	int msg_size;

	recv(newConnection, (char*)&msg_size, sizeof(int), NULL);
	char *msg = new char[msg_size + 1];
	msg[msg_size] = '\0';
	recv(newConnection, msg, msg_size, NULL);
	GetCoordinat(msg);
	delete[] msg;

}

void ClientHandlerClient() {
	int msg_size;

	recv(Connection, (char*)&msg_size, sizeof(int), NULL);
	char *msg = new char[msg_size + 1];
	msg[msg_size] = '\0';
	recv(Connection, msg, msg_size, NULL);
	GetCoordinat(msg);
	delete[] msg;

}

void GetCoordinat(string massage) {
	xROW = massage[0] - 48;
	yCOL = massage[1] - 48;
	if (xROW != 3) {
		IndicatorRecv = 1;
	}
	if (xROW == 2) {
		xROW = -1;
	}
	if (yCOL == 2) {
		yCOL = -1;
	}
}

int PlayAgaine() {
	int options;
	system("cls");
	cout << "\n\tTry again?" << endl;
	cout << "\t" << (char)254 << " Yes " << (char)254 << endl;
	cout << "\t  No " << endl;
	options = 1;
	while (true)
	{
		if (GetAsyncKeyState(VK_UP))
		{  
			system("cls");
			cout << "\n\tTry again?" << endl;
			cout << "\t" << (char)254 << " Yes " << (char)254 << endl;
			cout << "\t  No " << endl;
			options = 1;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			system("cls");
			cout << "\n\tTry again?" << endl;
			cout << "\t  Yes " << endl;
			cout << "\t" << (char)254 << " No " << (char)254 << endl;
			options = 2;
		}
		else if (GetAsyncKeyState(VK_RETURN))
		{
			return options;
		}
		Sleep(100);
	}
}

void CleanMap() {
	for (int i = 1; i < 17; i++)
	{
		for (int j = 1; j < 55; j++)
		{
			Map[i][j] = ' ';
		}
	}
}
