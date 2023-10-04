/*19.Write a Custom Action Handler to get Item Revision from Target which is an Assembly and get the related BOMView Revision.
	Get Child lines from BOMView Revision by expanding with “Latest Working” Revision Rule and write below information into a .csv file.
	The location of .csv file to be created should be stored in a preference.
	Level| Child Item ID| Child Revision ID| Quantity| UOM
	*/

#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <tccore/item.h>
#include <pom/pom/pom.h>
#include <tc/preferences.h>
#include <bom/bom.h>
#include <tccore/aom_prop.h>
#include <cfm/cfm.h>
#include <fstream>
using namespace std;
void printError(int errorCode)
{
	char* errorText = NULL;
	EMH_ask_error_text(errorCode, &errorText);
	cout << errorText << endl;
}
void PrintBOMToFile(tag_t top_line_tag, int level, ofstream& myFile)
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
	int iValue = 0;


	AOM_ask_value_int(top_line_tag, "bl_level_starting_0", &iValue);
	AOM_ask_value_string(top_line_tag, "bl_item_item_id", &cItemName);
	AOM_ask_value_string(top_line_tag, "bl_rev_item_revision_id", &cValue);
	AOM_ask_value_string(top_line_tag, "bl_quantity", &cQuantity);
	AOM_ask_displayable_values(top_line_tag, "bl_uom", &iCount2, &cUOM);

	
	for (int i = 0; i < iCount2; i++)
	{
		
		myFile << iValue << "|" << cItemName << "|" << cValue << "|" << cQuantity << "|" << cUOM[i] << endl;
	}
	if (level == 0)
	{
		myFile << iValue << "|" << cItemName << "|" << cValue << "|" << cQuantity << "|" << endl;
	}

	
	iFail = BOM_line_ask_all_child_lines(top_line_tag, &iCount, &tChildren);
	if (iFail == ITK_ok)
	{
		for (int i = 0; i < iCount; i++)
		{
			PrintBOMToFile(tChildren[i], level + 1, myFile);
		}
	}
	else
	{
		printError(iFail);
	}
}
void checkBOM(tag_t tRev)
{
	tag_t tWindow = NULLTAG;
	tag_t tTop_bom_line = NULLTAG;
	tag_t tConfig_rule = NULLTAG;
	tag_t tRule_name = NULLTAG;
	int ifail = 0;
	const char* pref_name = "ActionHandler_to_pirnt_bom_properties";
	const char* cRule_name = "Latest Working";
	char* cPrefValue = NULL;
	char* cConfigName = NULL;
	ifail = BOM_create_window(&tWindow);
	if (ifail == ITK_ok && tWindow != NULL)
	{
		ifail = BOM_set_window_top_line(tWindow, NULLTAG, tRev, NULLTAG, &tTop_bom_line);
		if (ifail == ITK_ok && tTop_bom_line != NULLTAG)
		{
			ifail = PREF_ask_char_value(pref_name, 0, &cPrefValue);
			if (ifail == ITK_ok && cPrefValue != NULL)
			{
				ofstream myFile(cPrefValue);
				if (!myFile.is_open())
				{
					cout << "Error: Unable to open CSV file for writing." << endl;
				}
				else
				{
					ifail = BOM_ask_window_config_rule(tWindow, &tConfig_rule);
					ifail = AOM_ask_value_string(tConfig_rule, "object_name", &cConfigName);
					if (tc_strcmp(cConfigName, "Latest Working") == 0)
					{
						cout << "Revision rule is latest working only " << "\n";
						myFile << "Level | Child Item ID | Child Revision ID | Quantity | UOM " << "\n";
						PrintBOMToFile(tTop_bom_line, 0, myFile);
					}
					else
					{
						ifail = CFM_find(cRule_name, &tRule_name);
						ifail = BOM_set_window_config_rule(tWindow, tRule_name);
						BOM_save_window(tWindow);
						myFile << "Level | Child Item ID | Child Revision ID | Quantity | UOM " << "\n";
						PrintBOMToFile(tTop_bom_line, 0, myFile);
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
	MEM_free(cPrefValue);
	MEM_free(cConfigName);
}

extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +13)
	int status = 0;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int ADD_Action_Handler(EPM_action_message_t msg);


	extern DLLAPI int ConsoleApplication2_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return status;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "******* Welcome to dll registartion proess ***** \n";
		status = EPM_register_action_handler("DLL_Action_BOM_Print_Child_Prop", "action", (EPM_action_handler_t)ADD_Action_Handler);
		if (status == 0)
		{
			cout << " **** Hanlder Registered ****" << endl;
		}
		return status;
	}
	extern DLLAPI int ADD_Action_Handler(EPM_action_message_t msg)
	{
		tag_t tRoot_task = NULLTAG;
		tag_t tClass_id = NULLTAG;
		tag_t rev = NULLTAG;
		char* cName = NULL;
		char* cValName = NULL;
		int ifail = 0;
		int iCount = 0;
		int countRev = 0; 
		tag_t*  bvrs = NULL;
		tag_t* tAttachment = NULL;
		
		ifail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (ifail == ITK_ok && tRoot_task != NULL)
		{
			ifail = EPM_ask_attachments(tRoot_task, EPM_target_attachment, &iCount, &tAttachment);
			if (ifail == ITK_ok && iCount > 0 && tAttachment != NULL)
			{
				for (int i = 0; i < iCount; i++)
				{
					POM_class_of_instance(tAttachment[i], &tClass_id);
					POM_name_of_class(tClass_id, &cName);
					if (tc_strcmp(cName, "ItemRevision") == 0)
					{
						ifail = ITEM_rev_list_bom_view_revs(tAttachment[i], &countRev, &bvrs);
						if (ifail == ITK_ok && countRev != 0 && bvrs != NULL)
						{
							checkBOM(tAttachment[i]);
						}
						else
						{
							AOM_ask_value_string(tAttachment[i], "object_string", &cValName);
							EMH_store_error_s1(EMH_severity_error, PLM_error, cValName);
							return PLM_error;
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
		return ifail;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}