//To retrieve puid, object_name of items created between a date range.
#include "header.h"
using namespace std;
int checkError(int fail)
{
	char* cError = NULL;
	if (fail == 0)
	{
		cout << "OK" << "\n";
	}
	else
	{
		EMH_ask_error_text(fail, &cError);
		cout << cError << "\n";
	}
	return fail;
}
int ITK_user_main(int argc, char* argv[])
{
	int iFail = 0;
	vector<tag_t> vObjs;
	checkError(ITK_init_module("infodba", "infodba", "dba"));
	checkError(POM_enquiry_create("query1"));
	const char* select_attrs[] = {  "puid","creation_date", "object_name"};
	const char select_attrs1[] = { "creation_date" };
	date_t StartDate = NULLDATE;
	date_t EndDate = NULLDATE;
	int rows, col = 0;
	void*** report = NULL;
	checkError(ITK_string_to_date("22-Apr-2024", &StartDate));
	checkError(ITK_string_to_date("17-May-2024", &EndDate));
	checkError(POM_enquiry_add_select_attrs("query1", "PC2Software", 3, select_attrs));
	checkError(POM_enquiry_set_date_expr("query1", "ExpID1", "PC2Software", select_attrs1, POM_enquiry_greater_than, StartDate));
	checkError(POM_enquiry_set_date_expr("query1", "ExpID2", "PC2Software", select_attrs1, POM_enquiry_less_than, EndDate));
	checkError(POM_enquiry_set_expr("query1", "ExpID3", "ExpID1", POM_enquiry_and, "ExpID2"));
	checkError(POM_enquiry_set_where_expr("query1", "ExpID3"));
	checkError(POM_enquiry_execute("query1", &rows, &col, &report));
	cout << "no of rows : " << rows << "\n";
	if (rows > 0) {
		for (int i = 0; i < rows; i++) {
			tag_t obj = *(tag_t*)report[i][0];
			date_t crDate = *(date_t*)report[i][1];
			char* objNamePtr = (char*)report[i][2];
			string objName;
			if (objNamePtr != nullptr) {
				objName = string(objNamePtr); // Convert char* to string
			}
			else {
				// Handle the case where objNamePtr is nullptr
				objName = "Unknown"; // Or any default value
			}

		    char* newDate = NULL;
			ITK_date_to_string(crDate, &newDate);
			cout << obj << "\n";
			cout << "newDate:" << newDate << "\n";
			cout << "objName:" << objName << "\n";
		}
	}

	return iFail;
}