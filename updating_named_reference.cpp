#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include  <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <ae/dataset.h>
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
	int ifail = 0;
	tag_t tDataset = NULLTAG;
	tag_t* tValue = NULL;
	int iCount = 0;
	char* cName = NULL;
	
	
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		ifail = ITK_init_module(cUsername, cPassword, cGroup);
		if (ifail == ITK_ok)
		{
			
			cout << "Login Success" << endl;
			ifail = AE_find_dataset2("NamedRef", &tDataset);
			if (ifail == ITK_ok && tDataset != NULLTAG)
			{
				AOM_refresh(tDataset, true);
				cout << "dataset found " << "\n";
				ifail = AOM_ask_value_tags(tDataset, "ref_list",&iCount, &tValue);
				if (ifail == ITK_ok && tValue != NULL)
				{
					for (int i = 0; i < iCount; i++)
					{
						ifail = AOM_ask_value_string(tValue[i], "object_string", &cName);
						if (ifail == ITK_ok && cName != NULL)
						{
							cout << cName << "\n";
							if (i == 0)
							{
								AOM_refresh(tValue[i], true);
								ifail = AOM_set_value_string(tValue[i], "original_file_name", "task.pdf");
								if (ifail == ITK_ok)
								{
									cout << "value updated " << "\n";
								}
								else
								{
									printError(ifail);
								}
								AOM_save_without_extensions(tValue[i]);
								AOM_refresh(tValue[i], false);
								AOM_save_without_extensions(tDataset);
								AOM_refresh(tDataset, false);
							}
							else
							{
								continue;
							}
						}
						
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
			printError(ifail);
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}

	return ifail;
}