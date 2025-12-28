from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # allow web access from localhost website

latest_data = {"lat": 0, "lon": 0, "msg": "SAFE_UPDATE"}

@app.route('/update', methods=['GET'])
def update_data():
    lat = request.args.get('lat', type=float)
    lon = request.args.get('lon', type=float)
    msg = request.args.get('msg', default="SAFE_UPDATE", type=str)
    
    global latest_data
    latest_data = {"lat": lat, "lon": lon, "msg": msg}
    print(f"📡 Received -> Lat: {lat}, Lon: {lon}, Msg: {msg}")
    
    return "Data received", 200

@app.route('/get_data', methods=['GET'])
def get_data():
    return jsonify(latest_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
