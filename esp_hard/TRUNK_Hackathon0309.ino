#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h> // HTTPClient
#include <NTP.h>

const char* ssid     = "sakura-guest";
const char* password = "2D8uWfEqMp";

const char* url = "http://aonr.jp/flask/add"; //"http://192.168.166.209:5000/add";
const char* target_point = "sakura-guest";

const char* myuuid = "3E50517B-D197-4F11-8CC6-E79E2A6E35B9";

bool isFirstConnection = true;

void setup() {
  Serial.begin(115200);

  disconnectWiFi();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  scanWiFiSpot();

  // Wait a bit before scanning again
  delay(5000);
}

void scanWiFiSpot() {
  Serial.println("scan start");

  int n = WiFi.scanNetworks();
  Serial.println("scan done");

  int sorted_count[n];
  int sorted_rssi[n];
  int active_points[n];
  int active_count = 0;

  if (n == 0) {
    Serial.println("no networks found");
    return;
  }
  else
  {
    for (int i = 0; i < n; i++) {
      sorted_count[i] = i;
      sorted_rssi[i] = WiFi.RSSI(i) * -1;
    }

    for (int i = 0; i < n; i++) {
      for (int j = n - 1; j > i; j--) {
        if (sorted_rssi[j] < sorted_rssi[j - 1]) {
          // replace count
          int temp = sorted_count[j];
          sorted_count[j] = sorted_count[j - 1];
          sorted_count[j - 1] = temp;
          // replace rssi value
          temp = sorted_rssi[j];
          sorted_rssi[j] = sorted_rssi[j - 1];
          sorted_rssi[j - 1] = temp;
        }
      }
    }

    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print("[");
      Serial.print(i + 1);
      Serial.print("]\t");
      Serial.print(sorted_rssi[i]);
      Serial.print("\t(");
      Serial.print(WiFi.BSSIDstr(sorted_count[i]));
      Serial.print(") ");
      Serial.print(WiFi.SSID(sorted_count[i]));
      Serial.println((WiFi.encryptionType(sorted_count[i]) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
      if (WiFi.SSID(sorted_count[i]) == target_point)
      {
        active_points[active_count] = i;
        active_count++;
      }
    }
  }
  Serial.println("");

  Serial.print("- near point list [");
  Serial.print(target_point);
  Serial.println("] -");
  for (int i = 0; i < active_count; i++) {
    Serial.print("[");
    Serial.print(i + 1);
    Serial.print("] ");
    Serial.print(WiFi.BSSIDstr(sorted_count[active_points[i]]));
    Serial.print(" @");
    Serial.print(sorted_rssi[active_points[i]]);
    Serial.println("");
  }
  Serial.print("");

  connectWiFi();

  Serial.print("[ Requesting POST ] ");
  // Send request to the server:
  HTTPClient http;
  http.begin(url);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // JST
  time_t now_time = now();
  time_t t;
  char s[20];
  const char* format = "%04d-%02d-%02d %02d:%02d:%02d";
  t = localtime(now_time, 9);
  sprintf(s, format, year(t), month(t), day(t), hour(t), minute(t), second(t));

  // POST パラメータ作る
  String data;

  if (active_count >= 3) {
    data = "uuid=" + String(myuuid) +
           "&bssid_one=" + String(WiFi.BSSIDstr(sorted_count[active_points[0]])) +
           "&rssi_one=" + String(sorted_rssi[active_points[0]]) +
           "&bssid_two=" + String(WiFi.BSSIDstr(sorted_count[active_points[1]])) +
           "&rssi_two=" + String(sorted_rssi[active_points[1]]) +
           "&bssid_three=" + String(WiFi.BSSIDstr(sorted_count[active_points[2]]))  +
           "&rssi_three=" + String(sorted_rssi[active_points[2]]) +
           "&time=" + String(s);
  }
  else if (active_count == 2) {
    data = "uuid=" + String(myuuid) +
           "&bssid_one=" + String(WiFi.BSSIDstr(sorted_count[active_points[0]])) +
           "&rssi_one=" + String(sorted_rssi[active_points[0]]) +
           "&bssid_two=" + String(WiFi.BSSIDstr(sorted_count[active_points[1]])) +
           "&rssi_two=" + String(sorted_rssi[active_points[1]]) +
           "&bssid_three=NaN"
           "&rssi_three=NaN"
           "&time=" + String(s);
  }
  else {
    data = "uuid=" + String(myuuid) +
           "&bssid_one=" + String(WiFi.BSSIDstr(sorted_count[active_points[0]])) +
           "&rssi_one=" + String(sorted_rssi[active_points[0]]) +
           "&bssid_two=NaN"
           "&rssi_two=NaN"
           "&bssid_three=NaN"
           "&rssi_three=NaN"
           "&time=" + String(s);
  }
  Serial.println(data);
  // POST リクエストする
  int httpCode = http.POST(data);

  if (httpCode > 0) {
    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
  } else {
    Serial.println("[HTTPS] no connection or no HTTP server.");
  }

  http.end();

  disconnectWiFi();
}

void connectWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // ntp
  if (isFirstConnection) {
    ntp_begin(2390);
    isFirstConnection = false;
  }
}

void disconnectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

