#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <epm/epm.h>
#include <pom/pom/pom.h>
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
		status = EPM_register_action_handler("DLL_CUSTOM_STATUS", "Action Handler For Creating Custom Status Through DLL", (EPM_action_handler_t)ADD_custom_status_handler);
		cout << "statuts" << status << endl;
		return status;
	}
	extern DLLAPI int ADD_custom_status_handler(EPM_action_message_t msg)
	{
		int iFail = 0;
		int iCount = 0;
		tag_t tClass_id = NULLTAG;
		tag_t* tAttachments = NULL;
		char* cClass_name = NULL;
		tag_t tRoot_task = NULLTAG;
		logical retain_released_date = true;
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
				for (int i = 0; i < iCount; i++)
				{
					iFail = POM_class_of_instance(tAttachments[i], &tClass_id);
					if (iFail == ITK_ok && tClass_id != NULLTAG)
					{
						cout << "Class ID found " << endl;
						iFail = POM_name_of_class(tClass_id, &cClass_name);
						if (tc_strcmp(cClass_name, "ItemRevision") == 0)
						{
							cout << "Item Revision Found" << endl;
							iFail = RELSTAT_create_release_status("DLL Custom Status", &tRelease_status);
							if (iFail == ITK_ok && tRelease_status != NULLTAG)
							{
								cout << "sttaus created : " << endl;
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