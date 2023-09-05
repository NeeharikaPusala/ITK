#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <qry/qry.h>
#include <fclasses/tc_string.h>
#include <tccore/aom_prop.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUserName = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cQry_name = ITK_ask_cli_argument("-qry_name=");
	tag_t tQuery = NULLTAG;
	int iN_entries = NULL;
	char** cEntries = NULL;
	char** cValues = NULL;
	tag_t* tResult = NULL;
	char** cValue = NULL;
	int iNum_value = 0;
	int iNoOfObjectsFound = 0;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(cUserName, NULL) == 1||tc_strcmp(cUserName,"")==1||tc_strcmp(cPassword,NULL)==1||tc_strcmp(cPassword,"")==1||
		tc_strcmp(cQry_name, NULL) == 1 || tc_strcmp(cQry_name, "") == 1)
	{
		iFail = ITK_init_module(cUserName, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = QRY_find2(cQry_name, &tQuery);
			if (iFail == ITK_ok && tQuery != NULLTAG)
			{
				cout << "Query found \n";
				iFail = QRY_find_user_entries(tQuery, &iN_entries, &cEntries, &cValues);
				if (iFail == ITK_ok &&  cValues != NULL)
				{
					cout << "user cEntries found " << endl;
					iFail = QRY_execute(tQuery, iN_entries, cEntries, cValues, &iNoOfObjectsFound, &tResult);
					
					cout << "executing query" << endl;
					for (int i = 0; i < iNoOfObjectsFound ; i++)
					{
						AOM_ask_displayable_values(tResult[i], "user_name", &iNum_value, &cValue);
						for (int i = 0; i < iNum_value; i++) {
							cout << "user name : " << cValue[i] << endl;
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
		cout << "Usage: " << argv[0] << " -u= <cUserName> -p= <cPassword> -g= <cGroup> -cQry_name= <query name>" << endl;
	}
	
	return iFail;
}