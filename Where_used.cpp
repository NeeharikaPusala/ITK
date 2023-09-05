#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <ps/ps.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
    char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cSearch_id = ITK_ask_cli_argument("-cSearch_id=");
	tag_t tItem = NULLTAG;
	tag_t* tParents = NULL;
	int iNparents = 0;
	int *i_n_levels = 0;
	tag_t tRevision = NULLTAG; 
	int iFail = 0;
	char *cItem_id = NULL;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			if(cSearch_id)
			{
				iFail = ITEM_find_rev(cSearch_id, "A", &tRevision);
				if (iFail == ITK_ok && tRevision != NULLTAG)
				{
					cout << "Item revision found " << endl;
					iFail = PS_where_used_all(tRevision, -1, &iNparents, &i_n_levels, &tParents);
					if (iFail == ITK_ok && iNparents != 0 && tParents != NULL)
					{
						cout << "no. of parents : " << iNparents << endl;
						for (int i = 0; i < iNparents; i++)
							
						{
						//	cout << "Level : " << levels[i] << ", Item : " << tParents[i] << endl;

							AOM_ask_value_string(tParents[i], "cItem_id", &cItem_id);
							cout << "Level : " << i_n_levels[i] << ", Item : " << cItem_id << endl;
						}
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << " parents not found " << cError << endl;
					}
				}
				else
				{
					EMH_ask_error_text(iFail, &cError);
					cout << "Revision not found " <<  cError << endl;
				}
			}
			else
			{
				cout << "Provide cSearch_id -cSearch_id= <cSearch_id> \n";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -cSearch_id= <cSearch_id>" << endl;
	}

	return iFail;
}