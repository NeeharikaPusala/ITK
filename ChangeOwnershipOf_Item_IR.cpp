#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <sa/user.h>
#include <sa/group.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tGroup = NULLTAG;
	if (cUsername && cPassword && cGroup)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			char* item_id = ITK_ask_cli_argument("-item_id=");
			if (item_id)
			{
				iFail = ITEM_find_item(item_id, &tItem);
				if(iFail == ITK_ok && tItem != NULLTAG)
				{
					cout << "Item found successfully" << endl;
					iFail = SA_find_user2("demo", &tUser);
					if (iFail == ITK_ok && tUser != NULLTAG)
					{
						cout << "user found successfully" << endl;
						iFail = SA_find_group("My Group", &tGroup);
						if (iFail == ITK_ok && tGroup != NULLTAG)
						{
							cout << "cGroup found successfully" << endl;
							iFail = AOM_set_ownership(tItem, tUser, tGroup);
							if (iFail == ITK_ok)
							{
								cout << "ownership changed successfully\n";
								iFail = AOM_save(tItem);
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
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;

				}

			}
			else
			{
				cout << "Usage: " << argv[0] << " -item_id= <item_id>" << endl;
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
		cout << "Usage: " << argv[0] << " -cUsername= <cUsername> -cPassword= <cPassword> -cGroup= <cGroup>" << endl;
	}

	return iFail;
}