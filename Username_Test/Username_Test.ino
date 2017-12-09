#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

String getContentType(String filename);
bool handleFileRead(String path);

String user[2];
int maxScore;
bool screaming;

void handleRoot();              // function prototypes for HTTP handlers
void handleLogin();
void handleLeaderboard();
void handleHistory();
void handleScream();
void handleScreaming();
void handlePost();
void handleLogout();
void handleNotFound();

void setup(void) {
  user[0] = "";
  user[1] = "";
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("MIT", "");   // add Wi-Fi networks you want to connect to
  //  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  SPIFFS.begin();
  //  server.onNotFound([]() {
  //    if (!handleFileRead(server.uri()))
  //      server.send(404, "text/plain", "404: Not Found");
  //  });

  // Display the home page
  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  //  // When the user logs in successfully, go to /login
  server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.on("/leaderboard", HTTP_POST, handleLeaderboard);
  server.on("/history", HTTP_POST, handleHistory);
  server.on("/scream", HTTP_POST, handleScream);
  server.on("/screaming", HTTP_POST, handleScreaming);
  server.on("/post", HTTP_POST, handlePost);
  server.on("/", HTTP_POST, handleLogout);
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                     // Listen for HTTP requests from clients
}

//String getContentType(String filename) { // convert the file extension to the MIME type
//  if (filename.endsWith(".html")) return "text/html";
//  else if (filename.endsWith(".css")) return "text/css";
//  else if (filename.endsWith(".js")) return "application/javascript";
//  else if (filename.endsWith(".ico")) return "image/x-icon";
//  else if (filename.endsWith(".gz")) return "application/x-gzip";
//  return "text/plain";
//}

//bool handleFileRead(String path) { // send the right file to the client (if it exists)
//  Serial.println("handleFileRead: " + path);
//  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
//  String contentType = getContentType(path);             // Get the MIME type
//  String pathWithGz = path + ".gz";
//  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
//    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
//      path += ".gz";                                         // Use the compressed version
//    File file = SPIFFS.open(path, "r");                    // Open the file
//    size_t sent = server.streamFile(file, contentType);    // Send it to the client
//    file.close();                                          // Close the file again
//    Serial.println(String("\tSent file: ") + path);
//    return true;
//  }
//  Serial.println(String("\tFile Not Found: ") + path);
//  return false;                                          // If the file doesn't exist, return false
//}

void handleRoot() {                          // When URI / is requested, send the home page
  server.send(200, "text/html", "<p>Welcome to 6.S06Scream!!!</p><form action=\"/login\" method=\"POST\">Firstname: <input type=\"text\" name=\"firstname\" placeholder=\"John\"></br>Lastname: <input type=\"text\" name=\"lastname\" placeholder=\"Doe\"></br><input type=\"submit\" value=\"Login\"></form><form action=\"/leaderboard.html\" method=\"POST\"><input type=\"submit\" value=\"Leaderboard\"></form>");

//  File readLog = SPIFFS.open("/temp.csv", "r");
//  for (int i = 0; i < 10; i++){
//    Serial.println(readLog.readStringUntil('\n'));
//  }
}

void handleLogin() {                         // If a POST request is made to URI /login
  if (user[0] != "" && user[1] != ""){
    server.send(200, "text/html", "<p>Welcome, " + user[0] + " " + user[1] + "!</p><form action=\"/history\" method=\"POST\"><input type=\"submit\" value=\"My Scores\"></form><form action=\"/scream\" method=\"POST\"><input type=\"submit\" value=\"Scream\"></form><form action=\"/leaderboard\" method=\"GET\"><input type=\"submit\" value=\"Leaderboard\"></form><form action=\"/\" method=\"POST\"><input type=\"submit\" value=\"Logout\"></form>");
  }
  else if ( ! server.hasArg("firstname") || ! server.hasArg("lastname")
       || server.arg("firstname") == NULL || server.arg("lastname") == NULL) {
    server.send(400, "text/plain", "400: Invalid Request, please enter your name");
    return;
  }
  else {
    user[0] = server.arg("firstname");
    user[1] = server.arg("lastname");
//    SPIFFS.remove("/temp.csv");
//    File tempLog = SPIFFS.open("/temp.csv", "a");
//    tempLog.print(server.arg("firstname") + " " + server.arg("lastname") + ".\n");
//    tempLog.close();

    server.send(200, "text/html", "<p>Welcome, " + user[0] + " " + user[1] + "!</p><form action=\"/history\" method=\"POST\"><input type=\"submit\" value=\"My Scores\"></form><form action=\"/scream\" method=\"POST\"><input type=\"submit\" value=\"Scream\"></form><form action=\"/leaderboard\" method=\"GET\"><input type=\"submit\" value=\"Leaderboard\"></form><form action=\"/\" method=\"POST\"><input type=\"submit\" value=\"Logout\"></form>");
  }
}

void handleLeaderboard(){
  
}

void handleHistory(){
  
}

void handleScream(){
  if (maxScore != 0){ // If the user came back from /screaming, then there is a maxScore for the session
    screaming = false;
    String maxScoreString = String(maxScore);
    server.send(200, "text/html", "<p>Scream volume: " + maxScoreString + "</p><form action=\"/post\" method=\"POST\"><input type=\"submit\" value=\"Post\"></form>");
  } else{
    server.send(200, "text/html", "<p>Scream volume: </p><form action=\"/screaming\" method=\"POST\"><input type=\"submit\" value=\"Start\"></form><form action=\"/post\" method=\"POST\"><input type=\"submit\" value=\"Post\"></form><form action=\"/login\" method=\"POST\"><input type=\"submit\" value=\"Home\"></form>");
  }
}

void handleScreaming(){
  screaming = true;
  server.send(200, "text/html", "<form action=\"/scream\" method=\"POST\"><input type=\"submit\" value=\"Stop\"></form>");
  while(screaming){
    Serial.println("Hello");
    maxScore += 1;
  }
}

void handlePost(){
  
}

void handleLogout(){
  user[0] = "";
  user[1] = "";
  handleRoot();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
