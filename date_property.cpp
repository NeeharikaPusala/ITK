#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <ctime>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <fclasses/tc_string.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	char* cError = NULL;
	tag_t t_Item = NULLTAG;

	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);

		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}

		iFail = ITEM_find_item("000586", &t_Item);
		if (iFail == ITK_ok && t_Item != NULLTAG)
		{
			AOM_refresh(t_Item, true);
			time_t currentTime;
			time(&currentTime);

			// Format the current time in "dd-MMM-yyyy hh:mm" format
			char timeStr[100];
			tm localTime;
			localtime_s(&localTime, &currentTime);
			//cout << "localTime : " << localTime << endl;
			cout << " currentTime : " << currentTime << endl;
			strftime(timeStr, sizeof(timeStr), "%d-%b-%Y %H:%M", &localTime);
			cout <<"timeStr : " << timeStr << endl;

			// Create the date_t object
			date_t currentDate;
			currentDate.year = localTime.tm_year + 1900;
			currentDate.month = localTime.tm_mon;
			currentDate.day = localTime.tm_mday;
			currentDate.hour = localTime.tm_hour;
			currentDate.minute = localTime.tm_min;
			currentDate.second = 0;

			// Set the date property with the date_t object
			iFail = AOM_set_value_date(t_Item, "a2date_property", currentDate);
			AOM_save_without_extensions(t_Item);
			AOM_refresh(t_Item, false);

			if (iFail == ITK_ok)
			{
				cout << "Updated date_property successfully." << endl;
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}
	return iFail;
}
