#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/grm.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	char* cRev_id = ITK_ask_cli_argument("-rev_id=");
	int iFail = 0;
	char* cError = NULL;
	tag_t tRevision = NULLTAG;
	tag_t tRelation = NULLTAG;
	tag_t tRel_tag = NULLTAG;
	int iCount = 0;
	tag_t* tSecondary_objects = NULL;
	if (cUsername && cPassword && cGroup)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			if(cItem_id)
			{
				iFail = ITEM_find_rev(cItem_id, "A", &tRevision);
				if (iFail == ITK_ok && tRevision != NULLTAG)
				{
					cout << "Item Revision found successfully\n";
					iFail = GRM_find_relation_type("IMAN_reference", &tRelation);
					if (iFail == ITK_ok && tRelation != NULLTAG)
					{
						cout << "Relation found successfully\n";
						iFail = GRM_list_secondary_objects_only(tRevision, tRelation, &iCount, &tSecondary_objects);
						
						if (iFail == ITK_ok && iCount > 0 && tSecondary_objects != NULL)
						{
							cout << "secondary objects \n";
							for (int i = 0; i < iCount; i++)
							{
								cout << "secondary object" << i + 1 << ":" << tSecondary_objects[i] << endl;
								iFail = GRM_find_relation(tRevision, tSecondary_objects[i], tRelation, &tRel_tag);
								if (iFail == ITK_ok && tRel_tag != NULLTAG)
								{

									iFail = GRM_delete_relation(tRel_tag);
									if (iFail == ITK_ok)
									{
										cout << "Relation deleted successfully" << endl;
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
				cout << "Provide -Item_id=<cItem_id>\n";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}	
	return iFail;
}