#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tc/preferences.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include<fclasses/tc_string.h>
#include <sstream>
#include <map>
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
#define PLM_error (EMH_USER_error_base +15)
	int status = 0;
	METHOD_id_t method_id;
	map<string, string> mapping;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv);

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

		tag_t* new_item = NULL;
		tag_t* new_rev = NULL;
		tag_t item_master_form = NULLTAG;
		tag_t item_rev_master_form = NULLTAG;

		item_id = va_arg(argv, char*);
		item_name = va_arg(argv, char*);
		type_name = va_arg(argv, char*);
		rev_id = va_arg(argv, char*);
		new_item = va_arg(argv, tag_t*);
		new_rev = va_arg(argv, tag_t*);
		item_master_form = va_arg(argv, tag_t);
		item_rev_master_form = va_arg(argv, tag_t);
		
		const char* pref_name = "Update_from_pref_automatically";
		int ifail = 0;
		int iCount = 0;
		char** cValues = NULL;
		char* cValuePref = NULL;
		char* dll1 = NULL;
		char* dll2 = NULL;
		char* cObj_string = NULL;

		

		ifail = PREF_ask_char_values(pref_name, &iCount, &cValues);
		ifail = AOM_ask_value_string(new_rev[0], "a2dll_1", &dll1);
		ifail = AOM_ask_value_string(new_rev[0], "a2dll_2", &dll2);
		if (ifail == ITK_ok && iCount > 0 && cValues != NULL)
		{
			for (int i = 0; i < iCount; i++)
			{
				string prefValue = cValues[i];
				stringstream ss(prefValue);
				string keyValue;
				while (getline(ss, keyValue, ':'))
				{
					string key = keyValue;
					cout << "key : " << key << endl;
					string value;
					getline(ss, value);
					cout << "value : " << value << endl;
					
					cout << "dll1 : " << dll1 << "\n";
					cout << "dll2 : " << dll2 << "\n";
					if (tc_strcmp(dll1,"")!=0 ||tc_strcmp(dll2, "")!=0)
					{
						if (strcmp(dll1, "A") == 0 && strcmp(key.c_str(), "A") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_2", value.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);
						}

						else if (strcmp(dll1, "B") == 0 && strcmp(key.c_str(), "B") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_2", value.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);
						}
						else if (strcmp(dll1, "C") == 0 && strcmp(key.c_str(), "C") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_2", value.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);
						}
						else if (strcmp(dll2, "1") == 0 && strcmp(value.c_str(), "1") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_1", key.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);
						}
						else if (strcmp(dll2, "2") == 0 && strcmp(value.c_str(), "2") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_1", key.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);

						}
						else if (strcmp(dll2, "3") == 0 && strcmp(value.c_str(), "3") == 0)
						{
							AOM_refresh(new_rev[0], true);
							AOM_set_value_string(new_rev[0], "a2dll_1", key.c_str());
							AOM_save_without_extensions(new_rev[0]);
							AOM_refresh(new_rev[0], false);
						}
						else
						{
							continue;
						}

					}
					else
					{
						AOM_ask_value_string(new_rev[0], "object_name", &cObj_string);
						EMH_store_error_s1(EMH_severity_error, PLM_error, cObj_string);
						return PLM_error;
					}
					
				}
			}
		}
		else
		{
			printError(ifail);
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