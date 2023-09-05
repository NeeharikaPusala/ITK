#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/aom.h>
#include <ae/dataset.h>
#include <ae/datasettype.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	int iFail = 0;
	tag_t tDataset = NULLTAG;
	char* cError = NULL;
	//if (username && password && group)
	//{
		iFail = ITK_init_module("infodba", "infodba", "dba");
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = AE_find_dataset2("dataset", &tDataset);
			if (tDataset != NULLTAG && iFail == ITK_ok)
			{
				cout << "Dataset found " << tDataset << endl;
				//AOM_refresh(tDataset, true);
				iFail = AE_export_named_ref(tDataset, "PDF_Reference", "D:\\temp\\4563.pdf");
				//AOM_refresh(tDataset, false);
				//AOM_save_without_extensions(tDataset);
				if (iFail==ITK_ok)
				{
					cout << "exported successfully " << endl;
				}
				else
				{
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	//}
	/*else
	{
		cout << "Usage: " << argv[0] << " -u= <username> -p= <password> -g= <group>" << endl;
	}*/

	return iFail;
}