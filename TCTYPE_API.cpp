#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/aom.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cObjectName = ITK_ask_cli_argument("-object_name=");
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	tag_t tItem = NULLTAG;
	tag_t tCreateInput = NULL;
	tag_t tRelation = NULLTAG;
	tag_t tBo = NULLTAG;
	tag_t* tSecondary = NULL;
	int iFail = 0;
	char* cError = NULL;
	
	int iCount = 0;
	
	if (cUsername && cPassword && cGroup)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = TCTYPE_ask_type("Item", &tItem);
			if (tItem != NULLTAG && iFail == ITK_ok)
			{
				cout << "Item type found successfully \n";
				if (cObjectName && cItem_id)
				{
					iFail = TCTYPE_construct_create_input(tItem, &tCreateInput);
					if (iFail == ITK_ok && tCreateInput != NULLTAG)
					{
						cout << "Tag of the input has been created. \n";
						iFail = TCTYPE_create_object(tCreateInput, &tBo);
						if (iFail == ITK_ok && tBo != NULLTAG)
						{
							cout << "object created successfully \n";
							AOM_set_value_string(tBo, "object_name", cObjectName);
							AOM_set_value_string(tBo, "item_id", cItem_id);
							AOM_save_without_extensions(tBo);
							iFail = GRM_find_relation_type("IMAN_master_form", &tRelation);
							if (iFail == ITK_ok && tRelation != NULLTAG)
							{
								cout << "Relation type found successfully \n";
								iFail = GRM_list_secondary_objects_only(tBo, tRelation, &iCount, &tSecondary);
								if (tSecondary != NULL && tSecondary != NULL && iCount !=0 )
								{
									cout << "secondary objects found" << endl;
									cout << "List of secondary objects : \n";
									for (int i = 0; i < iCount; i++)
									{
										cout << tSecondary[i] << endl;
										AOM_set_value_string(tSecondary[i], "user_data_1", "neeha_master_form");
										AOM_save_without_extensions(tSecondary[i]);
										cout << "The value has been set successfully \n";
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
			}
			else
			{
				cout << "Provide object name and Item_id ";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -object_name= <item_name> -item_id= <item_id>" << endl;
	}

	return iFail;
}