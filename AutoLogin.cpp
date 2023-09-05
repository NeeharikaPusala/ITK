
#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	int fail = 0;
	char* error = NULL;

	// Call ITK_auto_login() for auto-login functionality
	fail = ITK_auto_login();

	if (fail == ITK_ok)
	{
		cout << "Auto Login Successful";
	}
	else
	{
		EMH_ask_error_text(fail, &error);
		cout << "Auto Login Failed. Error: " << error;
	}

	return fail;
}