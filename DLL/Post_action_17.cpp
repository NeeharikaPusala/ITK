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
#include <map>
#include <sstream>
#include <vector>

void printError(int error) {
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << endl;
}

extern "C" {
#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +14)
	int status = 0;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication2_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int* decision, va_list argv);
	extern DLLAPI int PLM_add_post_action(METHOD_message_t* msg, va_list argv);

	extern DLLAPI int ConsoleApplication2_register_callbacks() {
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		status = CUSTOM_register_exit("ConsoleApplication2", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return status;
	}

	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv) {
		*decision = ALL_CUSTOMIZATIONS;
		cout << "***** Welcome to DLL Registration Process *****" << endl;
		cout << "----- Login Success -----" << endl;
		status = METHOD_find_method("Item", "ITEM_create", &method_id);
		status = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
		return status;
	}

	extern DLLAPI int PLM_add_post_action(METHOD_message_t* msg, va_list argv) {
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

		int ifail = 0;
		const char* pref_name = "Update_from_preference_custom_prop";
		int iCount = 0;
		char** cValues = NULL;
		char* cName = NULL;

		// Get preference values
		ifail = PREF_ask_char_values(pref_name, &iCount, &cValues);
		AOM_refresh(new_rev[0], true);

		if (ifail == ITK_ok && iCount > 0 && cValues != NULL) {
			for (int i = 0; i < iCount; i++) {
				string prefValue = cValues[i];
				stringstream ss(prefValue);
				string keyValue;
				while (getline(ss, keyValue, ':')) {
					// Update custom properties based on the key and value
					string key = keyValue;
					string value;
					cout << "key : " << key << endl;
					getline(ss, value);
					cout << "value : " << value << endl;
					if (key == "B")
					{
						AOM_set_value_string(new_rev[0], "a2pref1", key.c_str());
						AOM_set_value_string(new_rev[0], "a2pref2", value.c_str());
					}
					else if (key == "A")
					{
						AOM_set_value_string(new_rev[0], "a2pref1", key.c_str());
						AOM_set_value_string(new_rev[0], "a2pref2", value.c_str());
					}
					else if (key == "C")
					{
						AOM_set_value_string(new_rev[0], "a2pref1", key.c_str());
						AOM_set_value_string(new_rev[0], "a2pref2", value.c_str());
					}
					else
					{
						AOM_ask_value_string(new_rev[0], "object_string", &cName);
						EMH_store_error_s1(EMH_severity_error, PLM_error, cName);
						return PLM_error;
					}
					
				}

			}
			
			AOM_save_with_extensions(new_rev[0]);
			AOM_refresh(new_rev[0], false);
		}
		else {
			printError(ifail);
		}

		return ifail;
	}

	extern DLLAPI int PLM_execute_callbacks2(int* decision, va_list argv) {
		*decision = ALL_CUSTOMIZATIONS;
		cout << "***** Logout Success *****" << endl;
		return status;
	}
}
