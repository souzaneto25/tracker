# Tracker
 Projeto de rastreador de geolocalização utilizando módulos GPS NEO-6M e o microcontrolador ESP32.

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

# 1. Rotas

| CONSTANTES    | Descrição                                                                             |
| ------------- | ------------------------------------------------------------------------------------- |
| RXD2          | Pino RXD2 do ESP32. Obs: deve ser conectado ao TX do GPS                              |
| TXD2          | Pino TXD2 do ESP32. Obs: deve ser conectado ao RX do GPS                              |
| GPS_BAUDRATE  | Baud rate do GPS. Obs: Geralmente é de 9600                                           |
| WIFI_SSID     | SSID da rede WiFi                                                                     |
| WIFI_PASSWORD | Senha da rede WiFi                                                                    |
| MQTT_SERVER   | Endereço do servidor MQTT                                                             |
| MQTT_PORT     | Porta do servidor MQTT                                                                |
| MQTT_TOPIC    | Tópico MQTT para publicar as coordenadas                                              |
| MQTT_USERNAME | Usuario do servidor                                                                   |
| MQTT_PASSWORD | Senha do usuario do servidor                                                          |
| LED           | Pino referente ao LED interno do esp32                                                |
| LED_EXT       | Pino referente ao LED externo ao esp32 que será utilizado. Obs: verificar o datasheet |
| DELAY         | Delay em milisegundos (ms) para envio de dados. Ex: 5*1000 = 5 segundos               |