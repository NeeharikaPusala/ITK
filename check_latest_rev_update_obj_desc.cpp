#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
using namespace std;

void printError(int error)
{
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << "\n";
}
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	int ifail = 0;
	tag_t tValue = NULLTAG;
	int iCount = 0;
	char* cReleaseName = NULL;
	tag_t* tRev_List = NULL;
	tag_t tItem = NULLTAG;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		ifail = ITK_init_module(cUsername, cPassword, cGroup);
		if (ifail == ITK_ok)
		{
			cout << "Login Success" << endl;
			if (cItem_id)
			{
				ifail = ITEM_find_item(cItem_id, &tItem);
				if (ifail == ITK_ok && tItem != NULLTAG)
				{
					ifail = ITEM_list_all_revs(tItem, &iCount, &tRev_List);
					cout << "iCount : " << iCount << "\n";
					if (ifail == ITK_ok && iCount != 0 && tRev_List != NULL)
					{
					
						for (int i = 0; i < iCount; i++)
						{
							
							if (i == iCount-1)
							{
								ifail = AOM_UIF_ask_value(tRev_List[i], "release_status_list", &cReleaseName);
								if (ifail == ITK_ok && cReleaseName != NULL)
								{
									cout << "cReleaseName : " << cReleaseName << "\n";
									if (tc_strcmp(cReleaseName,"")!=0)
									{
										ifail = AOM_refresh(tRev_List[i], true);
										ifail = AOM_set_value_string(tRev_List[i], "object_desc", "hello");
										if (ifail == ITK_ok)
										{
											cout << "Updated : " << "\n";
											AOM_save_without_extensions(tRev_List[i]);
											AOM_refresh(tRev_List[i], false);
										}
										else
										{
											printError(ifail);
										}
									}
									else
									{
										AOM_refresh(tRev_List[i - 1], true);
										AOM_set_value_string(tRev_List[i - 1], "object_desc", "update1");
										AOM_save_without_extensions(tRev_List[i - 1]);
										AOM_refresh(tRev_List[i - 1], false);

										AOM_refresh(tRev_List[i - 2], true);
										AOM_set_value_string(tRev_List[i - 2], "object_desc", "update2");
										AOM_save_without_extensions(tRev_List[i - 2]);
										AOM_refresh(tRev_List[i - 2], false);
									}
								}
								else
								{
									printError(ifail);
								}
								
							}
							else
							{
								continue;
							}
							
							/*AOM_ask_value_string(tRev_List[i], "object_string", &cReleaseName);
							cout << cReleaseName << endl;*/
						}
					}
					else
					{
						printError(ifail);
					}
				}
				else
				{
					printError(ifail);
				}
			}
			else
			{
				cout << "provide item id " << "\n";
			}
		}
		else
		{
			printError(ifail);
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -item_id= <item id>" << endl;
	}
	MEM_free(tRev_List);
	return ifail;
}