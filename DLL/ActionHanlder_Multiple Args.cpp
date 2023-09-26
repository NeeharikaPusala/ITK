#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <ae/ae.h>
#include <tccore/aom.h>
#include <pom/pom/pom.h>
#include <tccore/grm.h>
#include<tc/tc_arguments.h>
#include <tccore/releasestatus.h>
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
	int status = 0;
	char* cError = NULL;
#define PLM_error (EMH_USER_error_base +2)

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int handler_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int Action_handler_multiple_arg(EPM_action_message_t msg);
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
		status = EPM_register_action_handler("DLL_Multiple_Arg", "Arg", (EPM_action_handler_t)Action_handler_multiple_arg);
		if (status == 0)
		{
			cout << "----Handler Registerd----";
		}
		
		return status;
	}
	extern DLLAPI int Action_handler_multiple_arg(EPM_action_message_t msg)
	{
		int iFail = 0;
		char*cError = NULL;
		tag_t tRoot_task = NULLTAG;
		int iCount = 0;
		tag_t* tAttachment = NULL;
		int no_of_arg = 0;
		string dataset_name;
		string dataset_type;
		string relation;
		char* cArg_name = NULL;
		char* cArg_value = NULL;
		tag_t tClass_id = NULLTAG;
		char* cClass_name = NULL;
		tag_t tDataset_type = NULLTAG;
		tag_t tNewDataset = NULLTAG;
		tag_t tRelation_type = NULLTAG;
		tag_t tNew_relation = NULLTAG;

		iFail = EPM_ask_root_task(msg.task, &tRoot_task);
		if (iFail == ITK_ok && tRoot_task != NULLTAG)
		{
			cout << "Root task found : " << endl;
			no_of_arg = TC_number_of_arguments(msg.arguments);
			if (no_of_arg > 0)
			{
				for (int i = 0; i < no_of_arg; i++)
				{
					ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArg_name, &cArg_value);
					if (tc_strcmp(cArg_name, "dataset_name") == 0)
					{
						dataset_name = cArg_value;
					}
					if (tc_strcmp(cArg_name, "dataset_type") == 0)
					{
						dataset_type = cArg_value;
					}
					if (tc_strcmp(cArg_name, "relation") == 0)
					{
						relation = cArg_value;
					}
				}
			}
			else
			{
				PrintErrorMessage(iFail);
			}
			iFail = EPM_ask_attachments(tRoot_task, EPM_target_attachment, &iCount, &tAttachment);
			if (iFail == ITK_ok && iCount > 0 && tAttachment != NULL)
			{
				for (int i = 0; i < iCount; i++)
				{
					POM_class_of_instance(tAttachment[i], &tClass_id);
					POM_name_of_class(tClass_id, &cClass_name);
					if (tc_strcmp(cClass_name, "ItemRevision") == 0)
					{
						iFail = AE_find_datasettype2(dataset_type.c_str(), &tDataset_type);
						if (iFail == ITK_ok && tDataset_type != NULLTAG)
						{
							cout << "dataset type found : " << endl;
							iFail = AE_create_dataset_with_id(tDataset_type, dataset_name.c_str(), "wrkflw", "00", "A", &tNewDataset);
							if (iFail == ITK_ok && tNewDataset != NULLTAG)
							{
								AOM_save_without_extensions(tNewDataset);
								cout << "dataset created and saved successfully : " << endl;
								iFail = GRM_find_relation_type(relation.c_str(), &tRelation_type);
								if (iFail == ITK_ok && tRelation_type != NULLTAG)
								{
									cout << "relation type found : " << endl;
									iFail = GRM_create_relation(tAttachment[i], tNewDataset, tRelation_type, NULLTAG, &tNew_relation);
									if (iFail == ITK_ok && tNew_relation != NULLTAG)
									{
										cout << "relation created successfully " << endl;
										GRM_save_relation(tNew_relation);
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
		return iFail; 
	}
	extern DLLAPI int handler_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}