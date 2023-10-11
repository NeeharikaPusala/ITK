#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <fclasses/tc_string.h>
#include <tccore/item.h>
#include <epm/epm_task_template_itk.h>
#include <sa/user.h>
using namespace std;
void printError(int error)
{
	char* cError = NULL;
	EMH_ask_error_text(error, &cError);
	cout << cError << "\n";

}
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	int ifail = 0;
	char* cError = NULL;
	
	tag_t tProcess_template = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tNewProcess = NULLTAG;
	tag_t tRoot_task = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tSubTask = NULLTAG;
	char* cName = NULL;
	int iCount = 0;
	tag_t* tTasks = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		ifail = ITK_init_module(cUsername, cPassword, cGroup);
		if (ifail == ITK_ok)
		{
			cout << "Login Success" << endl;
			ifail = EPM_find_process_template("ITK_task", &tProcess_template);
			if (ifail == ITK_ok && tProcess_template != NULL)
			{
				ifail = ITEM_find_rev("000600", "A", &tRev);
				tag_t attachments[1];
				attachments[0] = tRev;
				int attachment_types[1];
				attachment_types[0] = EPM_target_attachment;

				ifail = EPM_create_process("itk task", "", tProcess_template, 1, attachments, attachment_types, &tNewProcess);
				if (ifail == ITK_ok && tNewProcess != NULL)
				{
					cout << "workflow initiated " << "\n";
					ifail = EPM_ask_root_task(tNewProcess, &tRoot_task);
					if (ifail == ITK_ok && tRoot_task != NULLTAG)
					{
						
						ifail = EPM_ask_sub_task(tRoot_task, "New Do Task 1", &tSubTask);
						if (ifail == ITK_ok && tSubTask != NULLTAG)
						{
							AOM_ask_value_string(tSubTask, "object_name", &cName);
							cout << cName << "\n";
							ifail = EPM_set_task_result(tSubTask, EPM_RESULT_Completed);
							if (ifail == ITK_ok)
							{
								cout << "complete radio button selected ";
								ifail = EPM_trigger_action(tSubTask, 4, "comment");
								if (ifail == ITK_ok)
								{
									cout << "\n" << "workflow completed" << "\n";
								}
							}
							
						}
						else
						{
							printError(ifail);
						}
					}
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
		}
		else
		{
			printError(ifail);
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}

	return ifail;
}