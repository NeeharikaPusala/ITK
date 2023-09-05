#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/aom_prop.h>

using namespace std;
int ITK_user_main(int argc, char*argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tRevision = NULLTAG;
	char* cDescrption_property = NULL;
	iFail = ITK_auto_login();
	if (iFail == ITK_ok)
	{
		cout << "Login Successfull" << endl;
		iFail = ITEM_find_rev("000000", "a", &tRevision);
		if (tRevision != NULL)
		{
			cout << "Revision Found Successfully" << endl;
			iFail = AOM_ask_value_string(tRevision, "object_desc", &cDescrption_property);
			if (cDescrption_property != NULL && iFail == ITK_ok)
			{
				cout << "Description value : " << cDescrption_property << endl;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << "Item revision search failed. Error: " << cError << endl;
		}
	}
	else
	{
		EMH_ask_error_text(iFail, &cError);
		cout << "Item revision search failed. Error: " << cError << endl;
	}
	return iFail;
}