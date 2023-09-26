#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <pom/pom/pom.h>
#include<tc/tc_arguments.h>
#include <tccore/releasestatus.h>
using namespace std;

extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
#define PLM_error (EMH_USER_error_base +2)

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int handler_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int ADD_custom_status_handler_with_arg(EPM_action_message_t msg);
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
		status = EPM_register_action_handler("DLL_Action_Arg", "Arg", (EPM_action_handler_t)ADD_custom_status_handler_with_arg);
		cout << "status" << status << endl;
		return status;
	}
	extern DLLAPI int ADD_custom_status_handler_with_arg(EPM_action_message_t msg)
	{
		int iFail = 0;
		int iCount = 0;
		int no_of_arguments = 0;
		tag_t tClass_id = NULLTAG;
		tag_t* tAttachments = NULL;
		char* cClass_name = NULL;
		tag_t tRoot_task = NULLTAG;
		char* cArgument_name = NULL;
		char* cArgument_value = NULL;
		logical retain_released_date = true;
		string value;
		tag_t tRelease_status = NULLTAG;
		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		cout << iFail << "iFail value" << endl;
		if (iFail == ITK_ok && tRoot_task != NULLTAG)
		{
			cout << "Root task added " << endl;
			iFail = EPM_ask_attachments(tRoot_task, EPM_target_attachment, &iCount, &tAttachments);
			if (iFail == ITK_ok && tAttachments != NULL)
			{
				cout << "attachments found " << endl;
				if (iCount > 0)
				{
					no_of_arguments = TC_number_of_arguments(msg.arguments);
					for (int j = 0; j < no_of_arguments; j++)
					{
						ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgument_name, &cArgument_value);
						if (tc_strcmp(cArgument_name, "apply_status") == 0)
						{
							value  = cArgument_value;
						}
					}
					for (int i = 0; i < iCount; i++)
					{
						if (tAttachments[i] != NULL)
						{
							iFail = POM_class_of_instance(tAttachments[i], &tClass_id);
							if (iFail == ITK_ok && tClass_id != NULLTAG)
							{
								cout << "Class ID found " << endl;
								iFail = POM_name_of_class(tClass_id, &cClass_name);
								if (tc_strcmp(cClass_name, "ItemRevision") == 0)
								{
									cout << "Item Revision Found" << endl;
									iFail = RELSTAT_create_release_status(value.c_str(), &tRelease_status);
									if (iFail == ITK_ok && tRelease_status != NULLTAG)
									{
										cout << "status created : " << endl;
										iFail = RELSTAT_add_release_status(tRelease_status, 1, &tAttachments[i], retain_released_date);
										if (iFail == ITK_ok)
										{
											cout << "Custom status created " << endl;
										}
										else
										{
											EMH_store_error_s1(EMH_severity_error, PLM_error, cClass_name);
											return PLM_error;
										}
									}
								}
							}
						}
						
					}
					MEM_free(cArgument_name);
					MEM_free(cArgument_value);
					MEM_free(cClass_name);
					MEM_free(tAttachments);
				
				
				}

			}
		}
		return iFail;
	}
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}