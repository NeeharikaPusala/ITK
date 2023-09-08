#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <sa/user.h>
#include <tccore/aom.h>
#include <sa/group.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cUserId = ITK_ask_cli_argument("-user_id=");
	char* cPersonName_user = ITK_ask_cli_argument("-person_name=");
	char* cPassword_user = ITK_ask_cli_argument("-password=");
	char* cGroup_user = ITK_ask_cli_argument("-group=");
	tag_t tUser = NULLTAG;
	tag_t tGroup = NULLTAG;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = SA_create_user2(cUserId, cPersonName_user, cPassword_user, &tUser);
			if (iFail == ITK_ok && tUser != NULLTAG)
			{
				cout << "user created successfully : " << endl;
				iFail = SA_find_group("My Group", &tGroup);
				if (iFail == ITK_ok && tGroup != NULLTAG)
				{
					cout << "Group found successfully : " << endl;
					iFail = SA_set_user_login_group(tUser, tGroup);
					if (iFail == ITK_ok)
					{
						cout << "Login group set successfully : \n";
						iFail = SA_set_os_user_name2(tUser, "Windows");
						if (iFail == ITK_ok)
						{
							cout << "os name set successfully : \n";
							AOM_save_without_extensions(tUser);
						}
						else
						{
							cout << "os name not set : ";
							EMH_ask_error_text(iFail, &cError);
							cout << cError << endl;
						}
					}
					else
					{
						cout << "Login group not set :" << endl;
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}
				}
				else
				{
					cout << "Group not found  : " << endl;
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "user not created : ";
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -user_id= <user id> -person_name= <person_name> -password= <password> -group= <user default group>" << endl;
	}

	return iFail;
}