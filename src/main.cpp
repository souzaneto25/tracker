#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RXD2 16                      // RXD2 do ESP32 conectado ao TX do GPS
#define TXD2 17                      // TXD2 do ESP32 conectado ao RX do GPS
#define GPS_BAUDRATE 9600            // Baud rate do GPS
#define WIFI_SSID "PRUU"             // SSID da rede WiFi
#define WIFI_PASSWORD "joaoetheo"    // Senha da rede WiFi
#define MQTT_SERVER "broker.emqx.io" // Endereço do servidor MQTT
#define MQTT_PORT 1883               // Porta do servidor MQTT
#define MQTT_TOPIC "data/location"   // Tópico MQTT para publicar as coordenadas
#define MQTT_USERNAME "emqx"         // Usuario do servidor
#define MQTT_PASSWORD "public"       // Senha do usuario
#define LED 2                        // Led interno do esp32
#define LED_EXT 15                   // Led externo ao esp32
#define DELAY 5000                   // Delay em ms para envio de dados. Ex:60*1000=5seg

SoftwareSerial ss(RXD2, TXD2);         // Define a serial para comunicação com o GPS
TinyGPSPlus gps;                       // Objeto para fazer o parsing dos dados do GPS
WiFiClient wifiClient;                 // Cliente WiFi
PubSubClient client(wifiClient);       // Cliente MQTT
const int json = JSON_OBJECT_SIZE(10); // Tamanho do objeto
StaticJsonDocument<json> doc;          // Criação do documento (objeto que vai ser publicado)
size_t freeHeap = ESP.getFreeHeap();   // Quantidade de memória livre disponível na heap, que é a área de memória dinâmica usada pelo programa durante a execução

DynamicJsonDocument doc2((freeHeap / 2)); // Criação de um documento com tamanho máximo igual a metade da memoria disponivel no ESP32
JsonArray array = doc2.to<JsonArray>(); // Criação de um array de documentos

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

//Delay customizado com verificação do gps
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
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
  Serial.print(freeHeap);
}

void loop()
{
  // Se os dados do GPS não estiverem disponíveis e válidos
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS data received: check wiring"));
    Serial.println("Esperando GPS...");
    digitalWrite(LED_EXT, LOW);
  }
  else
  {

    String client_id = "esp32-";
    client_id += String(WiFi.macAddress());
    String topic = String(MQTT_TOPIC) + "/" + client_id;
    client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD);

    // se o cliente estiver conectado publica o
    if (client.connected() && gps.location.isValid() && gps.hdop.isValid() && gps.satellites.isValid())
    {
      // se ja tiver dados armazenados, envia
      String array_armazenado;
      serializeJson(doc2, array_armazenado);
      if (array_armazenado.length() > 2)
      {
        Serial.println("_______________Old array location_________________");
        Serial.print("array_armazenado length: ");
        Serial.println(array_armazenado);
        client.publish(topic.c_str(), array_armazenado.c_str());
        array = doc2.to<JsonArray>();
        Serial.println("__________________________________________________");
      }
      Serial.println("...................New location....................");
      String new_location;
      doc.clear();
      doc["device"] = "esp32";
      doc["sensor"] = client_id;
      doc["coordinates"]["lat"] = String(gps.location.lat(), 6);
      doc["coordinates"]["lon"] = String(gps.location.lng(), 6);
      serializeJson(doc, new_location);
      digitalWrite(LED_EXT, HIGH);

      client.publish(topic.c_str(), new_location.c_str());

      delay(1000);
      digitalWrite(LED_EXT, LOW);
      Serial.println("Topic " + topic);
      Serial.println("Sats " + String(gps.satellites.value()));
      Serial.println("HDOP " + String(gps.hdop.hdop()));
      Serial.println(new_location);
      Serial.println("...................................................");

      client.disconnect();
    }
    else
    {
      Serial.println("...............Client disconnected.................");
      if (gps.location.isValid() && gps.hdop.isValid() && gps.satellites.isValid())
      {
        Serial.println("_______________New location saved_________________");
        JsonObject obj = array.createNestedObject();
        obj["device"] = "esp32";
        obj["sensor"] = client_id;
        obj["coordinates"]["lat"] = String(gps.location.lat(), 6);
        obj["coordinates"]["lon"] = String(gps.location.lng(), 6);
        Serial.println("{device:esp32, sensor:" + client_id + ", lat:" + String(gps.location.lat(), 6) + ", lon:" + String(gps.location.lng(), 6) + "}");
        String output;
        serializeJson(doc2, output);
        Serial.print("Output length: ");
        Serial.println(output.length());
        Serial.println("__________________________________________________");
      }
      else
      {
        Serial.println("..................Searching sats...................");
        Serial.println("{device:esp32, sensor:" + client_id + ", lat:" + String(gps.location.lat(), 6) + ", lon:" + String(gps.location.lng(), 6) + "}");
      }
      Serial.println("...................................................");
    }
  }
  smartDelay(DELAY);
}
