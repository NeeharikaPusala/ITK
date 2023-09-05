#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/grm.h>
#include <tccore/aom.h>
#include <ae/dataset.h>
#include <base_utils/Mem.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cSearch_id = ITK_ask_cli_argument("-item_id=");
	tag_t tRelation = NULLTAG;
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
				if (iFail == ITK_ok && tRevision != NULLTAG)
				{
					cout << "Item Revision found" << endl;
					int count = 0;
					tag_t* tSecondary_objects = NULLTAG;
					iFail = GRM_find_relation_type("IMAN_specification", &tRelation);
					if (iFail == ITK_ok && tRelation != NULLTAG)
					{
						iFail = GRM_list_secondary_objects_only(tRevision, tRelation, &count, &tSecondary_objects);
						if (iFail == ITK_ok && count > 0)
						{
							cout << "secondary objects : \n";
							for (int i = 0; i < count; ++i)
							{
								cout << "Secondary Object " << i + 1 << ": " << tSecondary_objects[i] << endl;
								iFail = AOM_refresh(tSecondary_objects[i], true);
								iFail = AE_import_named_ref(tSecondary_objects[i], "PDF_Reference", "C:\\Users\\T50391\\Downloads\\Neeharika Pusala(T50391) July.pdf", "payslip1.pdf", 020000);
								iFail = AOM_save(tSecondary_objects[i]);
								iFail = AOM_refresh(tSecondary_objects[i], false);

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
						cout << "Not bale to find relation type :";
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
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