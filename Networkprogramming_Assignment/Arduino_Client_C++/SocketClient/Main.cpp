/*
 * Projectwork by Jimmy & Yacoub, IOT19
 * Course: NÃĪtvÃĪrksprogrammering 20 YHP
 * Teacher: Aksel Forsberg
 * Program: Visual Studio CE 2019 and MySQL CE 8.0 including Workbench
 * Language: Server: C#
 *           Application-client: C#
 *           Arduino-client (simulation): C++
 *
 * This is the arduino-simulation in our solution where we in a loop send a string-query via socket to the server wich uploads it into the database.
 * Ideally we would not have put MySQL-code in this client, but to handle incoming variable on the serverside. Due to the timeframe we did it this way.
 * We have also (due to the timeframe) not included any object to handle real-time.
 * The simulation acts like an arduino, wich sends a temperature-value, with this we have also added information like: ID, Location, SensorType, unit and time.
 */

#include <chrono>
#include <thread>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "111.111.11.11"; //Put the servers public IP-adress
	int port = 99999;

	string ID = "CL1";
	string location = "Malmö";
	string sensorType = "Temperatur";			 
	string unit = "C";
	string timeOfMeasure = "";
	string output = "";
	
	// Initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0)
	{
		cerr << "cant start winsock, err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Cant create socket, err #" << WSAGetLastError() << endl;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Cant connect to server, err# " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::chrono::seconds interval(10);
	for (int i = 0; i < 10; i++)
	{
		int value = 0;
		srand(time(NULL));
		int tempValue = rand() % 90 + 10;
		value = tempValue;
		string strValue = to_string(value);		
		string output =  ID + " " + location + " " + sensorType  + " " + strValue + " " + unit;
		output = "INSERT INTO networkprogramming . clienproject (`id`, `location`, `sensorType`, `sesorvalue`, `unit`, `timeOfMeasure`) VALUES('" + ID + "', '" + location + "', '" + sensorType + "', '" + strValue + "', '" + unit + "', '" + "2021-01-15 13:14:09" + "')";
		send(sock, output.c_str(), output.size(), 0);
		cout << "String sent to server: "  << endl;
		output = "0";
		std::this_thread::sleep_for(interval);
	}

	// Close everything 
	output = "0";	
	send(sock, output.c_str(), output.size(), 0);
	
	closesocket(sock);
	WSACleanup();
}