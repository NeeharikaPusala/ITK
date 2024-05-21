#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <pom/enq/enq.h>
#include <tccore/aom_prop.h>
#include <vector>
#include <string>
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