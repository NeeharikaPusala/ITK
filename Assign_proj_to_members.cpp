#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/project.h>
#include <sa/group.h>
#include <pom/pom/pom.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cProject_id = ITK_ask_cli_argument("-project_id=");
	char* cGroupName = ITK_ask_cli_argument("-group_name=");
	tag_t tProject = NULLTAG;
	tag_t tGroup = NULLTAG;
	int iFail = 0;
	int iNo_GroupMembers = 0;
	char* cValue = NULL;
	tag_t* t_group_members = NULL;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = PROJ_find(cProject_id, &tProject);
			if (iFail == ITK_ok && tProject != NULLTAG)
			{
				cout << "Project found successfully \n";
				iFail = SA_find_group(cGroupName, &tGroup);
				if (iFail == ITK_ok && tGroup != NULLTAG)
				{
					cout << "Group found successfully " << endl;
					iFail = POM_list_group_members(tGroup, &iNo_GroupMembers, &t_group_members);
					if (iFail == ITK_ok && iNo_GroupMembers != 0 && t_group_members != NULL)
					{
						cout << "members count in group : " << iNo_GroupMembers << endl;
						for (int i = 0; i < iNo_GroupMembers; i++)
						{
							iFail = AOM_ask_value_string(t_group_members[i], "user_name", &cValue);
							if (iFail == ITK_ok && cValue != NULL)
							{
								cout << cValue << endl;
								iFail = PROJ_add_members(tProject, iNo_GroupMembers, &t_group_members[i]);
								if (iFail == ITK_ok)
								{
									cout << "member added successfully \n";
									//AOM_save_without_extensions(tProject);
								}
								else
								{
									cout << "Not able to add member : ";
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								cout << "Not able to retrieve the property value : ";
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
					}
					else
					{
						cout << "No members in the group : ";
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}
				}
				else
				{
					cout << "Not able to find group : \n";
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "Not able to find project ";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -project_id= <project id> -group_name= <Group name>" << endl;
	}

	return iFail;
}