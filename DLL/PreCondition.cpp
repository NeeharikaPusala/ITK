#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tc/emh.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>

using namespace std;

extern "C"
{
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +2)

	METHOD_id_t tMethod_id ;

	int status = 0;

	extern DLLAPI int ConsoleApplication1_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_add_preCondition(METHOD_message_t *msg, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);


	extern DLLAPI int ConsoleApplication1_register_callbacks()
	{
		status = CUSTOM_register_exit("ConsoleApplication1", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication1", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return status;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "******* Welcome to dll registartion proess ***** \n";
		cout << "-----------Login Success-------- \n";
		status = METHOD_find_method("ItemRevision", "ITEM_copy_rev", &tMethod_id);
		status = METHOD_add_pre_condition(tMethod_id, (METHOD_function_t)PLM_add_preCondition, NULL);
		return status;
	}
	extern DLLAPI int PLM_add_preCondition(METHOD_message_t *msg, va_list argv)
	{
		tag_t tItemRev = NULLTAG;
		char* cValue = NULL;
		char* cName = NULL;
		tItemRev = va_arg(argv, tag_t);
		AOM_ask_value_string(tItemRev, "object_name", &cName);
		cout << cName << endl;
		AOM_UIF_ask_value(tItemRev, "release_status_list", &cValue);
		cout << cValue << endl;
		if (tc_strcmp(cValue,"TCM Released") == 0)
		{
			cout << "Object is having TCM Release sttaus : \n";
		}
		else
		{
			
			EMH_store_error_s1(EMH_severity_error, PLM_error, cName);
			return PLM_error;
		}
		return status;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}