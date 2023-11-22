package com.teamcenter.hello;
import com.teamcenter.clientx.AppXSession;
import com.teamcenter.soa.client.model.ServiceData;
import com.teamcenter.services.strong.core.DataManagementService;
import com.teamcenter.services.strong.core._2007_01.DataManagement.VecStruct;
import com.teamcenter.soa.client.model.ModelObject;
import com.teamcenter.soa.client.model.strong.User;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
public class SetProperty
{
	public void change()
	{
		 ModelObject[] modelObjects = new ModelObject[1];
		 Query tcQuery = new Query();
		 Scanner sc = new Scanner(System.in);
		 String ItemID = new String();
		 String queryName = new String();
		 System.out.println("enter item id to searc");
		 ItemID = sc.next();
		 System.out.println("enter tc query name");
		 queryName = sc.next();
		 modelObjects = tcQuery.queryItems(ItemID, queryName);
		 
		 DataManagementService dmService = DataManagementService.getService(AppXSession.getConnection());
		 HashMap<String, VecStruct> objectPropMap = new HashMap<String, VecStruct>();
		 String[] sPropValue = {"Hello"};
		 VecStruct prop = new VecStruct();
		 prop.stringVec = sPropValue;
		 objectPropMap.put("object_desc", prop);
		 
		 ServiceData svdata =  dmService.setProperties(modelObjects, objectPropMap);
		 
		 
		
	}
	
}
