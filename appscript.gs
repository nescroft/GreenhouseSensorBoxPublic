// Enter Spreadsheet ID here
var SS = SpreadsheetApp.openById(
  "your sheets id in the url when looking at the spreadsheet"
);
var str = "";

function doPost(e) {
  var parsedData;
  var result = {};

  try {
    parsedData = JSON.parse(e.postData.contents);
  } catch (f) {
    return ContentService.createTextOutput(
      "Error in parsing request body: " + f.message
    );
  }

  if (parsedData !== undefined) {
    var flag = parsedData.format;
    if (flag === undefined) {
      flag = 0;
    }

    var sheet = SS.getSheetByName(parsedData.sheet_name);

    var date_now = Utilities.formatDate(
      new Date(),
      "Europe/Bucharest",
      "yyyy/MM/dd hh:mm:ss a"
    );

    // read and execute command from the "payload_base" string specified in Arduino code
    switch (parsedData.command) {
      case "insert_row":
        var dataArr = parsedData.values.split(",");
        sheet.insertRows(2);

        sheet.getRange("A2").setValue(date_now);

        // Loop through received values and set them dynamically
        for (var i = 0; i < dataArr.length; i++) {
          sheet.getRange(2, i + 2).setValue(dataArr[i]);
        }

        str = "Success";
        SpreadsheetApp.flush();
        break;

      case "append_row":
        var dataArr = parsedData.values.split(",");
        var publish_array = new Array();

        publish_array.push(date_now); // Add current date to publish array

        // Loop through received values and add them to publish_array
        for (var i = 0; i < dataArr.length; i++) {
          publish_array.push(dataArr[i]);
        }

        sheet.appendRow(publish_array); // publish data in publish_array after the last row of data in the sheet

        str = "Success";
        SpreadsheetApp.flush();
        break;
      case "get_controls":
        var controlsSheetName = parsedData.sheet_name; // Get sheet_name from payload
        var controlsSheet = SS.getSheetByName(controlsSheetName);
        var lastRow = controlsSheet.getLastRow();
        var range = controlsSheet.getRange(1, 1, lastRow, 2); // Get all rows, first and second column
        var data = range.getValues();

        var controlsData = {};

        // Iterate through each row in the data
        for (var i = 0; i < data.length; i++) {
          var key = data[i][0]; // First column is key
          var value = data[i][1]; // Second column is value
          controlsData[key] = value; // Add to the result object
        }

        var currentTime = Utilities.formatDate(
          new Date(),
          "Europe/Bucharest",
          "yyyy/MM/dd HH:mm:ss"
        ); //24hour format necessary for arduino night time function

        controlsData["currentTime"] = currentTime; // Add current time

        return ContentService.createTextOutput(JSON.stringify(controlsData));
        break;
    }

    return ContentService.createTextOutput(str);
  } else {
    return ContentService.createTextOutput(
      "Error! Request body empty or in incorrect format."
    );
  }
}
