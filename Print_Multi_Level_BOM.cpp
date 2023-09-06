#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <bom/bom.h>
#include <ps/ps.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
using namespace std;
void PrintBOM(tag_t top_line_tag, int level)
{
	char* cItemName = NULL;
	char* cError = NULL;
	int iChildCount = 0;
	int iFail = 0;
	int iCount = 0;
	tag_t* tChildren = NULL;
	AOM_ask_value_string(top_line_tag, "bl_indented_title", &cItemName);
	cout << "Level " << level << ": " << cItemName << endl;
	iFail = BOM_line_ask_all_child_lines(top_line_tag, &iCount, &tChildren);
	if (iFail == ITK_ok)
	{
		for (int i = 0; i < iCount; i++)
		{
			PrintBOM(tChildren[i], level + 1);
		}
	}
	else
	{
		EMH_ask_error_text(iFail, &cError);
		cout << cError << endl;
	}
}
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cSearch_id = ITK_ask_cli_argument("-item_id=");
	tag_t tItem_rev = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tView = NULLTAG;
	tag_t t_top_bom_line = NULLTAG;
	tag_t tBom_view = NULLTAG;
	tag_t tBVR = NULLTAG;
	int iCount = 0;
	tag_t* tChildren = NULL;
	char* cValue = NULL;
	int iFail = 0;
	char* cError = NULL;
	
	//if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	//{
		iFail = ITK_init_module("infodba", "infodba", "dba");
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_rev("1", "A", &tItem_rev);
			if (iFail == ITK_ok && tItem_rev != NULLTAG)
			{
				cout << "Item rev found successfully \n";
				iFail = BOM_create_window(&tWindow);
				if (iFail == ITK_ok && tWindow != NULLTAG)
				{
					cout << "BOM window created successfully \n";
					iFail = BOM_set_window_top_line(tWindow, NULLTAG, tItem_rev, NULLTAG, &t_top_bom_line);
					if (iFail == ITK_ok && t_top_bom_line != NULLTAG)
					{
						cout << "top line added \n";
						PrintBOM(t_top_bom_line, 0);
						/*iFail = BOM_line_ask_child_lines(t_top_bom_line, &iCount, &tChildren);
						cout << iCount << endl;
						if (iFail == ITK_ok && iCount !=0 && tChildren != NULL)
						{
							for (int i = 0; i < iCount; i++)
							{
								AOM_ask_value_string(tChildren[i], "bl_indented_title", &cValue);
								cout << cValue << endl;
							}
						}
						else
						{
							cout << "childs not found \n";
							EMH_ask_error_text(iFail, &cError);
							cout << cError << endl;
						} */
					}
					else
					{
						cout << "BOM window not created  \n";
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}
				}
				else
				{
					cout << "Item rev didn't found \n" << endl;
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
		return iFail;			
}
/*else
	{
	cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -item_id= <item_id>" << endl;
	}
	*/