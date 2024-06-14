from flask import Flask, request, jsonify

app = Flask(__name__)

# to store data from POST method
list_data = []

@app.route('/', methods=['POST'])
def sensor_data():
  # Get the data from the request
  data = request.get_json()
  temperature = data['suhu']
  humidity = data['kelembaban']

  # store the sensor data
  list_data.append({'suhu': temperature, 'kelembaban': humidity})

  # Response message
  response = {'message': 'Data received successfully!'}

  # Return the response
  return jsonify(response)

@app.route('/', methods=['GET'])
def get_sensor_data():
  # Return the sensor data
  return jsonify(list_data)


if __name__ == '__main__':
  app.run(debug=True, host='0.0.0.0', port='8080')
