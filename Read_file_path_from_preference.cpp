#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tc/preferences.h>
#include <tccore\item.h>
#include <fstream>>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	char* pref_name = ITK_ask_cli_argument("-pref_name=");
	char* search_id = ITK_ask_cli_argument("-search_id=");
	char* descValue = NULL;
	tag_t tItem = NULLTAG;
	char* value = NULL;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(username, "") == 1 || tc_strcmp(username, NULL) == 1 ||tc_strcmp(password, "") == 1 || tc_strcmp(password, NULL == 1) || tc_strcmp(group, "") == 1 || tc_strcmp(group, NULL == 1))
	{
		iFail = ITK_init_module(username, password, group);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			if (pref_name)
			{
				iFail = PREF_ask_char_value(pref_name, 0, &value);
				if (iFail == ITK_ok && value != NULL)
				{
					cout << "file path : " << value << endl;
					if (search_id)
					{
						iFail = ITEM_find_item(search_id, &tItem);
						if (iFail == ITK_ok && tItem != NULLTAG)
						{
							cout << "Item found successfully : " << tItem << endl;
							ofstream outputFile(value);
							if (outputFile.is_open())
							{
								cout << "File opened \n" << "--------------";
								iFail = AOM_ask_value_string(tItem, "object_desc", &descValue);
								cout << "desc value : " << descValue << endl;
								if (iFail == ITK_ok && descValue != NULL)
								{
									outputFile << "object_desc value : " << descValue << endl;
									outputFile.close();
								}
								else
								{
									cout << "Failed to retrieve object_desc value.";
								}
								
							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << "not able to open file " << cError << endl;
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
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group> -pref_name= <preference_name> -search_id= <item_id>" << endl;
	}

	return iFail;
}