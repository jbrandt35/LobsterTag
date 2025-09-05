
import struct
import pandas as pd

data = open("/Volumes/NO_NAME/LOG.DAT", "rb").read()

length_of_bytes = 38
i = 0

data_storage = pd.DataFrame(columns = ["temperature_reading", "phototransistor_reading", "IR_sensor_reading", "seconds_now", "accelerometer_x", "accelerometer_y", "accelerometer_z", "magnetometer_x", "magnetometer_y", "magnetometer_z"])

while True:

    try:
        data_point = struct.unpack("<fffHffffff", data[i:i+length_of_bytes])
    except struct.error:
        break

    data_storage.loc[len(data_storage)] = data_point

    i += length_of_bytes

data_storage.to_csv("/Volumes/NO_NAME/LOG.csv", index = False)