#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <string>
#include <wchar.h>
#include <regex>
#include <stdio.h>
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

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
								Sleep(5000);	// every 10 seconds, pause the display for 5 seconds before resuming with current time
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
