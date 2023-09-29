//16.Rule Handler : If the particular IR has already WF initiated, it should through custom error else workflow should initiate.

#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <tc/emh.h>
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
#define PLM_error (EMH_USER_error_base +12)
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
		status = EPM_register_rule_handler("DLL_Check_WF_InProcess", "rule handler", (EPM_rule_handler_t)ADD_custom_condition);
		if (status == 0)
		{
			cout << "---------Handler registered-------- \n";
		}

		return status;
	}
	extern DLLAPI int ADD_custom_condition(EPM_rule_message_t msg)
	{
		tag_t troot_task = NULLTAG;
		tag_t *tattachments = NULL;
		tag_t tclass_id = NULLTAG;

		int *attachment_type = 0;
		int count = 0;
		int ifail = 0;

		char *class_name = NULL;
		char *dvalue = NULL;
		char *rvalue = NULL;
		char *obj_name = NULL;

		EPM_decision_t decision = EPM_nogo;

		ifail = EPM_ask_root_task(msg.task, &troot_task);
		ifail = EPM_ask_name2(troot_task, &rvalue);
		cout << "rvalue : " << rvalue;
		if (ifail == ITK_ok && troot_task != NULLTAG)
		{
			ifail = EPM_ask_all_attachments(troot_task, &count, &tattachments, &attachment_type);
			if (ifail == ITK_ok && count != 0)
			{
				for (int i = 0; i < count; i++)
				{
					POM_class_of_instance(tattachments[i], &tclass_id);
					POM_name_of_class(tclass_id, &class_name);
					if (tc_strcmp(class_name, "ItemRevision") == 0)
					{
						AOM_UIF_ask_value(tattachments[i], "process_stage_list", &dvalue);
						if (tc_strcmp(dvalue, rvalue) != 0)
						{
							AOM_ask_value_string(tattachments[i], "object_name", &obj_name);
							EMH_store_error_s1(EMH_severity_error, PLM_error, obj_name);
							return decision;
						}
						else
						{
							return EPM_go;
						}

					}
				}
			}
			else
			{
				PrintErrorMessage(ifail);
			}
		}
		else
		{
			PrintErrorMessage(ifail);
		}
		MEM_free(attachment_type);
		MEM_free(tattachments);
		return ifail;
	}

	
	
extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
{
	*decisison = ALL_CUSTOMIZATIONS;
	cout << "-------Logout success--------- \n";
	return status;
}
}
//cout << "iAttachment_types[0]: " << iAttachment_types[0] << endl;
						/*cout << "iAttachment_types[1]: " << iAttachment_types[1] << endl;
						cout << "iAttachment_types[2]: " << iAttachment_types[2] << endl;*/
						//AOM_ask_value_string(tAttachments[0], "object_type", &cType1);
						//cout << "type1 : " << cType1 << "\n";
						/*AOM_ask_value_string(tAttachments[1], "object_type", &cType2);
						cout << "type2 : " << cType2 << "\n";
						AOM_ask_value_string(tAttachments[2], "object_type", &cType3);
						cout << "type3 : " << cType3 << "\n";*/