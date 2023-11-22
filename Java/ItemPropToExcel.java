package com.teamcenter.hello;
import com.teamcenter.clientx.AppXSession;
import com.teamcenter.schemas.soa._2006_03.exceptions.ServiceException;
import com.teamcenter.soa.client.model.ServiceData;
import com.teamcenter.services.strong.core.DataManagementService;
import com.teamcenter.services.strong.core._2007_01.DataManagement.VecStruct;
import com.teamcenter.services.strong.core._2006_03.DataManagement;
import com.teamcenter.soa.internal.client.XmlRestSender;
import com.teamcenter.soa.client.model.ModelObject;
import com.teamcenter.soa.client.model.strong.User;
import com.teamcenter.soa.exceptions.NotLoadedException;

import org.apache.poi.ss.usermodel.*;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Scanner;

import com.teamcenter.soa.client.model.ModelObject;

public class ItemPropToExcel {
    public void writeToExcel() throws NotLoadedException {
        ModelObject[] modelObjects = new ModelObject[1];
        Query tcQuery = new Query();
        Scanner sc = new Scanner(System.in);
        String itemID = new String();
        String queryName = new String();
        System.out.println("Enter item id to search:");
        itemID = sc.next();
        System.out.println("Enter TC query name:");
        queryName = sc.next();
       
        modelObjects = tcQuery.queryItems(itemID, queryName);
        Workbook workbook = new XSSFWorkbook();
        Sheet sheet = workbook.createSheet("ItemProperties");

        // Create header row
        Row headerRow = sheet.createRow(0);
        String[] headers = {"Property Name", "Property Value"};
        for (int i = 0; i < headers.length; i++) {
            Cell cell = headerRow.createCell(i);
            cell.setCellValue(headers[i]);
        }

        // Populate data rows
        int rowIndex = 1;
        for (ModelObject modelObject : modelObjects) {
            String[] propertyNames = modelObject.getPropertyNames();
            for (String propertyName : propertyNames) {
                Row dataRow = sheet.createRow(rowIndex++);
                Cell nameCell = dataRow.createCell(0);
                nameCell.setCellValue(propertyName);

                Cell valueCell = dataRow.createCell(1);
                String propertyValue = modelObject.getPropertyDisplayableValue(propertyName);
                valueCell.setCellValue(propertyValue);
            }
        }

        // Write the workbook to a file
        try (FileOutputStream fileOut = new FileOutputStream("C:\\Users\\T50391\\Downloads\\itemPropertiesToExcel.xlsx")) {
            workbook.write(fileOut);
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("exported successfully");
    }
  }



//        Workbook workbook = new XSSFWorkbook();
//        Sheet sheet = workbook.createSheet("ItemProperties");
//
//        // Create header row
//        Row headerRow = sheet.createRow(0);
//        String[] headers = {"Property Name", "Property Value"};
//        for (int i = 0; i < headers.length; i++) {
//            Cell cell = headerRow.createCell(i);
//            cell.setCellValue(headers[i]);
//        }
//
//        // Populate data rows
//        int rowIndex = 1;
//        for (ModelObject modelObject : modelObjects) {
//            String[] propertyNames = modelObject.getPropertyNames();
//            for (String propertyName : propertyNames) {
//                Row dataRow = sheet.createRow(rowIndex++);
//                Cell nameCell = dataRow.createCell(0);
//                nameCell.setCellValue(propertyName);
//
//                Cell valueCell = dataRow.createCell(1);
//                String propertyValue = modelObject.getPropertyDisplayableValue(propertyName);
//                valueCell.setCellValue(propertyValue);
//            }
//        }
//
//        // Write the workbook to a file
//        try (FileOutputStream fileOut = new FileOutputStream("C:\Users\T50391\Downloads\itemPropertiesToExcel.xlsx")) {
//            workbook.write(fileOut);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
//
//        System.out.println("Excel file created successfully!");
    
