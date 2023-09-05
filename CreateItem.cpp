#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char *cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tItemRev = NULLTAG;
	iFail = ITK_auto_login();
	if (iFail == ITK_ok)
	{
		cout << "Login Success" << endl;
		iFail = ITEM_create_item("000000", "TOYOTA", "Item","a", &tItem, &tItemRev);
		{
			if (iFail == ITK_ok)
			{
				cout << "Item Created successfully" << endl;;
				iFail = ITEM_save_item(tItem);
				if (iFail == ITK_ok)
				{
					cout << "Item Saved Successfully" << endl;;
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
	return iFail;
}