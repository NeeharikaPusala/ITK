//When a dataset is attached to a item revision with some specific relation then dataset name pattern should be in the form of  “ itemid - revid - n...only one customization..It iwll 
// generate the ids only won't check for secondary objects..
#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <pom/pom/pom.h>
#include <fclasses/tc_string.h>
#include <tccore/grm.h>
#include <ae/dataset.h>
#include <fstream>

using namespace std;


int datasetcounter = 0; 
void printError(int error)
{
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << "\n";
}

extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
	METHOD_id_t method_id;

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
		cout << "******* Welcome to dll registration process ***** \n";
		cout << "-----------Login Success-------- \n";
		status = METHOD_find_method("IMAN_manifestation", "GRM_create", &method_id);
		status = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)PLM_add_post_action, NULL);
		return status;
	}

	extern DLLAPI int PLM_add_post_action(METHOD_message_t *msg, va_list argv)
	{
		tag_t primary_object = NULLTAG;
		tag_t secondary_object = NULLTAG;
		tag_t relation_type = NULLTAG;
		tag_t user_data = NULLTAG;
		tag_t* new_relation = NULL;
		primary_object = va_arg(argv, tag_t);
		secondary_object = va_arg(argv, tag_t);
		relation_type = va_arg(argv, tag_t);
		user_data = va_arg(argv, tag_t);
		new_relation = va_arg(argv, tag_t*);

		int ifail = 0;

		tag_t tClass_id = NULLTAG;
		tag_t tNewDataset = NULLTAG;
		tag_t tNewRelation = NULLTAG;
		char* cName_class = NULL;
		char* cItem_id = NULL;
		char* cDatasetName = NULL;
		char* cRev_id = NULL;
		char dataset_name[256];
	
		ifail = AOM_ask_value_string(primary_object, "item_id", &cItem_id);
		cout << "cItem_id : " << cItem_id << endl;

		ifail = AOM_ask_value_string(primary_object, "item_revision_id", &cRev_id);
		cout << "item_rev_id : " << cRev_id << endl;

		ifail = POM_class_of_instance(secondary_object, &tClass_id);
		if (ifail == ITK_ok && tClass_id != NULLTAG)
		{
			ifail = POM_name_of_class(tClass_id, &cName_class);
			cout << "cName_class : " << cName_class << "\n";
			if (tc_strcmp(cName_class, "Dataset") == 0)
			{
				datasetcounter++;
				
				// Check if the dataset name already contains a counter
				AOM_ask_value_string(secondary_object, "object_name", &cDatasetName);

				cout << cName_class << "\n";
				snprintf(dataset_name, sizeof(dataset_name), "%s-%s-%d", cItem_id, cRev_id, datasetcounter);
				AOM_refresh(secondary_object, true);
				
				ifail = AOM_set_value_string(secondary_object, "object_name", dataset_name);
				AOM_save_without_extensions(secondary_object);
				AOM_refresh(secondary_object, false);
			}
			else
			{
				printError(ifail);
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
