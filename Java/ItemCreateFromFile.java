package com.teamcenter.hello;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import com.teamcenter.clientx.AppXSession;     

import com.teamcenter.services.strong.core.DataManagementService;
import com.teamcenter.services.strong.core._2006_03.DataManagement.ItemProperties;
import com.teamcenter.soa.client.model.strong.Folder;
import com.teamcenter.soa.client.model.strong.User;
import com.teamcenter.soa.exceptions.NotLoadedException;


public class ItemCreateFromFile {
	public ItemCreateFromFile(AppXSession session2, String filePath) {
        DataManagementService service = DataManagementService.getService(AppXSession.getConnection());
        User user = session2.login();
        Folder folderHome = null;
		try {
			folderHome = user.get_home_folder();
		} catch (NotLoadedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            // Skip the header
            br.readLine();

            // Count the number of items in the file
            long itemCount = br.lines().count();

            // Reset the BufferedReader to the beginning of the file
            br.close();
            BufferedReader br1 = new BufferedReader(new FileReader(filePath));
            br1.readLine(); // Skip the header

            ItemProperties[] items = new ItemProperties[(int) itemCount];
            String line;

            int index = 0;
            while ((line = br1.readLine()) != null) {
                String[] fields = line.split("\\|");
                ItemProperties item = new ItemProperties();
                item.itemId = fields[0];
                item.name = fields[1];
                item.type = fields[2];
                item.description = fields[3];
                items[index++] = item;
            }
            service.createItems(items, folderHome, "contents");
            System.out.println("Items created from file.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
	
}
