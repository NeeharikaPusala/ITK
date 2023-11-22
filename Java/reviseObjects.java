package com.teamcenter.hello;

import java.util.Scanner;

import com.teamcenter.clientx.AppXSession;
import com.teamcenter.schemas.soa._2006_03.exceptions.ServiceException;
import com.teamcenter.services.strong.core.DataManagementService;
import com.teamcenter.services.strong.core._2008_06.DataManagement.ReviseInfo;
import com.teamcenter.services.strong.core._2008_06.DataManagement.ReviseResponse2;
import com.teamcenter.services.strong.query.SavedQueryService;
import com.teamcenter.services.strong.query._2006_03.SavedQuery.GetSavedQueriesResponse;
import com.teamcenter.services.strong.query._2007_09.SavedQuery.QueryResults;
import com.teamcenter.services.strong.query._2007_09.SavedQuery.SavedQueriesResponse;
import com.teamcenter.services.strong.query._2008_06.SavedQuery.QueryInput;
import com.teamcenter.soa.client.model.ModelObject;
import com.teamcenter.soa.client.model.Property;
import com.teamcenter.soa.client.model.ServiceData;
import com.teamcenter.soa.client.model.strong.ImanQuery;
import com.teamcenter.soa.client.model.strong.ItemRevision;
import com.teamcenter.soa.exceptions.NotLoadedException;

public class reviseObjects {
	public static void reviseIR() throws NotLoadedException
	{
		
		ModelObject[] revObjArr = null;
		
		reviseObjects obj = new reviseObjects();
		
		Scanner sc = new Scanner(System.in);
		
		System.out.println("Enter TC Query for finding Item Revisions");
		
		final String queryName = sc.nextLine();
		
		System.out.println("Enter Item ID");
		
		final String itemID = sc.next();
		
		System.out.println("Enter Rev ID");
		
		final String revisionID = sc.next();
		
		revObjArr =  obj.queryItemsFoIR(queryName, itemID, revisionID);
		
		ItemRevision newItemRev = (ItemRevision)revObjArr[0];
		
		ReviseInfo[] revInfoObj = new ReviseInfo[1];
		
		revInfoObj[0] = new ReviseInfo();
		
		revInfoObj[0].baseItemRevision = newItemRev;
		
		revInfoObj[0].name = "new rev from SOA";
		
		revInfoObj[0].description = "description revision from SOA";
		
		revInfoObj[0].newRevId = "B";
		
		System.out.println("length of model obj array is : " + revObjArr.length);
		
		DataManagementService dmService= DataManagementService.getService(AppXSession.getConnection());
		
		
		
		
		String[] propNames = {"release_status_list"};
		
        String[] releaseValue = {"object_name"};
        
        for (int i = 0; i < revInfoObj.length; i++) 
        {
        	
			dmService.getProperties(revObjArr, propNames);
			
			Property propertObject = revObjArr[0].getPropertyObject(propNames[0]);
			
			System.out.println("The property object is : " + propertObject);
			
			if(propertObject != null)
			{
				ModelObject[] releaseStatusObjects = propertObject.getModelObjectArrayValue();
				
				System.out.println("Release status objects count : " + releaseStatusObjects.length);
				
				if(releaseStatusObjects != null && releaseStatusObjects.length > 0)
				{
					ReviseResponse2 reviseResp = dmService.revise2(revInfoObj);
				}
				else 
				{
					System.out.println("Item Revision is not having any release status....cannot revise IR");
				}
			}
			else 
			{
				System.out.println("Not able to get the property object");
			}
        }
        	
	}
        	 
           

	
	public ModelObject[] queryItemsFoIR(String queryName, String itemID,  String revisionID)
    {

        ImanQuery query = null;
       

        // Get the service stub.
        SavedQueryService queryService = SavedQueryService.getService(AppXSession.getConnection());
        DataManagementService dmService= DataManagementService.getService(AppXSession.getConnection());
        try
        {

            // *****************************
            // Execute the service operation
            // *****************************
            GetSavedQueriesResponse savedQueries = queryService.getSavedQueries();


            if (savedQueries.queries.length == 0)
            {
                System.out.println("There are no saved queries in the system.");
                return null;
            }

            // Find one called 'Item Name'
            for (int i = 0; i < savedQueries.queries.length; i++)
            {

                if (savedQueries.queries[i].name.equals(queryName))
                {
                    query = savedQueries.queries[i].query;
                    break;
                }
            }
        }
        catch (ServiceException e)
        {
            System.out.println("GetSavedQueries service request failed.");
            System.out.println(e.getMessage());
            return null;
        }

        if (query == null)
        {
            System.out.println("There is not an 'Item Name' query.");
            return null;
        }

        ModelObject[] foundObjs = null;
     

        try {
            // Search for Item Revisions, returning a maximum of 25 objects
            QueryInput savedQueryInput[] = new QueryInput[1];
            savedQueryInput[0] = new QueryInput();
            savedQueryInput[0].query = query;
            savedQueryInput[0].maxNumToReturn = 25;
            savedQueryInput[0].limitList = new ModelObject[0];
            savedQueryInput[0].entries = new String[]{"Item ID", "Revision"}; // Add both Item ID and Revision ID
            savedQueryInput[0].values = new String[2]; // Update to accommodate both values
            savedQueryInput[0].values[0] = itemID; // Set Item ID
            savedQueryInput[0].values[1] = revisionID; // Set Revision ID

            // Execute the service operation
            SavedQueriesResponse savedQueryResult = queryService.executeSavedQueries(savedQueryInput);
            QueryResults found = savedQueryResult.arrayOfResults[0];

            System.out.println("");
            System.out.println("Found Items:");

            // Page through the results 10 at a time
            for (int i = 0; i < found.objectUIDS.length; i += 10) {
                int pageSize = (i + 10 < found.objectUIDS.length) ? 10 : found.objectUIDS.length - i;

                String[] uids = new String[pageSize];
                for (int j = 0; j < pageSize; j++) {
                    uids[j] = found.objectUIDS[i + j];
                }
                ServiceData sd = dmService.loadObjects(uids);
                foundObjs = new ModelObject[sd.sizeOfPlainObjects()];
                for (int k = 0; k < sd.sizeOfPlainObjects(); k++) {
                    foundObjs[k] = sd.getPlainObject(k);
                }

                AppXSession.printObjects(foundObjs);
                
            }
        } catch (Exception e) {
            System.out.println("ExecuteSavedQuery service request failed.");
            System.out.println(e.getMessage());
            return null;
        }
		return  foundObjs;


    }
}
