#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/tctype.h>
#include <tccore/grm.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <vector>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cValue = NULL;
	tag_t tItemRev = NULLTAG;
	int iCount = 0;
	int iGRMCount = 0;
	char* cValueTool = NULL;
	tag_t* tRelationList = NULL;
	vector<tag_t> vSecondaryToolTags;
	vector<tag_t> vRelTags;
	tag_t tValueTool = NULLTAG;
	tag_t* tSecondary_objects = NULL;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_rev("000326", "A", &tItemRev);
			if (iFail == ITK_ok && tItemRev != NULLTAG)
			{
				cout << "Item revision found \n";
				iFail = GRM_list_secondary_objects_only(tItemRev, NULLTAG, &iCount, &tSecondary_objects);
				cout << "number of secondary objects including IMAN master form " << iCount << endl;
				if (iFail == ITK_ok && tSecondary_objects != NULL && iCount != 0)
				{
					for (int i = 0; i < iCount; i++)
					{
						iFail = AOM_ask_value_tag(tSecondary_objects[i], "tool_used", &tValueTool);
						if (iFail == ITK_ok && tValueTool != NULLTAG)
						{
							cout << "Tool found : \n";
							iFail = AOM_ask_value_string(tValueTool, "object_name", &cValueTool);
							if (iFail == ITK_ok && cValueTool != NULL)
							{
								cout << "tool_used : " << cValueTool << endl;
								if (cValueTool != NULL)
								{
									vSecondaryToolTags.push_back(tSecondary_objects[i]);
									for (int i = 0; i < vSecondaryToolTags.size(); i++)
									{
										iFail = GRM_list_relations(tItemRev, vSecondaryToolTags[i], NULLTAG, NULLTAG, &iGRMCount, &tRelationList);
										if (iFail == ITK_ok && iGRMCount > 0 && tRelationList != NULL)
										{
											for (int i = 0; i < iGRMCount; i++)
											{
												vRelTags.push_back(tRelationList[i]);
												for (int i = 0; i < vRelTags.size(); i++)
												{
													iFail = GRM_delete_relation(vRelTags[i]);
													if (iFail == ITK_ok)
													{
														cout << "Relation deleted successfully \n ";
													}
													else
													{
														cout << "relation not deleted \n ";
													}
												}
											}
											
										}
										else
										{
											cout <<"no relation list found \n";
											EMH_ask_error_text(iFail, &cError);
											cout << cError << endl;
										}
									}
								}
								else
								{
									break;
								}
							}
							else
							{
								cout << "no tool value found" <<  endl;
							}
							
						}
						else
						{
							cout << "no tool is associated with object : \n";
							EMH_ask_error_text(iFail, &cError);
							cout << cError << endl;
						}
					}
				}
				else
				{
					cout << "no secondary objects : ";
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "Item revision not found : ";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}

	return iFail;
}