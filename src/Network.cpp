#include "Network.h"

Network::Network()
{
    ssid = "UPC2703909";
    password = "nxkVF7ksvt8j";
    is_connected = false;
    last_network_call = 0;
    uuid = "";
}

void Network::connect()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("*");
    }

    Serial.println("");
    Serial.println("WiFi connection Successful");
    Serial.print("The IP Address of Arduino is: ");
    Serial.println(WiFi.localIP()); // Print the IP address

    // Once device is connected to the network, get the uuid from the server
    get_name();

    is_connected = true;
}

void Network::get_name()
{

    Serial.println("Device not synced with cloud. Syncing...");

    // get uuid from cloud
    char serverAddress[] = "sensors-data.herokuapp.com"; // server address
    int port = 80;

    WiFiClient wifiClient;
    HttpClient client = HttpClient(wifiClient, serverAddress, port);

    client.beginRequest();
    client.get("/api/board/new");
    client.endRequest();

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    if (statusCode > 0)
    {
        Serial.println("Successfully synced with cloud");
        // print uuid
        Serial.println("uuid: " + response);
        uuid = response;
    }
    else
    {
        Serial.println("Failed to sync with cloud");
        // Serial.println(response);
    }
    client.stop();
}

void Network::run(StaticJsonDocument<256> data)
{
    if (!is_connected)
    {
        connect();
    }
    //  Run loop every second
    if (last_network_call != 0 && millis() - last_network_call < 1000)
        return;

    last_network_call = millis();
    postData(data);
}

void Network::postData(StaticJsonDocument<256> data)
{
    // Making JSON String from the data
    StaticJsonDocument<256> doc;
    doc["id"] = uuid;
    doc["current_temperature"] = data["current_temperature"];
    doc["resistance_status"] = data["resistance_status"];
    doc["pump_status"] = data["pump_status"];
    doc["current_kw"] = data["current_energy"];

    String json;
    serializeJson(doc, json);

    // POSTING TO SERVER
    char serverAddress[] = "sensors-data.herokuapp.com"; // server address
    int port = 80;

    WiFiClient wifiClient;
    HttpClient client = HttpClient(wifiClient, serverAddress, port);

    String contentType = "application/json";

    client.beginRequest();
    client.post("/api/post", contentType, json);
    client.endRequest();

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    if (statusCode > 0)
    {
        // Serial.println("Successfully posted to cloud");
        // Serial.println(response);
    }
    else
    {
        Serial.println("Failed to post to cloud");
        Serial.println(response);
    }

    client.stop();
}