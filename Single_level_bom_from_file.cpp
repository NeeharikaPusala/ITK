#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <bom/bom.h>
using namespace std;

int ITK_user_main(int argc, char* argv[])
{
	char* username = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	tag_t tParentItem = NULLTAG;
	tag_t tITEM_TAG_REV = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tNewLine = NULLTAG;
	tag_t top_bom_line = NULLTAG;
	tag_t tView_type = NULLTAG;
	tag_t tBom_view = NULLTAG;
	tag_t tBvr = NULLTAG;
	char* cError = NULL;
	char* value = NULL;
	string line;
	vector<tag_t> vItemTags;

	//if (tc_strcmp(username, NULL) == 1 || tc_strcmp(username, "") == 1 || tc_strcmp(password, NULL) == 1 || tc_strcmp(password, "") == 1 || tc_strcmp(group, NULL) == 1 || tc_strcmp(group, "") == 1)
	//{
		iFail = ITK_init_module("infodba", "infodba", "dba");
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			ifstream myFile("C:\\Users\\T50391\\Downloads\\bom_item_id.txt");
			if (myFile.is_open())
			{
				while (getline(myFile, line))
				{
					iFail = ITEM_find_rev(line.c_str(), "A", &tITEM_TAG_REV);
					if (tITEM_TAG_REV != NULLTAG)
					{
						vItemTags.push_back(tITEM_TAG_REV);
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
						cout << "error6 " << endl;
					}
				}
				// Check if there are items in the vector
				if (!vItemTags.empty())
				{
					// Create the BOM window outside of the loop
					iFail = BOM_create_window(&tWindow);
					if (iFail != ITK_ok)
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
						cout << "error5 " << endl;
					}
					for (int i = 0; i < vItemTags.size(); i++)
					{
						iFail = PS_find_view_type("view", &tView_type);
						if(iFail == ITK_ok && tView_type != NULLTAG)
						{
							if (i == 0)
							{
								iFail = PS_create_bv_with_bvr(NULL, NULL, tView_type, "bvr", "new bvr", false, vItemTags[i], &tBom_view, &tBvr);
								if (iFail == ITK_ok && tBom_view != NULLTAG && tBvr != NULLTAG)
								{
									iFail = BOM_set_window_top_line(tWindow, NULLTAG, vItemTags[i], NULLTAG, &top_bom_line);
									if (iFail == ITK_ok && top_bom_line != NULLTAG)
									{
										AOM_ask_value_string(vItemTags[i], "item_id", &value);
										cout << "item id : " << value << endl;
									}
									else
									{
										EMH_ask_error_text(iFail, &cError);
										cout << cError << endl;
										cout << "error4 " << endl;
									}
								}
								else
								{
									cout << "Not bale to create bv and bvr" << endl;
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								AOM_ask_value_string(vItemTags[i], "item_id", &value);
								cout << "child item id : " << value << endl;
								iFail = BOM_line_add(top_bom_line, NULLTAG, vItemTags[i], NULLTAG, &tNewLine);
								if (iFail == ITK_ok && tNewLine != NULL && tBom_view != NULLTAG)
								{
									cout << "BOM line added successfully " << endl;
								}
								else
								{
									cout << "Not able to add BOM line : ";
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
						}
						else
						{
							cout << "not able to find view type" << endl;
						}
						
					}
					BOM_save_window(tWindow);
				}
				else
				{
					cout << "No valid items found in the file." << endl;
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
					cout << "error3 " << endl;
				}
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
				cout << "error2 " << endl;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
			cout << "error1 " << endl;
		}
		
	return iFail;
}
	//else
	//{
	//	cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>" << endl;
	//}
