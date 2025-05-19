import logging
from flask import Flask, request, jsonify
from flask_socketio import SocketIO, send, emit
from flask_cors import CORS
from main import wait_read

logging.basicConfig(
    level=logging.INFO,  # Cambia a DEBUG para más verbosidad
    format='[%(asctime)s] %(levelname)s in %(module)s: %(message)s',
    handlers=[
        logging.FileHandler("server.log"),  # Guarda en archivo
        logging.StreamHandler()             # También imprime en consola
    ]
)

logger = logging.getLogger(__name__)  # Logger con el nombre del módulo
app = Flask(__name__)
CORS(app)  # Permitir CORS
socketio = SocketIO(app, cors_allowed_origins="*")  # "*" o usa "http://localhost:5173"



@app.route('/')
def index():
    return "Servidor Flask WebSocket funcionando"


# post data { id: "2432432" }
@app.route('/data', methods=['POST'])
def emit_data():
    data = request.json  # Recibe JSON desde el frontend o cualquier cliente
    logger.info(f"Datos recibidos por POST: {data}")

    # Emitimos los datos a todos los clientes conectados
    socketio.emit('message', data)

    return jsonify({"status": "ok", "mensaje": "Datos enviados al frontend vía WebSocket"})

@app.route('/interact')
def interact():
    card = wait_read()
    
    return jsonify(card)

@app.route('/shutdown', methods=['POST'])
def shutdown():
    socketio.stop()

@socketio.on('message')
def handle_message(msg):
    print(f"Mensaje recibido: {msg}")
    send(f"Echo desde el servidor: {msg}")



if __name__ == '__main__':
    print("Servidor Flask WebSocket iniciado en http://localhost:5000")
    socketio.run(app, host="0.0.0.0", port=5000)
