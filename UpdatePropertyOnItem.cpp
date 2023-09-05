#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <res/res_itk.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tUser = NULLTAG;

	iFail = ITK_init_module("test", "test", "dba");
	if (iFail == ITK_ok)
	{
		cout << "login success" << endl;
		iFail = ITEM_find_item("000000", &tItem);
		if (tItem != NULLTAG && iFail == ITK_ok)
		{
			cout << "Item found successfully" << endl;
			iFail = RES_checkout2(tItem, "NULL", "NULL", "NULL", RES_EXCLUSIVE_RESERVE);
			if (iFail == ITK_ok)
			{
				cout << "object checked out successfully" << endl;
				iFail = AOM_set_value_string(tItem, "object_desc", "Neeha");
				if (iFail == ITK_ok)
				{
					cout << "description updated" << endl;
					iFail = RES_checkin(tItem);
					if (iFail == ITK_ok)
					{
						cout << "Check in successfull";
						iFail = AOM_save(tItem);
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
	return iFail;
}