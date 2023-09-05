#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <tccore/workspaceobject.h>
#include <tccore/aom_prop.h>
#include <string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-cItem_id=");
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	int iNreferencers = 0;
	int *nLevels = 0;
	tag_t *tReferencers = NULL;
	char** cRelations = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_item(cItem_id, &tItem);
			if (tItem != NULL && iFail == ITK_ok)
			{
				cout << "Item found \n";
				iFail = WSOM_where_referenced2(tItem, -1, &iNreferencers, &nLevels, &tReferencers, &cRelations);
				if (iFail == ITK_ok && iNreferencers > 0)
				{
					cout << "Reference Objects:" << endl;
					for (int i = 0; i < iNreferencers; i++)
					{
						char* refObjectName = NULL;
						iFail = AOM_ask_value_string(tReferencers[i], "object_name", &refObjectName);
						if (iFail == ITK_ok)
						{
							cout << refObjectName << endl;
							MEM_free(refObjectName); // Free the memory allocated by AOM_ask_value_string
						}
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -cItem_id= <cItem_id>" << endl;
	}

	return iFail;
}