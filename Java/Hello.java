//==================================================
//
//  Copyright 2020 Siemens Digital Industries Software
//
//==================================================

package com.teamcenter.hello;

import com.teamcenter.clientx.AppXSession;
import com.teamcenter.schemas.soa._2006_03.exceptions.ServiceException;
import com.teamcenter.services.strong.core._2006_03.DataManagement.ItemIdsAndInitialRevisionIds;
import com.teamcenter.soa.client.model.strong.User;
import java.util.Scanner;


/**
 * This sample client application demonstrates some of the basic features of the
 * Teamcenter Services framework and a few of the services.
 *
 * An instance of the Connection object is created with implementations of the
 * ExceptionHandler, PartialErrorListener, ChangeListener, and DeleteListeners
 * intefaces. This client application performs the following functions:
 * 1. Establishes a session with the Teamcenter server
 * 2. Display the contents of the Home Folder
 * 3. Performs a simple query of the database
 * 4. Create, revise, and delete an Item
 *
 */
public class Hello
{

    /**
     * @param args   -help or -h will print out a Usage statement
     * @throws ServiceException 
     */
    public static void main(String[] args) throws ServiceException
    {
        if (args.length > 0)
        {
            if (args[0].equals("-help") || args[0].equals("-h"))
            {
                System.out.println("usage: java [-Dhost=http://server:port/tc] com.teamcenter.hello.Hello");
                System.exit(0);
            }
        }

        // Get optional host information
        String serverHost = "http://INHYD0030DC:8080/tc";
        String host = System.getProperty("host");
        if (host != null && host.length() > 0)
        {
            serverHost = host;
        }

        

        AppXSession   session = new AppXSession(serverHost);
        
        DataManagement dm = new DataManagement();
        
        User user = session.login();
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter number of item");
        int noOfItems = sc.nextInt();
        ItemIdsAndInitialRevisionIds[] itemsAndRevisionsArray = new ItemIdsAndInitialRevisionIds[noOfItems];
        for(int i=0; i<noOfItems; i++)
        {
        	ItemIdsAndInitialRevisionIds revIDs = new ItemIdsAndInitialRevisionIds();
        	System.out.print("Enter the new item ID for item " + (i + 1) + ": ");
        	revIDs.newItemId = sc.next();
        	System.out.print("Enter the new revision ID for item " + (i + 1) + ": ");
        	revIDs.newRevId = sc.next();
        	itemsAndRevisionsArray[i] = revIDs;
        	System.out.println("enter item name:");
        	
        	
        	
        }
        sc.close();
        dm.createItems(itemsAndRevisionsArray, "Item");
        session.logout();
        
        


    }

    
}
