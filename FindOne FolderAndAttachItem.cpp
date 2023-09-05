#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/workspaceobject.h>
#include <tc/folder.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cFolder_name = ITK_ask_cli_argument("-name=");
	char* cObject_desc = NULL;
	tag_t tItem = NULLTAG;
	int nHits = 0;
	tag_t* tList = NULL;
	int iFail = 0;
	char* cError = NULL;
	if (cUsername && cPassword && cGroup && cFolder_name)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;	
			iFail = WSOM_find2(cFolder_name, &nHits, &tList);
			if (tList != NULL)
			{
				cout << "List found" << endl;
				for (int i = 0; i < nHits; i++)
				{
					cout << tList[i] << endl;
				}
				for (int i = 0; i < nHits; i++)
				{

					iFail = AOM_ask_value_string(tList[i], "cObject_desc", &cObject_desc);
					if (iFail == ITK_ok && cObject_desc)
					{
						cout << "descrption matched : " << cObject_desc << endl;
						if (strcmp(cObject_desc, "neeha") == 0)
						{
							cout << "Object description matched for object : " << tList[i] << endl;
							iFail = ITEM_find_item("000000", &tItem);
							if (tItem != NULLTAG && iFail == ITK_ok)
							{
								cout << "Item found successfully \n";
								iFail = FL_insert(tList[i], tItem, 1);
								if (iFail == ITK_ok)
								{
									cout << "item inserted successfully" << endl;
									iFail = AOM_save(tList[i]);
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
							cout << "Object description did not match." << endl;
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -folder_name=<folder_name> " << endl;
	}

	return iFail;
}