#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <bom/bom.h>
#include <tc/preferences.h>
#include <tccore\item.h>
#include <fstream>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <cfm/cfm.h>
using namespace std;
void PrintBOMToFile(tag_t top_line_tag, int level, ofstream& csvFile)
{
	char* cItemName = NULL;
	char* cError = NULL;
	int iChildCount = 0;
	int iFail = 0;
	int iCount = 0;
	int iCount2 = 0;
	tag_t* tChildren = NULL;
	char* cValue = NULL;
	char* cQuantity = NULL;
	tag_t tUOM = NULLTAG;
	char** cUOM = NULL;
	char* cValues = NULL;
	

	// Get properties
	AOM_ask_value_string(top_line_tag, "bl_level_starting_0", &cValue);
	AOM_ask_value_string(top_line_tag, "bl_item_item_id", &cItemName);
	AOM_ask_value_string(top_line_tag, "bl_rev_item_revision_id", &cValue);
	AOM_ask_value_string(top_line_tag, "bl_quantity", &cQuantity);
	AOM_ask_displayable_values(top_line_tag, "bl_uom", &iCount2, &cUOM);

	// Print to CSV file
	for (int i = 0; i < iCount2; i++)
	{
		// Get the value of the typed reference property "bl_uom"

		csvFile << level << "|" << cItemName << "|" << cValue << "|" << cQuantity << "|" << cUOM[i] << endl;
	}
	if (level == 0)
	{
		csvFile << level << "|" << cItemName << "|" << cValue << "|" << cQuantity << "|" << endl;
	}
	
	// Recursively process child lines
	iFail = BOM_line_ask_all_child_lines(top_line_tag, &iCount, &tChildren);
	if (iFail == ITK_ok)
	{
		for (int i = 0; i < iCount; i++)
		{
			PrintBOMToFile(tChildren[i], level + 1, csvFile);
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
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	char* cPref_name = ITK_ask_cli_argument("-pref_name=");
	int iFail = 0;
	int iCount = 0;
	tag_t tRevision = NULLTAG;
	tag_t* tBVRS = NULL;
	tag_t tWindow = NULLTAG;
	tag_t t_top_bom_line = NULLTAG;
	tag_t tConfig_rule = NULLTAG;
	char* cNameConfig = NULL;
	tag_t tNewRule = NULLTAG;
	char* cValue = NULL;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_rev(cItem_id, "A", &tRevision);
			if (iFail == ITK_ok && tRevision != NULLTAG)
			{
				cout << "Item Revision Found : \n";
				iFail = ITEM_rev_list_all_bom_view_revs(tRevision, &iCount, &tBVRS);
				if (iFail == ITK_ok && iCount > 0 && tBVRS != NULL)
				{
					for (int i = 0; i < iCount; i++)
					{
						iFail = BOM_create_window(&tWindow);
						if (tWindow != NULLTAG)
						{
							cout << "bom window created \n";
							iFail = BOM_set_window_top_line(tWindow, NULLTAG, tRevision, NULLTAG, &t_top_bom_line);
							if (iFail == ITK_ok && t_top_bom_line != NULLTAG)
							{
								cout << "Top line added \n";
								iFail = PREF_ask_char_value(cPref_name, 0, &cValue);
								if (iFail == ITK_ok && cValue != NULL)
								{
									cout << "pref path read " << endl;
									ofstream csvFile(cValue);
									if (!csvFile.is_open())
									{
										cout << "Error: Unable to open CSV file for writing." << endl;
										return -1;
									}
									else
									{
										iFail = BOM_ask_window_config_rule(tWindow, &tConfig_rule);
										if (iFail == ITK_ok && tConfig_rule != NULLTAG)
										{
											cout << "Configuratuon rule found \n";
											iFail = AOM_ask_value_string(tConfig_rule, "object_name", &cNameConfig);
											if (iFail == ITK_ok && cNameConfig != NULL)
											{
												cout << "rule name : " << cNameConfig << endl;
												if (tc_strcmp(cNameConfig, "Latest Working") == 0)
												{
													cout << "The revision rule is Latest working only \n";
													csvFile << "Level|Child Item ID|Child Revision ID|Quantity|UOM" << endl;
													PrintBOMToFile(t_top_bom_line, 0, csvFile);
													csvFile.close();
												}
												else
												{
													iFail = CFM_find("Latest Working", &tNewRule);
													if (iFail == ITK_ok && tNewRule != NULLTAG)
													{
														cout << "rule with name \"Latest working\" found" << endl;
														iFail = CFM_updated(tNewRule);
														if (iFail == ITK_ok)
														{
															cout << "rule set \n";
															BOM_save_window(tWindow);
															csvFile << "Level|Child Item ID|Child Revision ID|Quantity|UOM" << endl;
															PrintBOMToFile(t_top_bom_line, 0, csvFile);
															csvFile.close();
														}
														else
														{
															cout << "rule not set \n";
														}
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
												cout << "not able read name \n";
												EMH_ask_error_text(iFail, &cError);
												cout << cError << endl;
											}
										}

										
									}
								}
								else
								{
									cout << "not able read file path \n";
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
						}
						else
						{
							cout << "Bom window not created : \n ";
						}
					}
				}
				else
				{
					cout << "No BOMView Revision if found for the given IR. \n";
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

		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup> -item_id= <cItem_id> -pref_name=<pref name>" << endl ;
	}

	return iFail;
}