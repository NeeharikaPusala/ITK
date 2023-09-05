#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/workspaceobject.h>
#include <tccore/aom_prop.h>
#include <tc/folder.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	char* name1 = ITK_ask_cli_argument("-name1=");
	char* name2 = ITK_ask_cli_argument("-name2=");
	char* object_type = NULLTAG;
	int hits = 0;
	tag_t* tList1 = NULL;
	tag_t* tList2 = NULL;
	int iFail = 0;
	char* cError = NULL;
	if (username && password && group )
	{
		iFail = ITK_init_module(username, password, group);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = WSOM_find2(name1, &hits, &tList1);
			if (tList1 != NULL)
			{
				cout << "List found " << endl;
				for (int i = 0; i < hits; i++)
				{
					cout << tList1[i] << endl;
					iFail = AOM_ask_value_string(tList1[i], "object_type", &object_type);
					if (object_type != NULLTAG)
					{

						if (strcmp(object_type, "Folder") == 0)
						{
							cout << "Folder1 found : " << tList1[i] << endl
							
						}
						else
						{
							cout << "folder not found \n";
						}
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}

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
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;

		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group> -name1= <name> - name2= <name> " << endl;
	}

	return iFail;
}