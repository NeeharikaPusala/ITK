#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <fstream>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	char* cError = NULL;
	ofstream logFile("C:\\Users\\T50391\\Downloads\\log.txt");
	if (!logFile.is_open())
	{
		cout << "failed to open log file " << endl;
	}
	if (tc_strcmp(username, NULL) == 1 || tc_strcmp(username, "") == 1 || tc_strcmp(password, NULL) == 1 || tc_strcmp(password, "") == 1 || tc_strcmp(group, NULL) == 1 || tc_strcmp(group, "") == 1)
	{
		iFail = ITK_init_module(username, password, group);
		if (iFail == ITK_ok)
		{
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
			cout << "Login Success" << endl;
			cout << "see the log file at C:\\Users\\T50391\\Downloads\\log.txt" << endl;
			logFile << "Login Success\n";
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			logFile << "------------------------ \n";
			logFile << "Login Failed \n";
			cout << "Login failed \n" << endl;
			cout << "see the log file at C:\\Users\\T50391\\Downloads\\log.txt" << endl;
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
			logFile << cError << endl;
			logFile << "------------------------ \n";
			logFile << "------------------------ \n";
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>" << endl;
	}

	return iFail;
}