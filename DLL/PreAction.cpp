#include <iostream>
#include <tccore/method.h>
#include <tccore/custom.h>
#include <tccore/tc_msg.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <fclasses/tc_string.h>
#include <tc/emh.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <vector>
using namespace std;

extern "C"
{
#define DLLAPI _declspec(dllexport)
	int status = 0;
	char* cError = NULL;
	vector<tag_t> vRelTags;
	METHOD_id_t method_id;

	extern DLLAPI int ConsoleApplication1_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list argv);


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
		status = METHOD_find_method("ItemRevision", "IMAN_delete", &method_id);
		status = METHOD_add_action(method_id, METHOD_pre_action_type, (METHOD_function_t)PLM_add_pre_action, NULL);
		return status;
	}
	extern DLLAPI int PLM_add_pre_action(METHOD_message_t *msg, va_list argv)
	{
		tag_t tRev = NULLTAG;
		tRev = va_arg(argv, tag_t);
		int iFail = 0;
		tag_t tItem = NULLTAG;
		tag_t* tRev_List = NULL;
		int iCountSec = 0;
		tag_t* tSecond_objcts = NULL;
		char* class_name = NULL;
		char* cName = NULL;
		tag_t tClass_id = NULLTAG;
		tag_t tRel_new = NULLTAG;
		int iCountRel = 0;
		tag_t* tRela_List = NULL;
		vector<tag_t> vDatasetTags;
		vector <tag_t> vRelTags;
		vector <tag_t> vRelTypeTags;
		tag_t tPrevRev = NULLTAG;
		int iCount = 0;
		iFail = ITEM_ask_item_of_rev(tRev, &tItem);
		iFail = ITEM_list_all_revs(tItem, &iCount, &tRev_List);
		for (int i = 0; i < iCount; i++)
		{
			if (tRev == tRev_List[i])
			{
				iFail = GRM_list_secondary_objects_only(tRev_List[i], NULLTAG, &iCountSec, &tSecond_objcts);
				cout << iCountSec << endl;
				for (int j = 0; j < iCountSec; j++)
				{
					iFail = POM_class_of_instance(tSecond_objcts[j], &tClass_id);
					iFail = POM_name_of_class(tClass_id, &class_name);
					cout << class_name << endl;
					if (tc_strcmp(class_name, "Dataset") == 0)
					{
						vDatasetTags.push_back(tSecond_objcts[j]);
					}
					
				}
			}
			else
			{
				// Track the previous revision
				tPrevRev = tRev_List[i];
			}
			
		}
		for (int k = 0; k < vDatasetTags.size(); k++)
		{
			// Get the relation type from the first dataset
			iFail = GRM_list_relations(tPrevRev, vDatasetTags[k], NULLTAG, NULLTAG, &iCountRel, &tRela_List);
			if (iFail == ITK_ok && tRela_List != NULL)
			{
				for (int h = 0; h < iCountRel; h++)
				{
					vRelTags.push_back(tRela_List[h]);
				}
			}
		}
		for (int n = 0; n < vRelTags.size(); n++)
		{
			tag_t tRelationType = NULLTAG;
			iFail = GRM_ask_relation_type(vRelTags[n], &tRelationType);
			if (iFail == ITK_ok && tRelationType != NULLTAG)
			{
				vRelTypeTags.push_back(tRelationType);
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		MEM_free(tRela_List);
		MEM_free(tRev_List);
		cout <<  " dataset tags size : " << vDatasetTags.size() << endl;
		cout << "----" << endl;
		cout << tPrevRev << endl;
		cout << "dataset type tags : " << vRelTypeTags.size() << endl;
		cout << '/n' << endl;
		iFail = AOM_ask_value_string(tPrevRev, "object_string", &cName);
		{
			if (iFail == ITK_ok && cName != NULL)
			{
				cout << cName << endl;
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		for (int k = 0; k < vDatasetTags.size(); k++)
		{
			if (k >= 0 && k < vRelTypeTags.size() )
			{
			    GRM_create_relation(tPrevRev, vDatasetTags[k], vRelTypeTags[k], NULLTAG, &tRel_new);
				cout <<  " new relation tag : " << tRel_new << endl;
				GRM_save_relation(tRel_new);
			}
		}

		return iFail;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}