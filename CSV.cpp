#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <tccore/aom_prop.h>
#include <fstream>
#include <filesystem>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-cItem_id=");
	tag_t tItem = NULLTAG;
	int iFail = 0;
	char* cError = NULL;
	char* cPropValue = NULL;
	int iCount = 0;
	char** cPropNames = NULL;

	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 ||
		tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 ||
		tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1 ||
		tc_strcmp(cItem_id, NULL) == 1 || tc_strcmp(cItem_id, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_item(cItem_id, &tItem);
			if (iFail == ITK_ok && tItem != NULLTAG)
			{
				cout << "Item found \n";
				iFail = AOM_ask_prop_names(tItem, &iCount, &cPropNames);
				if (iFail == ITK_ok && iCount !=0 && cPropNames != NULL)
				{
					cout << "Properties found \n";
					ofstream csvFile("C:\\Users\\T50391\\Downloads\\item_properties.csv");
					if (csvFile.is_open())
					{
						csvFile << "Property Name,Property Value" << endl;
						for (int i = 0; i < iCount; i++)
						{
							iFail = AOM_ask_value_string(tItem, cPropNames[i], &cPropValue);
							if (iFail == ITK_ok && cPropValue != NULL)
							{
								csvFile << cPropNames[i] << " : " << cPropValue << endl;
							}
						}
						csvFile.close();
						cout << "CSV file created: item_properties.csv" << endl;
						
					}
					else
					{
						cout << "Failed to create CSV file. \n";
					}
				}
				else
				{
					cout << "Failed to get property names \n";
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "Item not found \n";
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		else
		{
			cout << "Login failed \n";
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -cItem_id=<cItem_id>" << endl;
	}

	return iFail;
}
