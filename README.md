# Tracker
 Projeto de rastreador de geolocalização utilizando módulos GPS NEO-6M e o microcontrolador ESP32.

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