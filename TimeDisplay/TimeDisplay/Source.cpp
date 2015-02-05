/*
 * Author: Phi VoBa
 * Last Updated: 02/04/15
 * 
 * TimeDisplay
 * This application displays the system ID and system time.
 * 1) To display this help info, enter "-H", "-h", "-?", or no parameter
 * 2) To display the system time continuously, enter "-Tc" or "-tc"
 * 3) To display the system time with a five second pause every ten second
 *    interval, enter "-Tp" or "-tp"
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <string>
#include <iostream>
#include <wchar.h>
#include <regex>
#include <stdio.h>
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

// Presents program help info to user (filename, revision, contact info, procedure)
void getHelp(char** argv) {
	printf(argv[0]);
	printf("\nVersion 0.0-1\n");
	printf("Contact phivoba@uw.edu\n\n");
	printf("This application displays the system ID and system time.\n");
	printf("1) To display this help info, enter \"-H\", \"-h\", \"-?\", or no parameter\n");
	printf("2) To display the system time continuously, enter \"-Tc\" or \"-tc\"\n");
	printf("3) To display the system time with a five second pause every ten second\n");
	printf("   interval, enter \"-Tp\" or \"-tp\"\n");
}


int main(int argc, char** argv) {
	if (argc == 1) {	// if user passes no parameters
		getHelp(argv);
	}
	else {
		for (int i = 1; i < argc && argv[i][0] == '-'; i++) {	// determine value of each parameter passed
			switch (argv[i][1]) {
			case 'H':	// user passes parameter asking for help info
			case 'h':
			case '?':
				if (argv[i][2] == 0) {
					getHelp(argv);
				}
				else {
					printf("Not a valid parameter");
				}
				break;

			case 'T':	// user passes parameter asking to display time (MM/DD/Year, HH:MM:SS.Millisec) either...
			case 't':
				if (argv[i][3] == 0) {
					char szPath[128] = "";
					WSADATA wsaData;
					WSAStartup(MAKEWORD(2, 2), &wsaData);
					gethostname(szPath, sizeof(szPath));
					printf("%s\n", szPath);
					WSACleanup();

					SYSTEMTIME time;

					/*INITIALIZE (do not display until:)
					  a) the display cycle plateaus (dif between grab and display goes between margin)
				  	  b) next 10 second interval (calculate exact wait time)*/

					// a) the display cycle plateaus (dif between grab and display goes between margin)
					bool noplateau = true;
					while (noplateau) {
						GetLocalTime(&time);
						int ms1 = time.wSecond * 1000 + time.wMilliseconds;
						GetLocalTime(&time);
						int ms2 = time.wSecond * 1000 + time.wMilliseconds;
						if (abs(ms1 - ms2) <= 10) {	// margin of 10ms
							noplateau = false;
						}
					}

					// b) next 10 second interval (calculate exact wait time)
					GetLocalTime(&time);
					int secwait = 10 - time.wSecond % 10;
					int mswait = 1000 - time.wMilliseconds + (secwait * 1000);
					Sleep(mswait);

					switch (argv[i][2]) {
					case 'c':	// ...continuously (constantly updated display of system time)
						while (true) {
							GetLocalTime(&time);
							printf("%02d/%02d/%d, %02d:%02d:%02d.%d\r", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
						}
						break;

					case 'p':	// ...or with a pause (constant display for 5 seconds followed by a static frame for another 5 seconds, repeated)
						while (true) {
							GetLocalTime(&time);
							printf("%02d/%02d/%d, %02d:%02d:%02d.%d\r", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

							if (time.wSecond % 10 == 0) {
								int pause = 5000 - time.wMilliseconds;
								Sleep(pause);	// pause display for the remaining time until the next 10 second interval
							}
						}
						break;
					}
				}
				break;

			default:
				printf("Not a valid parameter");
				break;
			}
		}
	}
}
