#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tc/preferences.h>
#include <fclasses/tc_string.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	char* pref_name = ITK_ask_cli_argument("-pref_name=");
	char* search_id = ITK_ask_cli_argument("-search_id=");
	char* value = NULL;
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	if (tc_strcmp(username, NULL) == 1 || tc_strcmp(username, "") == 1 || tc_strcmp(password, NULL) == 1 || tc_strcmp(password, "") == 1 || tc_strcmp(group, NULL) == 1 || tc_strcmp(group, "") == 1
		|| tc_strcmp(pref_name, NULL) == 1 || tc_strcmp(pref_name, "") == 1 || tc_strcmp(search_id, NULL) == 1 || tc_strcmp(search_id, "") == 1)
	{
		iFail = ITK_init_module(username, password, group);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_item(search_id, &tItem);
			if (iFail == ITK_ok && tItem != NULLTAG)
			{
				cout << "Item found successfully \n";
				iFail = PREF_ask_char_value(pref_name, 0, &value);
				cout << value << endl;
				if (iFail == ITK_ok && value != NULL)
				{
					cout << "value found successfully " << endl;
					AOM_refresh(tItem, true);
					iFail = AOM_set_value_string(tItem, "object_desc", value);
					if (iFail == ITK_ok)
					{
						cout << "Value updated successfully" << endl;
					
						AOM_save_without_extensions(tItem);
						AOM_refresh(tItem, false);
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
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group> -pref_name= <preference_name> -search_id = <item_id>" << endl;
	}

	return iFail;
}