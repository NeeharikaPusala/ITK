//15.PreCondition task :
//Whenever you are  trying to attach a  only ItemRevision to the product structure, It should get attached  otherwise it should show custom error.
#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tccore/aom_prop.h>
#include <tccore/workspaceobject.h>
#include <fclasses/tc_string.h>
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
#define PLM_error (EMH_USER_error_base +11)
	int status = 0;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int ADD_pre_condition(METHOD_message_t msg, va_list argv);


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
		cout << "-----------Login Success-------- \n";
		status = METHOD_find_method("BOMLine", "BOMLine_add", &method_id);
		status = METHOD_add_pre_condition(method_id, (METHOD_function_t)ADD_pre_condition, NULL);
		return status;
	}
	extern DLLAPI int ADD_pre_condition(METHOD_message_t msg, va_list argv)
	{
		tag_t parent = NULLTAG;
		tag_t item = NULLTAG;
		tag_t item_revision = NULLTAG;
		tag_t bom_view = NULLTAG;
		char *occurrence_type = NULL;
		//--------------------------------------//
		parent = va_arg(argv, tag_t);
		item = va_arg(argv, tag_t);
		item_revision = va_arg(argv, tag_t);
		bom_view = va_arg(argv, tag_t);
		occurrence_type = va_arg(argv, char*);
		//-------------------------------------//
		
		int ifail = 0;
		char* cObject_type = NULL;
		char* cObject_type2 = NULL;
		AOM_ask_value_string(parent, "bl_item_object_type", &cObject_type);
		cout << "cObject_type : " << cObject_type << endl;
		if (tc_strcmp(cObject_type, "Item") == 0)
		{
			ifail = AOM_ask_value_string(item_revision, "object_type", &cObject_type2);
			if (ifail == ITK_ok && cObject_type2 != NULL)
			{
				
				if (tc_strcmp(cObject_type2, "ItemRevision") == 0)
				{
					cout << "cObject_type2 : " << cObject_type2 << endl;
				}
				else
				{
					EMH_store_error_s1(EMH_severity_error, PLM_error, cObject_type);
					return PLM_error;
				}
			}
			else
			{
				printError(ifail);
			}
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, PLM_error, cObject_type);
			return PLM_error;
		}
		
		return ifail;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}