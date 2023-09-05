#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <iostream>
#include <fclasses/tc_string.h>

using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* help = ITK_ask_cli_argument("-h");
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	char* errorText = NULL;

	if (help)
	{
		cout << "This is an ITK utility file\n";
		cout << "You must provide username, password, and group arguments in order to execute this file.\n";
		cout << "Options:\n";
		cout << "   -h ---- for help\n";
	}
	else if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>\n  Type -h for Showing help";
	}
	else if (tc_strcmp(username, NULL) == 1 || tc_strcmp(username, "") == 1 || tc_strcmp(password, NULL) == 1 || tc_strcmp(password, "") == 1 || tc_strcmp(group, NULL) == 1 || tc_strcmp(group, "") == 1)
	{
		

		if (username && password && group)
		{
			int iFail = ITK_init_module(username, password, group);

			if (iFail == ITK_ok)
			{
				cout << "Login successful" << endl;
				ITK_exit_module(true); // Clean up after usage
			}
			else
			{
				
				EMH_ask_error_text(iFail, &errorText);
				cout << "Login failed: " << errorText << endl;
			}
		}
		else
		{
			cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>  -h---for help" << endl;
		}
	}
	else
	{
		cout << "Login unsuccess" << endl;
	}

	return 0;
}
