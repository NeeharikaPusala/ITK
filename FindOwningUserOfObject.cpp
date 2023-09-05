#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <iostream>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* help = ITK_ask_cli_argument("-h");
	char* cError = NULL;
	int iFail = 0;
	tag_t tOwner = NULLTAG;
	tag_t tItem = NULLTAG;
	int count = 0;
	char* owner_name = NULL;
	char* search_id = ITK_ask_cli_argument("-item_id=");

	if (help)
	{
		cout << "This is an ITK utility file\n";
		cout << "You must provide username, password, and group arguments in order to execute this file.\n";
		cout << "Options:\n";
		cout << "   -h ---- for help\n";
	}
	else if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>\n  Type -h for Showing help";
	}
	else
	{
		char* username = ITK_ask_cli_argument("-u=");
		char* password = ITK_ask_cli_argument("-p=");
		char* group = ITK_ask_cli_argument("-g=");

		if (username && password && group)
		{
			int iFail = ITK_init_module(username, password, group);

			if (iFail == ITK_ok)
			{
				cout << "Login successful" << endl;
				if (search_id)
				{
					iFail = ITEM_find_item(search_id, &tItem);
					if (tItem != NULL)
					{
						cout << "Item found successfully " << endl;
						iFail = AOM_ask_owner(tItem, &tOwner);
						if (tOwner != NULLTAG)
						{
							cout << "owner found successfully " << endl;
							iFail = AOM_ask_value_string(tOwner, "user_id", &owner_name);
							if (iFail == ITK_ok)
							{
								cout << owner_name << endl;
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
					cout << "Provide item_id " << endl;
				}
			}
			else
			{
	
				EMH_ask_error_text(iFail, &cError);
				cout << "Login failed: " << cError << endl;
			}
		}
		else
		{
			cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group> -item_id= <item_id>  -h---for help" << endl;
		}
	}

	return iFail;
}