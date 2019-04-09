
##**Игра "Змейка" с возможностью играть онлайн с партнером**
***Описание***
Игрок управляет змей, которое ползает по плоскости 
(ограниченной стенками), собирая фрукты, избегая столкновения с собственным хвостом и краями игрового поля. 
Игрок управляет направлением движения головы змеи (вверх, вниз, влево, вправо), 
а хвост змеи движется следом. При подборе фрукта увеличивается размер змеи и количество очков.
**Онлайн режим**
При выборе онлайн режима, задача игроков постоянно охотятся на фрукт, который достанется только одному всячески избегая столкновения с собственным хвостом и краями игрового поля. Победитель тот кто собрал максимальное количество фруктов до момента столкновения с собственным хвостом и краями игрового поля одного из игроков.

##Описание кода игры

``` cpp
#include "pch.h" // Библиотека предкомпилированных заголовков
#include <iostream> // Библиотека ввода и вывода информации
#include <winsock2.h> // Библиотека для работы с сокетами (работе по сети)
#pragma comment(lib, "ws2_32.lib") // Подключаем 
#pragma warning(disable: 4996) // Игнорируем предупреждение
```
##Глобальный переменные:
``` cpp
int xROW; // координата х
int yCOL; // координата у
int counte = 0; // счетчик подсчета очков 1го игрока
int counteClient = 0; // счетчик подсчета очков 2го игрока
int MapW, MapH;
int MapWclient, MapHclient;
```
##Функции:
``` cpp
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
```
##Игровое полк
``` cpp
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
```
##Функция управления змейкой
``` cpp
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
```
##Функция отвечающая за движения 
``` cpp
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
```
##Функция проверки столкновения с собственным хвостом и краями игрового поля
``` cpp
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
```
##Функция вывода вывода изображения на консоль
``` cpp

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
```
##Функция используется сервером для подключения клиентом в случаи выбора режима онлайн
``` cpp
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
```
##Функция выводит на экран IP адрес сервера для подключения клиента 
``` cpp
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
```
##Функция используется клиентом для подключения к серверу в случаи выбора режима онлайн
``` cpp
int ClientForPlay() {
	string InPutIPofServer;
	//WSAStartup
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
```
##Функция для выбора режима игры (оффлайн, онлайн)
``` cpp
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
			//Sleep(5000);
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			system("cls");
			cout << "\n\tChoose Game:\n" << endl;
			cout << "\t  Online " << endl;
			cout << "\t" << (char)254 << " Offline " << (char)254 << endl;
			gameMode = 2;

			//Sleep(5000);

			//CleanBufferGetch(numberOfplayer);

		}
		else if (GetAsyncKeyState(VK_RETURN))
		{
			return gameMode;
		}
		Sleep(10);
	}
}
```
