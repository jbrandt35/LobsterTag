
import struct
import pandas as pd

data = open("/Volumes/NO_NAME/LOG.DAT", "rb").read()

struct_format = "<fHHHLffffff"

length_of_struct = struct.calcsize(struct_format)

data_storage = pd.DataFrame(columns = ["temperature_reading", "phototransistor_reading", "IR_sensor_reading", "seconds_now", "milliseconds", "accelerometer_x", "accelerometer_y", "accelerometer_z", "magnetometer_x", "magnetometer_y", "magnetometer_z"])

i = 0

while i < 1000000:

    try:
        data_point = struct.unpack(struct_format, data[i : i + length_of_struct])
    except struct.error:
        break

    data_storage.loc[len(data_storage)] = data_point

    i += length_of_struct

data_storage.to_csv("/Volumes/NO_NAME/LOG.csv", index = False)