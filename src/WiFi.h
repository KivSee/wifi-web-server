//#include <DNSServer.h>

//DNSServer dnsServer;

//const byte DNS_PORT = 53;

bool apMode = false;

// AP mode password
const char WiFiAPPSK[] = "";

// Wi-Fi network to connect to (if not in AP mode)
char* ssid = "";
char* password = "";

#define HOSTNAME "ESP8266-" ///< Hostname. The initializeWiFi function adds the Chip ID at the end.

#define DEBUG_WIFI 1

unsigned long futureTimeout = 0;
uint16_t connectionTimeout = 20000;

template <typename Generic>
void debugPrintln(Generic text) {
  if (DEBUG_WIFI) {
    Serial.print("*WiFi: ");
    Serial.println(text);
  }
}

void startAp() {
  apMode = true;

  WiFi.mode(WIFI_AP_STA);
  debugPrintln("SET AP STA");

  String AP_NameString = "ESP8266-";
  AP_NameString += String(ESP.getChipId(), HEX);

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  debugPrintln("Starting soft AP");

  if (WiFiAPPSK != NULL) {
    debugPrintln(WiFi.softAP(AP_NameChar, WiFiAPPSK) ? "ready" : "failed");
  } else {
    debugPrintln(WiFi.softAP(AP_NameChar) ? "ready" : "failed");
  }

  debugPrintln("Connect to Wi-Fi access point: ");
  debugPrintln(AP_NameChar);

  delay(500); // Without delay I've seen the IP address blank
  debugPrintln("AP IP address: ");
  debugPrintln(WiFi.softAPIP());

  //  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  futureTimeout = millis() + connectionTimeout;
}

String getWiFiJson() {
  String hostname = String(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);

  String json = "{";

  json += "\"status\":\"" + String(WiFi.status()) + "\"";
  json += ",\"localIP\":\"" + WiFi.localIP().toString() + "\"";
  json += ",\"softAPIP\":\"" + WiFi.softAPIP().toString() + "\"";
  json += ",\"hostname\":\"" + hostname + "\"";
  json += ",\"ssid\":\"" + WiFi.SSID() + "\"";
  json += ",\"rssi\":\"" + String(WiFi.RSSI()) + "\"";

  //  json += ",\"networks\":[";
  //  byte ssidCount = WiFi.scanNetworks();
  //  for (byte i = 0; i < ssidCount; i++) {
  //    if (i > 0)
  //      json += ",";
  //
  //    json += "{\"name\":\"" + WiFi.SSID(i) + "\",\"rssi\":\"" + String(WiFi.RSSI(i)) + "\"}";
  //  }
  //
  //  json += "]";

  json += "}";

  return json;
}

void initializeWiFi() {
  WiFi.mode(WIFI_STA);

  // Set Hostname.
  String hostname = String(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);

  // Print hostname.
  Serial.println("Hostname: " + hostname);

  char hostnameChar[hostname.length() + 1];
  memset(hostnameChar, 0, hostname.length() + 1);

  for (uint8_t i = 0; i < hostname.length(); i++)
    hostnameChar[i] = hostname.charAt(i);

  //  MDNS.begin(hostnameChar);

  // Add service to MDNS-SD
  //  MDNS.addService("http", "tcp", 80);

  // attempt to connect; should it fail, fall back to AP mode
  //  WiFi.mode(WIFI_STA);

  String stored_ssid = WiFi.SSID();
  if (stored_ssid != NULL && stored_ssid != "") {
    debugPrintln("Connecting to stored SSID:");
    debugPrintln(stored_ssid);
    futureTimeout = millis() + connectionTimeout;
    WiFi.begin();
  } else {
    debugPrintln("No stored SSID");
    startAp();
  }


  server.on("/wifi", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    // String mode = server.arg("mode");

    char ssidChars[50];
    ssid.toCharArray(ssidChars, 50);

    char passwordChars[50];
    password.toCharArray(passwordChars, 50);

    debugPrintln("Connecting to new SSID:");
    debugPrintln(ssid);

    // dnsServer.stop();
    // WiFi.softAPdisconnect(true);

    server.sendHeader("Location", "/wifi.htm");
    server.send(303);

    while (!WiFi.disconnect());
    apMode = false;
    WiFi.mode(WIFI_STA);
    // WiFi.setAutoReconnect ( false );
    WiFi.persistent(true);
    futureTimeout = millis() + connectionTimeout;
    WiFi.begin(ssidChars, passwordChars);

    // debugPrintln("WE ARE NOT STUCK HERE");
  });

  server.on("/wifi", HTTP_GET, []() {
    String json = getWiFiJson();
    server.send(200, "application/json", json);
  });
}

void checkWiFi() {
    static int cnt = 0;
    static String stored_ssid;
    static String psk;
    // time to give up on the stored network and switch to ap mode?
    if (futureTimeout != 0 && millis() < futureTimeout) {
      //debugPrintln("No Timeout");
      return;
    }

    if (WiFi.status() == WL_CONNECTED) {
     if (WiFi.getMode() == WIFI_AP_STA) {
       debugPrintln("STA connected, shutting down AP and reconnecting as STA only");
       apMode = false;
       char stored_ssidChars[50];
       stored_ssid.toCharArray(stored_ssidChars, 50);
       char pskChars[50];
       psk.toCharArray(pskChars, 50);
       WiFi.mode(WIFI_STA);
       WiFi.begin(stored_ssidChars,pskChars);
       futureTimeout = millis() + connectionTimeout;
       return;
     }
     if (WiFi.getMode() == WIFI_STA) {
       debugPrintln("connected");
       DBG_OUTPUT_PORT.print("Open http://");
       DBG_OUTPUT_PORT.print(host);
       DBG_OUTPUT_PORT.println(".local/edit to see the file browser");
       apMode = false;
       futureTimeout = millis() + connectionTimeout;
       return;
     }
    }

    if (apMode) {
     debugPrintln("Running in AP mode, will try a reconnect every 3rd timeout if no one logs on.");
     cnt++;
     futureTimeout = millis() + connectionTimeout;
     if (WiFi.softAPgetStationNum() == 0 && cnt > 2) {
       cnt = 0;
       if (stored_ssid != NULL && stored_ssid != "") {
         debugPrintln("Connecting to stored SSID:");
         debugPrintln(stored_ssid);
         apMode = false;
         char stored_ssidChars[50];
         stored_ssid.toCharArray(stored_ssidChars, 50);
         char pskChars[50];
         psk.toCharArray(pskChars, 50);
         WiFi.mode(WIFI_STA);
         WiFi.begin(stored_ssidChars,pskChars);
       } else {
         debugPrintln("No stored SSID");
       }
       futureTimeout = millis() + connectionTimeout;
     }
     return;
    }

    debugPrintln("Switching to AP mode, timeout elapsed: ");
    debugPrintln(connectionTimeout);
    // keep the stored ssid and psk before the disconnect erases them so we have them for later
    stored_ssid = WiFi.SSID();
    psk = WiFi.psk();
    WiFi.disconnect();
    startAp();
}
