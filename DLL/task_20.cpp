//20.Create a Custom Action Handler which firstly sees if in a BOM, all the child components of an Item Revision are released, if not then it should release all

#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <tccore/item.h>
#include <pom/pom/pom.h>
#include <bom/bom.h>
#include <tccore/aom_prop.h>
#include <tccore/releasestatus.h>
#include <tccore/releasestatus.h>
#include <tccore/aom_prop.h>
#include<tccore/aom.h>

using namespace std;
void printError(int error) {
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << endl;
}
void checkReleaseStatus(tag_t tRevLatest)
{
	int ifail = 0;
	int iRelCount = 0;
	char* cReleaseValues = NULL;
	tag_t tRelease_status = NULLTAG;
	ifail = AOM_UIF_ask_value(tRevLatest, "release_status_list", &cReleaseValues);
	if (ifail == ITK_ok && cReleaseValues == NULL)
	{
		/*for (int i = 0; i <= iRelCount; i++)
		{*/
			/*if (iRelCount != 0)
			{
				continue;
			}*/
			/*else
			{*/
				ifail = RELSTAT_create_release_status("custom_neeha", &tRelease_status);
				ifail = RELSTAT_add_release_status(tRelease_status, 1,  &tRevLatest, true);
				AOM_save_without_extensions(tRevLatest);
			/*}*/
		//}
		
	}
	else
	{
		printError(ifail);
	}
	MEM_free(cReleaseValues);
}

void PrintBOM(tag_t top_line_tag, int level)
{
	
	tag_t tItem = NULLTAG;
	tag_t tLatestRev = NULLTAG;
	char* cItemName = NULL;
	char* cRelease = NULL;
	char* cRevId = NULL;
	char* cError = NULL;
	int iChildCount = 0;
	int ifail = 0;
	int iCount = 0;
	tag_t* tChildren = NULL;
	//AOM_ask_value_string(top_line_tag, "bl_indented_title", &cItemName);
//	cout << "Level " << level << ": " << cItemName << endl;
	



	ifail = BOM_line_ask_all_child_lines(top_line_tag, &iCount, &tChildren);
	if (ifail == ITK_ok)
	{
		for (int i = 0; i < iCount; i++)
		{
			ifail = AOM_ask_value_string(tChildren[i], "bl_item_item_id", &cRelease);
			ifail = AOM_ask_value_string(tChildren[i], "bl_rev_item_revision_id", &cRevId);
			if (cRelease != NULL && cRevId != NULL)
			{
				/*ifail = ITEM_find_item(cRelease, &tItem);
				ifail = ITEM_ask_latest_rev(tItem, &tLatestRev);*/
				ifail = ITEM_find_rev(cRelease, cRevId, &tLatestRev);
				checkReleaseStatus(tLatestRev);
			}
			else
			{
				continue;
			}
			PrintBOM(tChildren[i], level + 1);
		}
	}
	else
	{
		printError(ifail);
	}
	MEM_free(tChildren);
	MEM_free(cRelease);
	MEM_free(cRevId);

}

extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
#define PLM_error (EMH_USER_error_base +2)





	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int handler_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int ADD_custom_status_handler(EPM_action_message_t msg);





	extern DLLAPI int ConsoleApplication2_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks2);
		return status;
	}
	extern DLLAPI int handler_execute_callbacks1(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "******* Welcome to dll registartion proess ***** \n";
		cout << "-----------Handler registered-------- \n";
		status = EPM_register_action_handler("DLL_Action_Handler_To_ReleaseChildComponents", "Action Handler For Creating Custom Status Through DLL", (EPM_action_handler_t)ADD_custom_status_handler);
		cout << "statuts" << status << endl;
		return status;
	}
	extern DLLAPI int ADD_custom_status_handler(EPM_action_message_t msg)
	{
		int ifail = 0;
		int iCount = 0;
		int iBomViewCount = 0;
		tag_t tClass_id = NULLTAG;
		tag_t* tAttachments = NULL;
		tag_t* bvrs = NULL;
		char* cClass_name = NULL;
		tag_t tRoot_task = NULLTAG;
		tag_t tWindow = NULLTAG;
		tag_t top_bom_line = NULLTAG;
		ifail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (ifail == ITK_ok && tRoot_task != NULLTAG)
		{
			cout << "Root task added " << endl;
			ifail = EPM_ask_attachments(tRoot_task, EPM_target_attachment, &iCount, &tAttachments);
			if (ifail == ITK_ok && tAttachments != NULL)
			{
				cout << "attachments found " << endl;
				for (int i = 0; i < iCount; i++)
				{
					ifail = POM_class_of_instance(tAttachments[i], &tClass_id);
					if (ifail == ITK_ok && tClass_id != NULLTAG)
					{
						cout << "Class ID found " << endl;
						ifail = POM_name_of_class(tClass_id, &cClass_name);
						if (tc_strcmp(cClass_name, "ItemRevision") == 0)
						{
							cout << "Item Revision Found" << endl;
							ifail = ITEM_rev_list_bom_view_revs(tAttachments[i], &iBomViewCount, &bvrs);
							if (ifail == ITK_ok && iBomViewCount != 0 && bvrs != NULL)
							{
								ifail = BOM_create_window(&tWindow);
								if (ifail == ITK_ok && tWindow != NULLTAG)
								{
									ifail = BOM_set_window_top_line(tWindow, NULLTAG, tAttachments[i], NULLTAG, &top_bom_line);
									if (ifail == ITK_ok && top_bom_line != NULL)
									{
										PrintBOM(top_bom_line, 0);
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
							printError(ifail);
						}
					}
					else
					{
						printError(ifail);
					}
				}
			}
			else
			{
				printError(ifail);
			}
		}
		MEM_free(tAttachments);
		MEM_free(cClass_name);
		MEM_free(bvrs);
		return ifail;

	}
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}