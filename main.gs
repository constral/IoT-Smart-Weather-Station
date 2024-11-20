function doPost(e) {
  var sheet = SpreadsheetApp.openById('1BQjqDTZU2MV_H1nEIzAjPqWiOFBHWNmCn5B4KafBGNw').getActiveSheet();
  
  // Parse the POST request body as JSON
  var data = JSON.parse(e.postData.contents);
  
  // Append the data to the sheet
  sheet.appendRow([new Date(), data.temperature, data.humidity, data.pressure, data.altitude, data.pulseCount]);
  
  return ContentService.createTextOutput("Success");
}

function doGet(e) {
  return ContentService.createTextOutput("GET requests are not supported.");
}
