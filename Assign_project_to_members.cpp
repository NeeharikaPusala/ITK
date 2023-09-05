#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <sa/user.h>
#include <tccore/project.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cProject_id = ITK_ask_cli_argument("-cProject_id=");
	tag_t tUser = NULLTAG;
	tag_t tProject = NULLTAG;
	int iMember_count = 0;
	tag_t* tMembers = NULL;
	int iAdmin_count = 0;
	tag_t* tAdmin_tags = NULL;
	int iPrivileged_user_count = 0;
	tag_t* tPrivileged_user_tags = NULL;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			if (cProject_id)
			{
				iFail = PROJ_find(cProject_id, &tProject);
				if (iFail == ITK_ok && tProject != NULLTAG)
				{
					cout << "Project found successfully " << endl;
					iFail = SA_find_user2("demo", &tUser);
					if (iFail == ITK_ok && tUser != NULLTAG)
					{
						iFail = PROJ_ask_team(tProject, &iMember_count, &tMembers, &iAdmin_count, &tAdmin_tags, &iPrivileged_user_count, &tPrivileged_user_tags);
						if (iFail == ITK_ok && iMember_count != 0)
						{
							cout << "Members in the project are : " << iMember_count << endl;
							iFail = PROJ_add_members(tProject, 1, &tUser);
							if (iFail == ITK_ok)
							{
								cout << "User added successfully " << endl;
								iFail = PROJ_ask_team(tProject, &iMember_count, &tMembers, &iAdmin_count, &tAdmin_tags, &iPrivileged_user_count, &tPrivileged_user_tags);
								cout << "No of users in the project after adding : " << iMember_count << endl;
							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << "Not able to add user : " << cError << endl;
							}
						}
						else
						{
							EMH_ask_error_text(iFail, &cError);
							cout << "No users in the project : " << cError << endl;
						}
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << "user not found : " << cError << endl;
					}
				}
				else
				{
					EMH_ask_error_text(iFail, &cError);
					cout << "Project not found : " << cError << endl;
				}
			}
			else
			{
				cout << "Provide project id " << endl;
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}

	return iFail;
}