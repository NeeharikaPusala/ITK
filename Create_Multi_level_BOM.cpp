#include <iostream>
#include <tc\tc_startup.h>
#include <tcinit\tcinit.h>
#include <tc\emh.h>
#include <bom/bom.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <ps/ps.h>
#include <tccore\item.h>
#include <fclasses/tc_string.h>
using namespace std;
int ITK_user_main(int argc, char* argv[])
{
	char* cUsername = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	tag_t tItem_rev = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tWindow1 = NULLTAG;
	tag_t tWindow2 = NULLTAG;
	tag_t tBom_view = NULLTAG;
	tag_t tBomViewRev = NULLTAG;
	tag_t tBom_view1 = NULLTAG;
	tag_t tBom_viewRev1 = NULLTAG;
	tag_t tBom_view2 = NULLTAG;
	tag_t tBom_viewRev2 = NULLTAG;
	tag_t t_top_bom_line = NULLTAG;
	tag_t t_top_line_child1 = NULLTAG;
	tag_t t_top_line_child2 = NULLTAG;
	tag_t tItemRev_child1 = NULLTAG;
	tag_t tItemRev_child2 = NULLTAG;
	tag_t tNewLine1 = NULLTAG;
	tag_t tNewLine2 = NULLTAG;
	tag_t tNewLine8GB = NULLTAG;
	tag_t tNewLine6GB = NULLTAG;
	tag_t tNewLine6in = NULLTAG;
	tag_t tNewLine5in = NULLTAG;
	tag_t tItemRev8GB = NULLTAG;
	tag_t tItemRev6GB = NULLTAG;
	tag_t tItemRev6Inch = NULLTAG;
	tag_t tItemRev5Inch = NULLTAG;
	int iFail = 0;
	char* cError = NULL;
	//if (tc_strcmp(cUsername, NULL) == 1 || tc_strcmp(cUsername, "") == 1 || tc_strcmp(cPassword, NULL) == 1 || tc_strcmp(cPassword, "") == 1 || tc_strcmp(cGroup, NULL) == 1 || tc_strcmp(cGroup, "") == 1)
	//{
		iFail = ITK_init_module("infodba", "infodba", "dba");
		if (iFail == ITK_ok)
		{
			cout << "Login Success" << endl;
			iFail = ITEM_find_rev("000472","A", &tItem_rev);
			if (iFail == ITK_ok && tItem_rev != NULLTAG)
			{
				cout << "Item rev found successfully \n ";
				iFail = BOM_create_window(&tWindow);
				if (iFail == ITK_ok && tWindow != NULLTAG)
				{
					cout << "BOM window created "<< endl;
					iFail = PS_create_bv_with_bvr("parent bv", "bv_desc", NULLTAG, "prent bvr name", "bvr_desc", false, tItem_rev, &tBom_view, &tBomViewRev);
					if (iFail == ITK_ok && tBom_view != NULLTAG && tBomViewRev != NULLTAG)
					{
						cout << "bv and bvr created \n";
						iFail = BOM_set_window_top_line(tWindow, NULLTAG, tItem_rev, tBom_view, &t_top_bom_line);
						if (iFail == ITK_ok && t_top_bom_line != NULLTAG)
						{
							cout << "top line added : \n";
							iFail = ITEM_find_rev("000473", "A", &tItemRev_child1);
							if (iFail == ITK_ok && tItemRev_child1 != NULLTAG)
							{
								cout << "child 1 found successfully " << endl;
								iFail = ITEM_find_rev("000474", "A", &tItemRev_child2);
								if (iFail == ITK_ok && tItemRev_child2 != NULLTAG)
								{
									cout << "child 2 found successfully " << endl;
									iFail = BOM_line_add(t_top_bom_line, NULLTAG, tItemRev_child1, NULLTAG, &tNewLine1);
									if (iFail == ITK_ok && tNewLine1 != NULLTAG)
									{
										cout << "child1 added " << endl;
										iFail = ITEM_find_rev("000475", "A", &tItemRev8GB);
										if (iFail == ITK_ok && tItemRev8GB != NULLTAG)
										{
											cout << "8gb item found : " << endl;
											iFail = BOM_create_window(&tWindow1);
											if (iFail == ITK_ok && tWindow1 != NULLTAG)
											{
												cout << "window 1 created successfully : " << endl;
												iFail = PS_create_bv_with_bvr("8gb bv", "8gb_desc", NULLTAG, "8gb bv rev", "8gb bv rev desc", false, tItemRev_child1, &tBom_view1, &tBom_viewRev1);
												if (iFail == ITK_ok && tBom_view1 != NULLTAG && tBom_viewRev1 != NULLTAG)
												{
													cout << "bv and bvr created for child1 : \n";
													iFail = BOM_set_window_top_line(tWindow1, NULLTAG, tItemRev_child1, tBom_view1, &t_top_line_child1);
													if (iFail == ITK_ok && t_top_line_child1 != NULLTAG)
													{
														cout << "top line added child1 : \n";
														iFail = BOM_line_add(t_top_line_child1, NULLTAG, tItemRev8GB, NULLTAG, &tNewLine8GB);
														cout << tNewLine8GB << endl;
														if (iFail == ITK_ok && tNewLine8GB != NULLTAG)
														{
															cout << "8gb item added : \n";
															iFail = ITEM_find_rev("000476", "A", &tItemRev6GB);
															if (iFail == ITK_ok && tItemRev6GB != NULLTAG)
															{
																cout << "6gb item found : " << endl;
																iFail = BOM_line_add(t_top_line_child1, NULLTAG, tItemRev6GB, NULLTAG, &tNewLine6GB);
																if (iFail == ITK_ok && tNewLine6GB != NULLTAG)
																{
																	cout << "6gb item added : \n";
																	BOM_save_window(tWindow1);
																}
																else
																{
																	cout << "6gb item not added : \n";
																	EMH_ask_error_text(iFail, &cError);
																	cout << cError << endl;
																}
															}
															else
															{
																cout << "6gb item not found : \n";
																EMH_ask_error_text(iFail, &cError);
																cout << cError << endl;
															}
														}
														else
														{
															cout << "8gb item not added : \n";
															EMH_ask_error_text(iFail, &cError);
															cout << cError << endl;
														}
													}
													else
													{
														cout << "top line added child1 : \n";
														EMH_ask_error_text(iFail, &cError);
														cout << cError << endl;
													}
												}
												else
												{
													cout << "bv and bvr not created for child1 : \n";
													EMH_ask_error_text(iFail, &cError);
													cout << cError << endl;
												}
											}
											else
											{
												cout << "window 1 not created successfully : " << endl;
												EMH_ask_error_text(iFail, &cError);
												cout << cError << endl;
											}
										}
										else
										{
											cout << "8gb item not found : " << endl;
											EMH_ask_error_text(iFail, &cError);
											cout << cError << endl;
										}
										iFail = BOM_line_add(t_top_bom_line, NULLTAG, tItemRev_child2, NULLTAG, &tNewLine2);
										if (iFail == ITK_ok && tNewLine2 != NULLTAG)
										{
											cout << "child2 added \n";
											iFail = ITEM_find_rev("000477", "A", &tItemRev6Inch);
											if (iFail == ITK_ok && tItemRev6Inch != NULLTAG)
											{
												cout << "6 inch item found : \n";
												iFail = BOM_create_window(&tWindow2);
												if (iFail == ITK_ok && tWindow2 != NULLTAG)
												{
													cout << "Bom window created for child2 \n";
													iFail = PS_create_bv_with_bvr("6in bv", "bv_desc", NULLTAG, "6in bvr", "bvr_desc", false, tItemRev_child2, &tBom_view2, &tBom_viewRev2);
													if (iFail == ITK_ok && tBom_view2 != NULLTAG && tBom_viewRev2 != NULLTAG)
													{
														cout << "bv and bvr created for child2 \n";
														iFail = BOM_set_window_top_line(tWindow2, NULLTAG, tItemRev_child2, tBom_view2, &t_top_line_child2);
														if (iFail == ITK_ok && t_top_line_child2 != NULLTAG)
														{
															cout << "Bom top line set for child2 : \n";
															iFail = BOM_line_add(t_top_line_child2, NULLTAG, tItemRev6Inch, NULLTAG, &tNewLine6in);
															if (iFail == ITK_ok && tNewLine6in != NULLTAG)
															{
																cout << "6inc item added : " << endl;
																iFail = ITEM_find_rev("000478", "A", &tItemRev5Inch);
																if (iFail == ITK_ok && tItemRev5Inch != NULLTAG)
																{
																	cout << "5 inch item found " << endl;
																	iFail = BOM_line_add(t_top_line_child2, NULLTAG, tItemRev5Inch, NULLTAG, &tNewLine5in);
																	if (iFail == ITK_ok && tNewLine5in != NULLTAG)
																	{
																		cout << "5 inch item added successfully : ";
																		BOM_save_window(tWindow2);
																		BOM_save_window(tWindow);
																	}
																	else
																	{
																		cout << "5 inc item not added : ";
																		EMH_ask_error_text(iFail, &cError);
																		cout << cError << endl;
																	}
																}
																else
																{
																	cout << "5 inch item not found : ";
																	EMH_ask_error_text(iFail, &cError);
																	cout << cError << endl;
																}
																/*BOM_save_window(tWindow2);
																BOM_save_window(tWindow); */
															}
															else
															{
																cout << "6inc item not added : ";
																EMH_ask_error_text(iFail, &cError);
																cout << cError << endl;
															}
														}
														else
														{
															cout << "Bom top line not set for child2 :";
															EMH_ask_error_text(iFail, &cError);
															cout << cError << endl;
														}
													}
													else
													{
														cout << "bv and bvr not created for child2 : ";
														EMH_ask_error_text(iFail, &cError);
														cout << cError << endl;
													}
												}
												else
												{
													cout << "Bom window not created for child2 : ";
													EMH_ask_error_text(iFail, &cError);
													cout << cError << endl;
												}
											}
											else
											{
												cout << "6 inch item not found : ";
												EMH_ask_error_text(iFail, &cError);
												cout << cError << endl;
											}
										}
										else
										{
											cout << "child2 not added : ";
											EMH_ask_error_text(iFail, &cError);
											cout << cError << endl;
										}
									}
									else
									{
										cout << "child1 not added : ";
										EMH_ask_error_text(iFail, &cError);
										cout << cError << endl;
									}
								}
								else
								{
									cout << "child 2 not found : ";
									EMH_ask_error_text(iFail, &cError);
									cout << cError << endl;
								}
							}
							else
							{
								cout << "child 1 not found : ";
								EMH_ask_error_text(iFail, &cError);
								cout << cError << endl;
							}
						}
						else
						{
							cout << "top line not added : ";
							EMH_ask_error_text(iFail, &cError);
							cout << cError << endl;
						}
					}
					else
					{
						cout << "bv and bvr are not created : ";
						EMH_ask_error_text(iFail, &cError);
						cout << cError << endl;
					}
				}
				else
				{
					cout << "BOM window not created : ";
					EMH_ask_error_text(iFail, &cError);
					cout << cError << endl;
				}
			}
			else
			{
				cout << "Item rev not found : ";
				EMH_ask_error_text(iFail, &cError);
				cout << cError << endl;
			}
		}
		else
		{
			EMH_ask_error_text(iFail, &cError);
			cout << cError << endl;
		}
	return iFail;
}
/*else
	{
		cout << "Usage: " << argv[0] << " -u= <cUsername> -p= <cPassword> -g= <cGroup>" << endl;
	}*/