#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <sa/user.h>
#include <tccore/grm.h>
#include <tc/folder.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tUser = NULLTAG;
	tag_t tHomeFolder = NULLTAG;
	tag_t tRelation = NULLTAG;
	tag_t tFolder = NULLTAG;
	char* fName = ITK_ask_cli_argument("-name=");
	const char * cRel = "contents";
	iFail = ITK_auto_login();
	if (iFail == ITK_ok)
	{
		cout << "Login Successfull" << endl;
		iFail = SA_find_user2("test", &tUser);
		if (tUser != NULLTAG && iFail == ITK_ok)
		{
			cout << "user found" << endl;
			iFail = SA_ask_user_home_folder(tUser, &tHomeFolder);
			if (tHomeFolder != NULLTAG && iFail == ITK_ok)
			{
				cout << "user home folder found" << endl;
				iFail = FL_create2(fName, "new folder itk", &tFolder);
				{
					if (tFolder != NULLTAG && iFail == ITK_ok)
					{
						cout << "Folder created successfully" << endl;
						iFail = AOM_save(tFolder);
						if (iFail == ITK_ok)
						{
							cout << "folder saved successfully" << endl;
							iFail = FL_insert(tHomeFolder, tFolder, 999);
							if (iFail == ITK_ok)
							{
								cout << "folder inserted successfully" << endl;
								iFail = AOM_save(tHomeFolder);
							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << cError;
							}

						}
						else
						{
							EMH_ask_error_text(iFail, &cError);
							cout << cError;
						}
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError;
					}
				}
				
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError;
		}
	}
	else
	{
		EMH_ask_error_text(iFail, &cError);
		cout << cError;
	}
	return iFail;
}