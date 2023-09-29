#include <iostream>
using namespace std;
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <fclasses/tc_string.h>
#include <tccore/aom.h>
#include <tc/preferences.h>
#include <tccore/aom_prop.h>
void printError(int error)
{
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << endl;
}
extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);


	extern DLLAPI int ConsoleApplication2_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return status;
	}

	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "***** Welcome to DLL Registartion Process *****" << endl;
		cout << "----- Login Success -----";
		status = METHOD_find_method("Item", "ITEM_create", &method_id);
		status = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
		return status;
	}
	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv)
	{

		const char* item_id = NULL;
		const char* item_name = NULL;
		const char* type_name = NULL;
		const char* rev_id = NULL;
		tag_t*      new_item = NULL;
		tag_t*      new_rev = NULL;
		tag_t       item_master_form = NULLTAG;
		tag_t       item_rev_master_form = NULLTAG;
		item_id = va_arg(argv, char*);
		item_name = va_arg(argv, char*);
		type_name = va_arg(argv, char*);
		rev_id = va_arg(argv, char*);
		new_item = va_arg(argv, tag_t*);
		new_rev = va_arg(argv, tag_t*);
		item_master_form = va_arg(argv, tag_t);
		item_rev_master_form = va_arg(argv, tag_t);
		//-----------------------------------//

		int iFail = 0;
		tag_t tItem = NULLTAG;
		char* cItemString = NULL;
		int iCount = 0;
		char** cValues = NULL;

		cout << item_id << "\n";
		cout << item_name << "\n";
		cout << type_name << "\n";
		cout << rev_id << "\n";
		cout << new_item[0] << "\n";
		cout << new_item[1] << "\n";
		AOM_refresh(new_item[0], true);
		iFail = PREF_ask_char_values("17task", &iCount, &cValues);
		if (iFail == ITK_ok && cValues != NULL)
		{
			for (int i = 0; i < iCount; i++)
			{
				if (i == 0)
				{
					iFail = AOM_set_value_string(new_item[0], "object_desc", cValues[i]);
					if (iFail == ITK_ok)
					{
						cout << "property set " << "\n";
					}
					else
					{
						printError(iFail);
					}
					AOM_save_with_extensions(new_item[0]);
					
				}
				else if (i == 1)
				{
					AOM_set_value_string(new_item[0], "object_name", cValues[i]);
					AOM_save_with_extensions(new_item[0]);
				}
				else if (i == 2)
				{
					AOM_set_value_string(new_item[0], "item_id", cValues[i]);
					AOM_save_with_extensions(new_item[0]);
					AOM_refresh(new_item[1], false);
				}
				else
				{
					break;
				}
				cout << cValues[i] << "\n" ;
			}
		}
		else
		{
			printError(iFail);
		}
		
		return iFail;

	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "*****Logout Success*****";
		return status;
	}
}