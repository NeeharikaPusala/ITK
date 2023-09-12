#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <tccore\item.h>
#include <bom/bom.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <ps/ps.h>
#include <string>
#include <fclasses/tc_string.h>
#include <fstream>
#include <vector>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	tag_t tItemRevGF = NULLTAG;
	tag_t tItemRevSON = NULLTAG;
	tag_t tItemRevDaughter = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tWindow1 = NULLTAG;
	tag_t tWindow2 = NULLTAG;
	tag_t tNewLineSON = NULLTAG;
	tag_t tNewLineDaughter = NULLTAG;
	tag_t tNewLineGrandSon = NULLTAG;
	tag_t tNewLineGrandDaughter = NULLTAG;
	tag_t tITEM_TAG_REV = NULLTAG;
	tag_t tBomView = NULLTAG;
	tag_t tBomViewRev = NULLTAG;
	tag_t tBomViewSon = NULLTAG;
	tag_t tBomViewRevSon = NULLTAG;
	tag_t tBomViewDaughter = NULLTAG;
	tag_t tBomViewRevDaughter = NULLTAG;
	tag_t t_top_bom_line = NULLTAG;
	tag_t t_top_line_son = NULLTAG;
	tag_t t_top_line_daughter = NULLTAG;
	string line;
	vector<tag_t> vItemRevTags;
	int iFail = 0;
	char* cError = NULL;
	if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	{
		iFail = ITK_init_module(cUsername, cPassword, cGroup);
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			ifstream myFile("C:\\Users\\T50391\\Downloads\\multi_lev_bom_itemID.txt");
			if (myFile.is_open())
			{
				while (getline(myFile, line))
				{
					iFail = ITEM_find_rev(line.c_str(), "A", &tITEM_TAG_REV);
					if (tITEM_TAG_REV != NULLTAG)
					{
						vItemRevTags.push_back(tITEM_TAG_REV);
					}
					else
					{
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}
				}
			}
			if (!vItemRevTags.empty())
			{
				iFail = BOM_create_window(&tWindow);
				if (iFail == ITK_ok && tWindow != NULLTAG)
				{
					for (int i = 0; i < vItemRevTags.size(); i++)
					{
						if (i == 0)
						{
							iFail = PS_create_bv_with_bvr("parent bv", "parent bv_desc", NULLTAG, "bvr", "bvr_desc", false, vItemRevTags[i], &tBomView, &tBomViewRev);
							if (iFail == ITK_ok && tBomView != NULLTAG && tBomViewRev != NULLTAG)
							{
								iFail = BOM_set_window_top_line(tWindow, NULLTAG, vItemRevTags[i], NULLTAG, &t_top_bom_line);
								if (iFail == ITK_ok && t_top_bom_line != NULLTAG)
								{
									cout << "Top line added successfully : " << endl ;
									BOM_save_window(tWindow);
								}
								else
								{
									cout << "not able to add top line : ";
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								cout << "bv and bvr not created : ";
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else if (i == 1)
						{
							iFail = BOM_line_add(t_top_bom_line, NULLTAG, vItemRevTags[i], NULLTAG, &tNewLineSON);
							if (iFail == ITK_ok && tNewLineSON != NULLTAG)
							{
								cout << "son line added successfully : ";
								BOM_save_window(tWindow);
							}
							else
							{
								cout << "son line not added successfully : ";
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else if (i == 2)
						{
							iFail = BOM_line_add(t_top_bom_line, NULLTAG, vItemRevTags[i], NULLTAG, &tNewLineDaughter);
							if (iFail == ITK_ok && tNewLineDaughter != NULLTAG)
							{
								cout << "daughter line added successfully : " << endl;
								BOM_save_window(tWindow);
							}
							else
							{
								cout << "daughter line not added successfully : ";
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else if (i == 3)
						{
							iFail = BOM_create_window(&tWindow1);
							if (iFail == ITK_ok && tWindow1 != NULLTAG)
							{
								cout << "BOM window1 created : ";
								iFail = PS_create_bv_with_bvr("son bv", "son bv_desc", NULLTAG, "son bvr", "son bvr_desc", false, vItemRevTags[i - 2], &tBomViewSon, &tBomViewRevSon);
								if (iFail == ITK_ok && tBomViewRevSon != NULLTAG && tBomViewRevSon != NULLTAG)
								{
									cout << "bv and bvr created for son : " << endl;
									iFail = BOM_set_window_top_line(tWindow1, NULLTAG, vItemRevTags[i - 2], NULLTAG, &t_top_line_son);
									if (iFail == ITK_ok && t_top_bom_line != NULLTAG)
									{
										cout << "top line added for son : " << endl;
										iFail = BOM_line_add(t_top_line_son, NULLTAG, vItemRevTags[i], NULLTAG, &tNewLineGrandSon);
										if (iFail == ITK_ok && tNewLineGrandSon != NULLTAG)
										{
											cout << "grand son added : \n";
											BOM_save_window(tWindow1);
										}
										else
										{
											cout << "grand son not added : \n";
											EMH_ask_error_text(iFail, &cError);
											cout << cError << endl;
										}
									}
									else
									{
										cout << "top line not added for son : " << endl;
										EMH_ask_error_text(iFail, &cError);
										cout << cError << endl;
									}
								}
								else
								{
									cout << "bv and bvr not created for son : " << endl;
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								cout << "bom window1 not created : " << endl;
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else if (i == 4)
						{
							iFail = BOM_create_window(&tWindow2);
							if (iFail == ITK_ok && tWindow2 != NULLTAG)
							{
								cout << "window2 created for daughter : " << endl;
								iFail = PS_create_bv_with_bvr("daughter bv", "daughter bv_desc", NULLTAG, "daughter bvr", "daughter bvr_desc", false, vItemRevTags[i - 2], &tBomViewDaughter, &tBomViewRevDaughter);
								if (iFail == ITK_ok && tBomViewDaughter != NULLTAG && tBomViewRevDaughter != NULLTAG)
								{
									cout << "bv and bvr created for daughter : " << endl;
									iFail = BOM_set_window_top_line(tWindow2, NULLTAG, vItemRevTags[i - 2], NULLTAG, &t_top_line_daughter);
									if (iFail == ITK_ok && t_top_line_daughter != NULLTAG)
									{
										cout << "top line added for daughter : \n";
										iFail = BOM_line_add(t_top_line_daughter, NULLTAG, vItemRevTags[i], NULLTAG, &tNewLineGrandDaughter);
										if (iFail == ITK_ok && tNewLineGrandDaughter != NULLTAG)
										{
											cout << "grand daughter added : \n ";
											BOM_save_window(tWindow2);
										}
										else
										{
											cout << "grand daughter not added : \n ";
											EMH_ask_error_text(iFail, &cError);
											cout << cError << endl;
										}
									}
									else
									{
										cout << "top line not added for daughter : \n";
										EMH_ask_error_text(iFail, &cError);
										cout << cError << endl;
									}
								}
								else
								{
									cout << "bv and bvr created for daughter : " << endl;
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								cout << "window2 not created for daughter : " << endl;
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else
						{
							cout << "not iten rev tags in vector : \n";
						}
					}
				}
				else
				{
					cout << "Bom window not createdd: ";
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "vector is empty : \n";
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
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}

	return iFail;
}