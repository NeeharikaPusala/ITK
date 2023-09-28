/*PreCondition task:
When save as Operation on Item, should check PDF dataset is attache dto IR or not, If attached check object_desc property on PDF. If length is less than 5 allow save as operation, 
else should show custom error.
*/

#include <iostream>
#include <fclasses/tc_string.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>

using namespace std;
void printError(int error)
{
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << endl;
}
extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +8)
#define PLM_error1 (EMH_USER_error_base +9)
#define PLM_error2 (EMH_USER_error_base +10)


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
		int iFail = 0;
		tag_t old_item = NULLTAG;
		tag_t old_rev = NULLTAG;
		old_item = va_arg(argv, tag_t);
		old_rev = va_arg(argv, tag_t);
		int iCountSec = 0;
		tag_t* tSec_objects = NULL;
		char* cObjct_type = NULL;
		char* cValuePDF = NULL;
		char* cItemName = NULL;
		bool flag =true;
		char* cIRName = NULL;

		iFail = GRM_list_secondary_objects_only(old_rev, NULLTAG, &iCountSec, &tSec_objects);
		if (iFail == ITK_ok && iCountSec != 0 && tSec_objects != NULL)
		{
			cout << iCountSec << endl;
			for (int i = 0; i < iCountSec; i++)
			{
				AOM_ask_value_string(tSec_objects[i], "object_type", &cObjct_type);
				if (tc_strcmp(cObjct_type, "PDF") == 0)
				{
					flag = false;
					cout << "object type is : " << cObjct_type << endl;
					AOM_ask_value_string(tSec_objects[i], "object_desc", &cValuePDF);
					if (tc_strcmp(cValuePDF,"") != 0)
					{
						if (tc_strlen(cValuePDF) <= 5)
						{
							cout << "length is less than 5";
						}
						else
						{
							AOM_ask_value_string(old_item, "object_string", &cItemName);
							EMH_store_error_s1(EMH_severity_error, PLM_error, cItemName);
							return PLM_error;
						}
					}
					else
					{
						EMH_store_error_s1(EMH_severity_error, PLM_error2, cItemName);
						return PLM_error2;
					}
				}
				else
				{
					continue;
				}
			}
			if (flag)
			{
				AOM_ask_value_string(old_rev, "object_string", &cIRName);
				EMH_store_error_s1(EMH_severity_error, PLM_error1, cIRName);
				return PLM_error1;
			}
		}
		else
		{
			printError(iFail);
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