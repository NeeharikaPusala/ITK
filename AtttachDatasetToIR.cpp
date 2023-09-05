#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <ae/datasettype.h>
#include <ae/dataset.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	char* cError = NULL;
	tag_t tItem = NULLTAG;
	tag_t tRevision = NULLTAG;
	tag_t tDataset = NULLTAG;
	tag_t tNewData = NULLTAG;
	tag_t tRelation = NULLTAG;
	iFail = ITK_auto_login();
	if (iFail == ITK_ok)
	{
		cout << "Login Success" << endl;
		iFail = ITEM_find_rev("000000", "a", &tRevision);
		if (tRevision != NULLTAG && iFail == ITK_ok)
		{
			cout << "Revision found successfully" << endl;
			iFail = AE_find_datasettype2("PDF", &tDataset);
			if (tDataset != NULLTAG && iFail == ITK_ok)
			{
				cout << "Dataset type found successfully" << endl;
				iFail = AE_create_dataset_with_id(tDataset, "NeehaDataset", "dataset_nee", "000001", "B", &tNewData);
				if (&tNewData != NULLTAG && iFail == ITK_ok)
				{
					cout << "new dataset created successfully" << endl;
					iFail = AOM_save(tNewData);
					if (iFail == ITK_ok)
					{
						cout << "Dataset saved successfully" << endl;
						iFail = GRM_find_relation_type("IMAN_reference", &tRelation);
						if (tRelation != NULLTAG && iFail == ITK_ok)
						{
							cout << "Relation found successfully" << endl;
							iFail = GRM_create_relation(tRevision, tDataset, tRelation, NULLTAG, &tRelation);
							if (iFail == ITK_ok && tRelation != NULLTAG)
							{
								cout << "Relation created successfully" << endl;
								iFail = GRM_save_relation(tRelation);
								if (iFail == ITK_ok)
								{
									cout << "Relation saved successfully" << endl;
								}
								else
								{
									EMH_ask_error_text(iFail, &cError);
									cout << cError;
								}

							}
							else
							{
								EMH_ask_error_text(iFail, &cError);
								cout << cError;
							}
						}
						else
						{
							EMH_ask_error_text(iFail, &cError);
							cout << cError;
						}

					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError;
					}
				}
				else
				{
					EMH_ask_error_text(iFail, &cError);
					cout << cError;
				}
			}
			else
			{
				EMH_ask_error_text(iFail, &cError);
				cout << cError;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError;
		}
	}

	return iFail;
}
