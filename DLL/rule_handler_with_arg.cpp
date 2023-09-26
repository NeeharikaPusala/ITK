#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/workspaceobject.h>
using namespace std;
void PrintErrorMessage(int errorCode)
{
	char* errorText = NULL;
	EMH_ask_error_text(errorCode, &errorText);
	cout << errorText << endl;
}
extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +4)
	int status = 0;
	int iFail = 0;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int handler_rule_with_arg(EPM_rule_message_t msg);


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
		status = EPM_register_rule_handler("DLL_Rule_one_arg", "rule", (EPM_rule_handler_t)handler_rule_with_arg);
		if (status == 0)
		{
			cout << "Handler registered successfully \n";
		}
		return status;
	}
	extern DLLAPI int handler_rule_with_arg(EPM_rule_message_t msg)
	{
		tag_t tRoot_task = NULLTAG;
		int iCount = 0;
		bool flag = false;
		int iCountSec = 0;
		char* cObject_type = NULL;
		tag_t* tSecondary_objects = NULL;
		tag_t* tAttachments = NULL;
		int* iAttachment_type = 0;
		int no_of_arguments = 0;
		char* cDataset_type = NULL;
		char* cName = NULL;
		char* cArg_name = NULL;
		char* cArg_value = NULL;
		tag_t tClass_id = NULL;
		char* cClass_name = NULL;
		EPM_decision_t decision = EPM_nogo;
		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (iFail == ITK_ok && tRoot_task != NULLTAG)
		{
			iFail = EPM_ask_all_attachments(tRoot_task, &iCount, &tAttachments, &iAttachment_type);
			if (iFail == ITK_ok && tAttachments != NULL && iCount > 0)
			{
				if (iCount > 0)
				{
					no_of_arguments = TC_number_of_arguments(msg.arguments);
					for (int j = 0; j < no_of_arguments; j++)
					{
						ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArg_name, &cArg_value);
						if (tc_strcmp(cArg_name, "dataset_type") == 0)
						{
							cDataset_type = cArg_value;
						}
					}
				}
				for (int i = 0; i < iCount; i++)
				{
					POM_class_of_instance(tAttachments[i], &tClass_id);
					POM_name_of_class(tClass_id, &cClass_name);
					if (tc_strcmp(cClass_name, "ItemRevision") == 0)
					{
						AOM_ask_value_string(tAttachments[i], "object_name" , &cName);
						iFail = GRM_list_secondary_objects_only(tAttachments[i], NULLTAG, &iCountSec, &tSecondary_objects);
						for (int j = 0; j < iCountSec; j++)
						{
							WSOM_ask_object_type2(tSecondary_objects[j], &cObject_type);
							if (tc_strcmp(cObject_type, cDataset_type) == 0)
							{
								flag = true;
							}
							else
							{
								continue;
							}
						}
						if (flag)
						{
							return EPM_go;
						}
						else
						{
							EMH_store_error_s1(EMH_severity_error, PLM_error, cName);
							return decision;
						}
					}
					else
					{
						PrintErrorMessage(iFail);
					}
				}
					
			}
			else
			{
				PrintErrorMessage(iFail);
			}
		}
		else
		{
			PrintErrorMessage(iFail);
		}
		MEM_free(tAttachments);
		MEM_free(iAttachment_type);
		MEM_free(cClass_name);
		MEM_free(cObject_type);
		MEM_free(tSecondary_objects);
		MEM_free(cName);

		return decision;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}