#include <iostream>
using namespace std;
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <fclasses/tc_string.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>

extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication1_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);
	

	extern DLLAPI int ConsoleApplication1_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication1", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication1", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return status;
	}

	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "***** Welcome to DLL Registartion Process *****" << endl;
		cout << "----- Login Success -----";
		status = METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &method_id);
		status = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
		return status;
	}
	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv)
	{
		
		tag_t  old_item = NULLTAG;
		old_item = va_arg(argv, tag_t);
		

		int iFail = 0;
		tag_t tRelationType = NULLTAG;
		int iCount = 0;
		tag_t* tSecondaryObj = NULL;
		iFail = GRM_find_relation_type("IMAN_master_form", &tRelationType);
		if (iFail == ITK_ok && tRelationType != NULLTAG)
		{
			cout << "relation found : \n";
			iFail = GRM_list_secondary_objects_only(old_item, tRelationType, &iCount, &tSecondaryObj);
			if (iFail == ITK_ok && iCount > 0 && tSecondaryObj != NULL)
			{
				for (int i = 0; i < iCount; i++)
				{
					AOM_refresh(tSecondaryObj[i], true);
					iFail = AOM_set_value_string(tSecondaryObj[i], "user_data_1", "Sep");
					
					AOM_save_with_extensions(tSecondaryObj[i]);
					AOM_refresh(tSecondaryObj[i], false);
					cout << "Property updated: \n";
				}

			}
		}
		else
		{
			cout << "Relation not found \n ";
		}

		return status;
		
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "*****Logout Success*****";
		return status;
	}
}