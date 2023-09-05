#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore/item.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <iostream>
#include <lov/lov.h>


using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* cHelp = ITK_ask_cli_argument("-h");
	char* cItem_id = ITK_ask_cli_argument("-cItem_id=");
	char* cProperty_value = NULL;
	int iCount = NULL;
	tag_t tLov = NULLTAG;
	char** cValues = NULL;
	char* cError = NULL;
	int iFail = 0;
	tag_t tItem = NULLTAG;

	if (cHelp)
	{
		cout << "This is an ITK utility file\n";
		cout << "You must provide username, password, and group arguments in order to execute this file.\n";
		cout << "Options:\n";
		cout << "   -h ---- for cHelp\n";
	}
	else if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>\n  Type -h for Showing cHelp";
	}
	else
	{
		char* username = ITK_ask_cli_argument("-u=");
		char* password = ITK_ask_cli_argument("-p=");
		char* group = ITK_ask_cli_argument("-g=");

		if (username && password && group)
		{
			iFail = ITK_init_module(username, password, group);

			if (iFail == ITK_ok)
			{
				cout << "Login successful" << endl;
				if (cItem_id)
				{
					iFail = ITEM_find_item(cItem_id, &tItem);
					if (iFail == ITK_ok && tItem != NULLTAG)
					{
						cout << "Item found successfully" << endl;
						iFail = AOM_ask_lov(tItem, "a2_cascade_lov", &tLov);
						if (tLov != NULLTAG && iFail == ITK_ok)
						{

							iFail = LOV_ask_values_string(tLov, &iCount, &cValues);
							if (cValues != NULL && iFail == ITK_ok)
							{
								AOM_refresh(tItem, true);
								for (int i = 0; i < iCount; i++)
								{
									cout << "Value " << i + 1 << ": " << cValues[i] << endl;
									AOM_set_value_string(tItem, "a2_cascade_lov", cValues[1]);
								}
								cout << "updated successfully" << endl;
								AOM_save_without_extensions(tItem);
								AOM_refresh(tItem, false);
								MEM_free(cValues);
							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl; cout << "No LOVs with the given name" << endl;
							}
						}
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl; cout << "No LOVs with the given name" << endl;
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
				cout << "provide " << " -cItem_id = <cItem_id> -h---for cHelp" << endl;
			}
		}
		else
		{

			EMH_ask_error_text(iFail, &cError);
			cout << "Login failed: " << cError << endl;
		}
	}


	return iFail;
}