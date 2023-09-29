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
		status = EPM_register_rule_handler("DLL_Rule_WF", "rule handler", (EPM_rule_handler_t)ADD_custom_condition);
		if (status == 0)
		{
			cout << "---------Handler registered-------- \n";
		}

		return status;
	}
	extern DLLAPI int ADD_custom_condition(EPM_rule_message_t msg)
	{
		int iFail = 0;
		int iCount = 0;
		tag_t* tAttachments = NULL;
		int* iAttachment_types = NULL;
		tag_t tRoot_task = NULLTAG;
		char** cProcessSL = NULL;
		char* cName = NULL;
		char* cType1 = NULL;
		char* cType2 = NULL;
		char* cType3 = NULL;
		int iNumCount = 0;
		EPM_decision_t decision = EPM_nogo;
		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (iFail == ITK_ok && tRoot_task != NULLTAG)
		{
			iFail = EPM_ask_all_attachments(tRoot_task, &iCount, &tAttachments, &iAttachment_types);
			if (iFail == ITK_ok && iCount > 0 && tAttachments != NULL)
			{
				cout << "iCount  : " << iCount << "\n";
				for (int i = 0; i < iCount; i++)
				{
					cout << "iAttachment_types[0]: " << iAttachment_types[0] << endl;
					/*cout << "iAttachment_types[1]: " << iAttachment_types[1] << endl;
					cout << "iAttachment_types[2]: " << iAttachment_types[2] << endl;*/
					AOM_ask_value_string(tAttachments[0], "object_type", &cType1);
					cout << "type1 : " << cType1 << "\n";
					/*AOM_ask_value_string(tAttachments[1], "object_type", &cType2);
					cout << "type2 : " << cType2 << "\n";
					AOM_ask_value_string(tAttachments[2], "object_type", &cType3);
					cout << "type3 : " << cType3 << "\n";*/
					AOM_ask_displayable_values(tAttachments[i], "process_stage_list", &iNumCount, &cProcessSL);
					for (int j = 0; j < iNumCount; j++)
					{
						if (tc_strcmp(cProcessSL[j], "") == 0)
						{
							return EPM_go;
						}
						else
						{
							AOM_ask_value_string(tAttachments[i], "object_string", &cName);
							EMH_store_error_s1(EMH_severity_error, PLM_error, cName);
							return decision;
						}
					}
					
				}
			}
		}
		return decision;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}