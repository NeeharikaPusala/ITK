//14. Create rule handler to check any property on bom line, if property filled for all child's then u should initiate the WF otherwise it should show custom error.


#include <iostream>
#include <fclasses/tc_string.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <epm/epm.h>
#include <bom/bom.h>
#include <pom/pom/pom.h>
using namespace std;
extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +6)
#define PLM_error1 (EMH_USER_error_base +7)
	int status = 0;
	

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int ADD_RULE_check_bom(EPM_rule_message_t msg);

	extern DLLAPI int ConsoleApplication2_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)handler_execute_callbacks2);
		return status;
	}
	
	extern DLLAPI int handler_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "---welcome to dll registration process---\n";
		cout << "---Login success---\n";
		status = EPM_register_rule_handler("DLL_rule_bom_line", "bom line", (EPM_rule_handler_t)ADD_RULE_check_bom);
		if (status == 0)
		{
			cout << "Handler registered \n";
		}
		return status;
	}
	extern DLLAPI int ADD_RULE_check_bom(EPM_rule_message_t msg)
	{
		int iFail = 0;
		tag_t tRoot_task = NULLTAG;
		int iCount = 0;
		tag_t* tAttachments = NULLTAG;
		tag_t tClasS_id = NULLTAG;
		char* cClass_name = NULL;
		int* iAttachmentTypeCount = 0;
		tag_t tWindow = NULLTAG;
		int iChilCount = 0;
		tag_t* tChildren = NULL;
		char* cValue = NULL;
		int iCountB = 0;
		tag_t* tBVRS = NULL;
		char* cIRname = NULL;
		char* cNameChild = NULL;
		EPM_decision_t decisionEPM = EPM_nogo;
		tag_t top_bom_line = NULLTAG;
		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (iFail == ITK_ok && tRoot_task != NULLTAG)
		{
			iFail = EPM_ask_all_attachments(tRoot_task, &iCount, &tAttachments, &iAttachmentTypeCount);
			if (iFail == ITK_ok && iCount > 0 && tAttachments != NULL)
			{
				cout << "iCount : " << iCount << endl;
				for (int i = 0; i < iCount; i++)
				{
					iFail = ITEM_rev_list_all_bom_view_revs(tAttachments[i], &iCountB, &tBVRS);
					if (iCountB != 0)
					{
						cout << "iAttachmentTypeCount : " << iAttachmentTypeCount[i] << endl;
						POM_class_of_instance(tAttachments[i], &tClasS_id);
						POM_name_of_class(tClasS_id, &cClass_name);
						if (tc_strcmp(cClass_name, "ItemRevision") == 0)
						{
							iFail = BOM_create_window(&tWindow);
							if (iFail == ITK_ok && tWindow != NULLTAG)
							{
								iFail = BOM_set_window_top_line(tWindow, NULLTAG, tAttachments[i], NULLTAG, &top_bom_line);
								BOM_save_window(tWindow);
								if (iFail == ITK_ok && top_bom_line != NULLTAG)
								{
									cout << "top line added \n";
								}
							}
							iFail = BOM_line_ask_all_child_lines(top_bom_line, &iChilCount, &tChildren);
							if (iFail == ITK_ok && iChilCount > 0 && tChildren != NULL)
							{
								cout << "children count : " << iChilCount;
								for (int j = 0; j < iChilCount; i++)
								{
									AOM_ask_value_string(tChildren[j], "bl_quantity", &cValue);
									if (tc_strcmp(cValue, "") == 0)
									{
										cout << "here" << endl;
										//return EPM_nogo;
										AOM_ask_value_string(tChildren[j], "bl_indented_title", &cNameChild);
										EMH_store_error_s1(EMH_severity_error, PLM_error, cNameChild);
										//return decisionEPM;
										cout << "property value is null " << endl;
										return decisionEPM;
									}
									else
									{
										return EPM_go;
									}
								}

							}
						}
					}
					else
					{
						AOM_ask_value_string(tAttachments[i], "object_name", &cIRname);
						EMH_store_error_s1(EMH_severity_error, PLM_error1, cIRname);
						return EPM_nogo;
					}
				}
			}
		}
		MEM_free(tAttachments);
		MEM_free(iAttachmentTypeCount);
		MEM_free(cClass_name);
		MEM_free(tChildren);

		return decisionEPM;
	}

	extern DLLAPI int handler_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "---Logout Success---\n";
		return status;
	}

}