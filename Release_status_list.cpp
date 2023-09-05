
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <tccore/aom_prop.h>
#include <iostream>

using namespace std;

int ITK_user_main(int argc, char* argv[]) {
	int iFail = 0;
	char* cError = NULL;
	tag_t tRevision = NULLTAG;
	char** cDisplayValues = NULL;
	int iNumDisplayValues = 0;

	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItem_id = ITK_ask_cli_argument("-item_id=");
	char* cRevision_id = ITK_ask_cli_argument("-rev_id=");

	if (cUsername && cPassword && cGroup) {
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok) {
			cout << "Login successful" << endl;
			if (cItem_id && cRevision_id) {
				iFail = ITEM_find_rev(cItem_id, cRevision_id, &tRevision);
				if (iFail == ITK_ok && tRevision != NULLTAG) {
					cout << "Item revision found successfully" << endl;

					// Using AOM_ask_displayable_values to get display values
					iFail = AOM_ask_displayable_values(tRevision, "release_status_list", &iNumDisplayValues, &cDisplayValues);
					if (iFail == ITK_ok && iNumDisplayValues != 0 && cDisplayValues != NULL) {
						for (int i = 0; i < iNumDisplayValues; i++ ) {
							cout << "Release Status: " << cDisplayValues[i] << endl;
						}

						// Free the memory allocated by AOM_ask_displayable_values
						MEM_free(cDisplayValues);
					}
					else {
						EMH_ask_error_text(iFail, &cError);
						cout << "Error retrieving displayable values: " << cError << endl;
					}
				}
				else {
					EMH_ask_error_text(iFail, &cError);
					cout << cError;
				}
			}
			else {
				cout << "Provide " << " -item_id = <cItem_id>  -rev_id = <cRevision_id> \n";
			}
		}
		else {
			EMH_ask_error_text(iFail, &cError);
			cout << cError;
		}
	}
	else {
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}
	return iFail;
}
