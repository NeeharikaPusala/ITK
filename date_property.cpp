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

		iFail = ITEM_find_item("000585", &t_Item);
		if (iFail == ITK_ok && t_Item != NULLTAG)
		{
			AOM_refresh(t_Item, true);
			time_t currentTime;
			time(&currentTime);

			// Format the current time in "dd-MMM-yyyy hh:mm" format
			char timeStr[100];
			tm localTime;
			localtime_s(&localTime, &currentTime);
			strftime(timeStr, sizeof(timeStr), "%d-%b-%Y %I:%M", &localTime);

			// Parse the formatted time string into its components
			int day, month, year, hour, minute;
			char monthStr[4];  // Sufficient buffer size for the month string
			if (sscanf_s(timeStr, "%d-%3s-%d %d:%d", &day, monthStr, sizeof(monthStr), &year, &hour, &minute) == 5)
			{
				// Map the month abbreviation to its numeric value
				const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
				for (int i = 0; i < 12; i++)
				{
					if (strcmp(monthStr, months[i]) == 0)
					{
						month = i;
						break;
					}
				}

				// Create the date_t object
				date_t currentDate;
				currentDate.year = year;
				currentDate.month = month;
				currentDate.day = day;
				currentDate.hour = hour;
				currentDate.minute = minute;
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
			else
			{
				cout << "Failed to parse the formatted time string." << endl;
			}
		}

	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}
	return iFail;
}