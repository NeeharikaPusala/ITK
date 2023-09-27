/*PreCondition task: 
When save as Operation on Item, should check user_data1 property value on ItemMasterForm, If the value is length is greater than 5 should send custom error.*/

#include <iostream>
#include <fclasses/tc_string.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
using namespace std;
extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +5)
	int status = 0;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int CUSTOM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int CUSTOM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int PLM_add_pre_condition(METHOD_message_t msg, va_list argv);

	extern DLLAPI int ConsoleApplication2_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)CUSTOM_execute_callbacks2);
		status = METHOD_find_method("Item", "ITEM_create_from_rev", &method_id);
		status = METHOD_add_pre_condition(method_id, (METHOD_function_t)PLM_add_pre_condition, NULL);
		return status;
	}

	extern DLLAPI int CUSTOM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "---welcome to dll registration process---\n";
		cout << "---Login success---\n";
		return status;
	}
	extern DLLAPI int PLM_add_pre_condition(METHOD_message_t msg, va_list argv)
	{
		tag_t tItem = NULLTAG;
		tItem = va_arg(argv, tag_t);
		tag_t tNewRelation = NULLTAG;
		int iFail = 0;
		int iCount = 0;
		tag_t* tSec_obj = NULL;
		char* cValue = NULL;
		char* cItemName = NULL;
		char* cError = NULL;
		iFail = GRM_find_relation_type("IMAN_master_form", &tNewRelation);
		if (iFail == ITK_ok && tNewRelation != NULLTAG)
		{
			cout << "relation type found \n";
			iFail = GRM_list_secondary_objects_only(tItem, tNewRelation, &iCount, &tSec_obj);
			if (iFail == ITK_ok && tSec_obj != NULL && iCount > 0)
			{
				cout << "secondary objects count : " << iCount << "\n";
				for (int i = 0; i < iCount; i++)
				{
					AOM_ask_value_string(tSec_obj[i], "user_data_1", &cValue);
					if (cValue != NULL)
					{

						if (tc_strlen(cValue) <= 5)
						{
							cout << "length is 5 \n";
						}
						else
						{
							AOM_ask_value_string(tItem, "object_name", &cItemName);
							EMH_store_error_s1(EMH_severity_error, PLM_error, cItemName);
							return PLM_error;
						}
					}
					MEM_free(cValue);
				}
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
			MEM_free(tSec_obj);
		}
			
	
		return iFail;
	}

	extern DLLAPI int CUSTOM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "---Logout Success---\n";
		return status;
	}

}