#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tc/folder.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tFolder = NULLTAG;
	iFail = ITK_auto_login();
	{
		if (iFail == ITK_ok)
		{
			cout << "Login success" << endl;
			iFail = FL_create2("ITK", "itk_folder_created", &tFolder);
			if (tFolder != NULLTAG)
			{
				cout << "folder created successfully" << endl;
				iFail = AOM_save(tFolder);
				if (iFail == ITK_ok)
				{
					cout << "Folder saved successfully" << endl;

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
	

	return iFail;
}