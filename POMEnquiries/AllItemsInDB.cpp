//query the released item revisions from the database.
#include "Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int fail = 0;
	int rows=0, cols = 0;
	void*** report = NULL;
	const char* selAttr[] = { "object_name", "object_type" };
	checkError(POM_enquiry_create("query1"));
	checkError(POM_enquiry_add_select_attrs("query1", "PC2Software", 2, selAttr));
	checkError(POM_enquiry_execute("query1", &rows, &cols, &report));
	cout << rows << "\n";
	if (rows > 0) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				char* value = (char*)report[i][j];
				std::cout << value << "\t";
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "No rows returned by the query." << std::endl;
	}
	return fail;
}