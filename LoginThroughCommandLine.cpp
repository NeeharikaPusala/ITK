#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(username, NULL) == 1 || tc_strcmp(username, "") == 1 || tc_strcmp(password, NULL) == 1 || tc_strcmp(password, "") == 1 || tc_strcmp(group, NULL) == 1 || tc_strcmp(group, "") == 1)
	{
		iFail = ITK_init_module(username, password, group);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>" << endl;
	}

	return iFail;
}