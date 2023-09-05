#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/grm.h>
#include <base_utils/Mem.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cSearch_id = ITK_ask_cli_argument("-item_id=");
	tag_t tRevision = NULLTAG;
	int iFail = 0;
	char* cError = NULL;

	if (cUsername && cPassword && cGroup)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;

			if (cSearch_id)
			{
				iFail = ITEM_find_rev(cSearch_id, "A", &tRevision);
				if (iFail==ITK_ok && tRevision != NULLTAG)
				{
					cout << "Item Revision found" << endl;
					int count = 0;
					tag_t* tSecondary_objects = NULLTAG;
					iFail = GRM_list_secondary_objects_only(tRevision, NULLTAG, &count, &tSecondary_objects);
					if (iFail == ITK_ok && count > 0 && tSecondary_objects != NULL)
					{
						cout << "secondary objects : \n";
						for (int i = 0; i < count; ++i)
						{
							cout << "Secondary Object " << i + 1 << ": " << tSecondary_objects[i] << endl;
						}
						if (tSecondary_objects)
						{
							MEM_free(tSecondary_objects);
						}
					}
					else
					{
						cout << "No secondary objects" << endl;
					}
				}
				else
				{
					EMH_ask_error_text(iFail, &cError);
					cout << "Item Revision not found: " << cError << endl;
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