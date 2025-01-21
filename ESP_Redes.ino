#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define pinLED 11
#define pinPOT 13
#define pinBUTTON 12

// Configuração Wi-fi
const char* ssid = "2.4G";
const char* password = "mieG3201";

// Configuração MQTT
const char* Server_MQTT = "6445f45591134ef191723c599d033f14.s1.eu.hivemq.cloud";
const int Port_MQTT = 8883;
const char* Username_MQTT = "admin";
const char* Password_MQTT = "Admin123";

// Topicos utilizados
const char* LED = "LED";
const char* POT = "POT";
const char* BUTTON = "BUTTON";

// Certificado
static const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void connect_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Tentando conexão com ");
  Serial.print(ssid);
  Serial.print("..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void connect_mqtt() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32Client", Username_MQTT, Password_MQTT)) { // Tenta inserir as credenciais
      Serial.println("Conectado com sucesso! ");
      client.subscribe(LED);
      client.subscribe(POT);
      client.subscribe(BUTTON);
    } else {
      Serial.print("FALHA, rc=");
      Serial.print(client.state());
      Serial.println("Nova tentativa em 3s");
      delay(3000);
    }
  }
}

//Funções para publicar e assinar
void callback(char* topico, byte* Message, unsigned int length) {
  // Tópico LED
  if (strcmp(topico, LED) == 0) {
    int estado = Message[0] - '0';
    digitalWrite(pinLED, estado);
    if(estado)
      Serial.println("Led Ligado!");
    else if(estado == 0)
      Serial.println("Led Desligado!");
  }
  // Tópico POT 
  else if (strcmp(topico, POT) == 0) {
    int leituraPOT = analogRead(pinPOT);
    client.publish(POT, String(leituraPOT).c_str(), true);
    Serial.print("Leitura analógica: ");
    Serial.println(leituraPOT);
    delay(500);
  }
  // Tópico BUTTON
  else if (strcmp(topico, BUTTON) == 0) {
    int leituraButton = digitalRead(pinBUTTON);
    if(leituraButton == HIGH){
      client.publish(BUTTON, "Aberto", true);
      Serial.println("Aberto");
    }else if(leituraButton == LOW){
      client.publish(BUTTON, "Fechado", true);
      Serial.println("Fechado");
    }
    delay(500);
  }
}

void setup() {
  // Entradas e Saídas do ESP
  pinMode(pinLED, OUTPUT);
  pinMode(pinPOT, INPUT);
  pinMode(pinBUTTON, INPUT);
  //Monitor Serial
  Serial.begin(9600);
  //Wi-fi
  connect_wifi();
  espClient.setCACert(root_ca);
  //MQTT Server
  client.setServer(Server_MQTT, Port_MQTT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) { // Credenciais MQTT
    connect_mqtt();
  }
  client.loop();
}


