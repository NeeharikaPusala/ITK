//24.Create a Rule Handler to validate if object owning group and logged in group are same, then only
//it should allow to initiate the workflow on it otherwise throw a custom error message.
#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <tc/emh.h>
#include <pom/pom/pom.h>
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
#define PLM_error (EMH_USER_error_base +17)
	int status = 0;
	

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int ADD_custom_condition(EPM_rule_message_t msg);


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
		status = EPM_register_rule_handler("DLL_Rule_owning_logged_grp", "rule handler", (EPM_rule_handler_t)ADD_custom_condition);
		if (status == 0)
		{
			cout << "-----------Handler registered-------- \n";
		}

		return status;
	}
	extern DLLAPI int ADD_custom_condition(EPM_rule_message_t msg)
	{
		int iFail = 0;
		bool flag = false;
		int iCount, iCountSec = 0;
		tag_t* tAttachments = NULL;
		tag_t* tSecondary_obcts = NULL;
		tag_t tClass_id = NULLTAG;
		int* iAttachment_types = 0;
		char* cClass_name = NULL;
		char* cObject_type = NULL;
		char* cGroup_name = NULL;
		char* cOwning_group = NULL;
		char* cObj_name = NULL;
		tag_t tGroup = NULLTAG;
		tag_t tRoot_task = NULLTAG;
		
		EPM_decision_t eDecision = EPM_nogo;
		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		iFail = EPM_ask_all_attachments(tRoot_task, &iCount, &tAttachments, &iAttachment_types);
		if (iFail == ITK_ok && iCount > 0 && tAttachments != NULL)
		{
			cout << "found attachments \n";
			for (int i = 0; i < iCount; i++)
			{
				POM_class_of_instance(tAttachments[i], &tClass_id);
				POM_name_of_class(tClass_id, &cClass_name);
				if (tc_strcmp(cClass_name, "ItemRevision") == 0)
				{
					iFail = POM_ask_group(&cGroup_name, &tGroup);
					cout << "Login group name : " << cGroup_name << "\n";
					if (iFail == ITK_ok && cGroup_name != NULL && tGroup != NULLTAG)
					{
						iFail = AOM_UIF_ask_value(tAttachments[i], "owning_group", &cOwning_group);
						if (tc_strcmp(cGroup_name, cOwning_group) == 0)
						{
							return EPM_go;
						}
						else
						{
							AOM_ask_value_string(tAttachments[i], "object_name", &cObj_name);
							EMH_store_error_s1(EMH_severity_error, PLM_error, cObj_name);
							return EPM_nogo;
						}
					}
					else
					{
						PrintErrorMessage(iFail);
					}
				}
			}
		}
		else
		{
			PrintErrorMessage(iFail);
		}
		MEM_free(tAttachments);
		MEM_free(iAttachment_types);
		MEM_free(tSecondary_obcts);
		MEM_free(cObj_name);
		MEM_free(cObject_type);
		MEM_free(cClass_name);
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}