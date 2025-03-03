// Configuration du broker MQTT
const brokerUrl = 'mqtt://'+API_URL;
const options = {
    clientId: `webClient_${Math.random().toString(16).substr(2, 8)}`
};

// Connexion au broker
const client = mqtt.connect(brokerUrl, options);

// Gestion des événements de connexion
client.on('connect', () => {
    document.getElementById('connexion').innerHTML='Connecté au broker MQTT';

    [MQTT_TOPIC_TEMPERATURE , MQTT_TOPIC_HUMIDITE].forEach(topic => {
        client.subscribe(topic, { qos: 0 }, err => {
            if (err) {
                console.log(`Erreur lors de l'abonnement au topic ${topic} :`, err);
            } else {
                console.log(`Abonné au topic : ${topic.split('/')[1]}`);
            }
        });
    });
});

// Gestion des messages reçus
client.on('message', (topic, message) => {
    if(topic == MQTT_TOPIC_TEMPERATURE ){
        updateValuesTemperature(message)
    }else{
        updateValuesHumidity(message)
    }
});

// Gestion des erreurs
client.on('error', err => document.getElementById('connexion').innerHTML='Erreur MQTT ');

// Gestion de la déconnexion
client.on('close', () => document.getElementById('connexion').innerHTML='Déconnecté du broker MQTT');
function updateValuesTemperature(t) {
    var data = parseFloat(t.toString())
    console.log(data)
    let hh = (data+40) * 100 / 120 
    document.getElementById("temperature").style.height = hh + "%";
    document.getElementById("tempValue").innerText = "Température: " + data + "°C"
    if (data > 30) { 
        envoyerNotification("Alerte Température", `Température élevée : ${data                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      }°C`);
    }
}
function updateValuesHumidity(h) {
    let humidity = parseFloat(h.toString());
    console.log(humidity)
    document.getElementById("humidity").style.width = humidity + "%";
    document.getElementById("humidityValue").innerText = "Humidité: " + humidity + "%";
}
function demanderPermissionNotifications() {
    if ("Notification" in window) {
        Notification.requestPermission().then(permission => {
            if (permission === "granted") {
                console.log("Permission de notification accordée !");
            } else {
                console.log("Permission de notification refusée.");
            }
        });
    } else {
        console.log("Les notifications ne sont pas prises en charge par ce navigateur.");
    }
}

document.addEventListener("DOMContentLoaded", demanderPermissionNotifications);
function envoyerNotification(title, message) {
    if (Notification.permission === "granted") {
        new Notification(title, {
            body: message,
        });
    }
}


