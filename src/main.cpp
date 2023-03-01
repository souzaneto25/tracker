#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RXD2 16                            // RXD2 do ESP32 conectado ao TX do GPS
#define TXD2 17                            // TXD2 do ESP32 conectado ao RX do GPS
#define GPS_BAUDRATE 9600                  // Baud rate do GPS
#define WIFI_SSID "PRUU"                   // SSID da rede WiFi
#define WIFI_PASSWORD "gustavoejulia"      // Senha da rede WiFi
#define MQTT_SERVER "broker.emqx.io"       // Endereço do servidor MQTT
#define MQTT_PORT 1883                     // Porta do servidor MQTT
#define MQTT_TOPIC "esp32/gps/coordinates" // Tópico MQTT para publicar as coordenadas
#define MQTT_USERNAME "emqx"               // Usuario do servidor
#define MQTT_PASSWORD "public"             // Senha do usuario
#define LED 2                              // Led interno do esp32
#define LED_EXT 15                         // Led externo ao esp32

SoftwareSerial ss(RXD2, TXD2);        // Define a serial para comunicação com o GPS
TinyGPSPlus gps;                      // Objeto para fazer o parsing dos dados do GPS
WiFiClient wifiClient;                // Cliente WiFi
PubSubClient client(wifiClient);      // Cliente MQTT
const int json = JSON_OBJECT_SIZE(6); // Tamanho do objeto
StaticJsonDocument<json> doc;         // Criação do documento (objeto que vai ser publicado)

// Função responsável por conectar ao Wifi
void setup_wifi()
{
  delay(1000);

  Serial.println();
  Serial.print("Conectando ao wifi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Enquanto nao for estabelecida, fica aguardadando
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  // Conexao com wifi estabelecida. Aciona o led interno
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED, HIGH);
}

// Função de inicialização, após conectar ao wifi inicializa o servidor
void setup()
{
  Serial.begin(115200);
  ss.begin(GPS_BAUDRATE);
  pinMode(LED, OUTPUT);
  pinMode(LED_EXT, OUTPUT);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop()
{
  // Lê os dados do GPS e aguarda até ter uma leitura válida
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      break;
    }
  }

  Serial.println("Satellites is valid " + String(gps.satellites.isValid()));
  Serial.println("Values " + String(gps.satellites.value()));
  Serial.println("{Lat: " + String(gps.location.lat(), 6) + "," + " Long: " + String(gps.location.lng(), 6) + "}");

  // Se os dados do GPS estiverem disponíveis e válidos, envia as coordenadas para o servidor MQTT
  if (gps.location.isValid())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    doc["device"] = "esp32";
    doc["sensor"] = client_id;
    doc["coordinates"]["lat"] = String(gps.location.lat(), 6);
    doc["coordinates"]["lon"] = String(gps.location.lng(), 6);

    char buffer[256];
    size_t n = serializeJson(doc, buffer);

    digitalWrite(LED_EXT, HIGH);
    client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
    client.publish(MQTT_TOPIC, buffer, n);
    client.disconnect();
  }
  else
  {
    Serial.println("Esperando GPS...");
    digitalWrite(LED_EXT, LOW);
  }
  delay(5000); // Aguarda 5 segundos antes de enviar a próxima mensagem
}