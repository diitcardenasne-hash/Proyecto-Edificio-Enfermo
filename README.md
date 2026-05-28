# Proyecto-Edificio-Enfermo
🚀 Sistema de Monitoreo Ambiental LIAC-CA
Este proyecto consiste en una red de sensores (Nodos ESP32) que miden variables ambientales y las envían mediante el protocolo MQTT a una Raspberry Pi, la cual almacena la información en una base de datos MariaDB y permite la consulta selectiva mediante un sistema de usuarios.

-Protocolo de Comunicación: MQTT.
-Base de Datos: MariaDB (SQL).
-Hardware: ESP32, Sensores MQ, DHT11/22.
-Backend: Python 3.


🛠️ Requisitos e Instalación
1. En los Nodos (ESP32 / Arduino IDE)
Para compilar los códigos .ino, es necesario instalar las siguientes librerías desde el Gestor de Librerías de Arduino:

        •PubSubClient: Para la conexión MQTT.
        •DHT sensor library: Para lectura de temperatura y humedad.
        •ArduinoJson: Para empaquetar los datos antes del envío.

2. En el Servidor (Raspberry Pi)
Es necesario tener instalado Mosquitto Broker y Python 3. Las dependencias de Python se instalan con:
        •pip3 install paho-mqtt mysql-connector-python

3. Configuración de la Base de Datos
El sistema utiliza una base de datos llamada LIAC_CA con dos tablas principales:

        •Datos: Almacena lecturas de nodo, temperatura, humedad y ppm.
        •Usuarios: Almacena usuario y password para el control de acceso.    

📂 Estructura del Repositorio
    /Arduino: Códigos .ino para los Nodos 1 y 2.
    /Python: 
        •procesador_nodos.py: El "bridge" que recibe datos MQTT y los guarda en SQL.
        •consulta_usuarios.py: Interfaz de terminal para consulta de datos por roles.
    /Config: Archivos de configuración del Broker Mosquitto.
    /Diagramas: Esquemas de conexión y pines del circuito.



🔐 Sistema de Roles y Privilegios
El acceso mediante la terminal está restringido por un sistema de autenticación que define qué información puede visualizar cada perfil:

    🔐Usuario: Operador
    Contraseña: 1111
    Función: Visualización de datos en tiempo real. Muestra las lecturas más recientes de temperatura, humedad y PPM de cada nodo conectado para monitoreo inmediato.

    🔐Usuario: Docente
    Contraseña: 2222
    Función: Análisis de condiciones ambientales. Calcula el promedio de las últimas horas y determina el estado del edificio (Ventilación OK / Necesario Ventilar) basándose en los niveles de CO2 (PPM) detectados.

    🔐Usuario: Administrador
    Contraseña: 3333
    Función: Reporte de valores críticos. Identifica y muestra los picos máximos (valores récord) de temperatura y PPM registrados en las últimas 24 horas para la gestión de seguridad ambiental.    


📝 Instrucciones de Uso (SSH: sick_build) (cd progs)
1.-Asegurarse de que el servicio de Mosquitto esté corriendo en la Raspberry Pi.
2.-Ejecutar el script procesador para comenzar a recibir datos:
python3 procesador_nodos.py

3.-Para consultar la información, ejecutar el sistema de login:
python3 consulta_usuarios.py