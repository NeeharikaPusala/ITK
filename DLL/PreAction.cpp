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
		tag_t tDestRev = NULLTAG;
		tag_t* tSecond_objcts = NULL;
		char* class_name = NULL;
		char* cName = NULL;
		tag_t tClass_id = NULLTAG;
		tag_t tRel_new = NULLTAG;
		int iCountRel = 0;
		tag_t* tRela_List = NULL;
		vector <tag_t>  vRelationType;
		tag_t tPrevRev = NULLTAG;
		int iCount = 0;
		iFail = ITEM_ask_item_of_rev(tRev, &tItem);
		iFail = ITEM_list_all_revs(tItem, &iCount, &tRev_List);
		cout << iCount << endl;
		if (iCount == 1)
		{
			exit(0);
		}
		else
		{
			for (int i = 0; i < iCount; i++)
			{
				if (tRev == tRev_List[i])
				{
					tPrevRev = tRev_List[i - 1];
					tDestRev = tRev_List[i];
				}
			}

		}

		cout << "tPrevRev : " << tPrevRev << endl;
		cout << "tDestRev : " << tDestRev << endl;
		iFail = GRM_list_relation_types(&iCountRel, &tRela_List);
		for (int i = 0; i < iCountRel; i++)
		{
			iFail = GRM_list_secondary_objects_only(tDestRev, tRela_List[i], &iCountSec, &tSecond_objcts);
			if (iFail == ITK_ok && iCountSec != 0 && tSecond_objcts != NULL)
			{
				cout << iCountSec << endl;
				for (int j = 0; j < iCountSec; j++)
				{
					POM_class_of_instance(tSecond_objcts[j], &tClass_id);
					POM_name_of_class(tClass_id, &class_name);
					cout << class_name << endl;
					if (tc_strcmp(class_name, "Dataset") == 0)
					{
						iFail = GRM_create_relation(tPrevRev, tSecond_objcts[j], tRela_List[i], NULLTAG, &tRel_new);
						if (iFail == ITK_ok && tRel_new != NULLTAG)
						{
							cout << "new rel tag : " << tRel_new;
							GRM_save_relation(tRel_new);
						}
						else
						{
							continue;
						}

					}
					else
					{
						continue;
					}
				}
			}

		}


		cout << " iCountRel " << iCountRel << endl;
		return iFail;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv)
	{
		*decisison = ALL_CUSTOMIZATIONS;
		cout << "-------Logout success--------- \n";
		return status;
	}
}