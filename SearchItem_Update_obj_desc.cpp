#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>

using namespace std;


 int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	const char* cItem_id =  "000000";
	const char* cObject_desc = "Neeha Description";

	iFail = ITK_init_module("test", "test", "dba");
	if (iFail == ITK_ok)
	{
		cout << "Login success" << endl;

		iFail = ITEM_find_item(cItem_id, &tItem);
		if (iFail == ITK_ok && tItem != NULLTAG)
		{
			cout << "Item found successfully" << endl;

			iFail = ITEM_set_description(tItem,"Neeha Description");
			if (iFail == ITK_ok)
			{
				cout << "Description Updated Successfully" << endl;
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

		ITK_exit_module(true); // Clean up after usage
	}
	else
	{
		EMH_ask_error_text(iFail, &cError);
		cout << cError;
	}

	return iFail;
}