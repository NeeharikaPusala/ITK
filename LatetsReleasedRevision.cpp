#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/workspaceobject.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <vector>
#include <epm/epm.h>
#include <string.h>
#include <pom/pom/pom.h>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;


string dateToString(const date_t& date)
{
	char neeha[20];

	sprintf(neeha, "%02d-%02d-%04d %02d:%02d:%02d",
		date.day, date.month, date.year, date.hour, date.minute, date.second);

	return string(neeha);
}
bool compareDates(const date_t& date1, const date_t& date2) {
	int answer;
	POM_compare_dates(date1, date2, &answer);
	return answer == -1;  // Sort in ascending order
}
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	int iCount = 0;
	int iAnswer = 0;
	int is_released = 0;
	tag_t* tRevList = NULL;
	string dateString;
	string datestring1;
	char* cValues = NULL;
	char* cValueNew = NULL;
	vector <tag_t> vIRTags;
	vector <string> vDateString;
	vector <tag_t> vReleaseTags;
	date_t dDateValue = NULLDATE;
	vector <date_t> vDates;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << endl << " Login Success" << endl;
			if (cItem_id)
			{
				iFail = ITEM_find_item(cItem_id, &tItem);
				if (iFail == ITK_ok && tItem != NULLTAG)
				{
					iFail = ITEM_list_all_revs(tItem, &iCount, &tRevList);
					if (iFail == ITK_ok && iCount > 0 && tRevList != NULL)
					{
						cout << "list found : \n";
						for (int i = 0; i < iCount; i++)
						{
							vIRTags.push_back(tRevList[i]);
						}
						for (int i = 0; i < vIRTags.size(); i++)
						{
							iFail = AOM_ask_value_string(vIRTags[i], "object_string", &cValues);
							if (iFail == ITK_ok && cValues != NULL)
							{
								iFail = EPM_ask_if_released(vIRTags[i], &is_released);
								if (iFail == ITK_ok && is_released == 1)
								{
									iFail = AOM_ask_value_string(vIRTags[i], "object_string", &cValues);
									cout << cValues << endl;
									vReleaseTags.push_back(vIRTags[i]);
									iFail = AOM_ask_value_date(vReleaseTags[i], "date_released", &dDateValue);
									vDates.push_back(dDateValue);
									sort(vDates.begin(), vDates.end(), compareDates);
									date_t latestDate = vDates.back();
									dateString = dateToString(latestDate);
								}
								else
								{
									continue;

								}
							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						cout << vDates.size() << endl;
						cout << "Latest Date Released: " << dateString << endl;
						for (int i = 0; i < vReleaseTags.size(); i++)
						{
							iFail = AOM_ask_value_date(vReleaseTags[i], "date_released", &dDateValue);
							datestring1 = dateToString(dDateValue);
							if (tc_strcmp(datestring1.c_str(), dateString.c_str()) == 0)
							{
								cout << datestring1 << endl;
								AOM_ask_value_string(vReleaseTags[i], "item_revision_id", &cValueNew);
								if (iFail == ITK_ok && cValueNew != NULL)
								{
									cout << cValueNew << endl;
								}
								else
								{
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								continue;
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
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -item_id= <item_id> " << endl;
	}

	return iFail;
}