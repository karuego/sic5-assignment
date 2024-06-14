#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>

#define WIFI_SSID "C42"
#define WIFI_PASSWORD "0806040200"
#define WIFI_CHANNEL 6

#define SERVER_URL = "https://0cd5w308ynnj.share.zrok.io";

#define DHTPIN	4
#define DHTTYPE	DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menunggu koneksi ke WiFi...");
  }
}

void loop() {
  // Membaca kelembapan dan suhu
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Jika koneksi WiFi terputus, coba koneksi ulang
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Menunggu koneksi ke WiFi...");
    }
  }

  // Mengirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    
    // Membuat JSON untuk dikirim
    String httpRequestData =
	"{'temperature':'" + String(temperature)
        + "', 'humidity':'" + String(humidity) + "'}";
    
    int httpResponseCode = http.POST(httpRequestData);

    // Cetak kode respons HTTP
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error pada kode: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(5000);
}
