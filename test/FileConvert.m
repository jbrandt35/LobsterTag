

 file = fopen('/Users/josh/Desktop/LOG.DAT','r');
 reached_end = false;

 data = [];

 while ~reached_end

     try

     data_point = cell2mat(struct_read(file,'fHHLLffffff'));

     data = vertcat(data, data_point);

     catch error

     if (strcmp(error.identifier,'MATLAB:cell2mat:MixedDataTypes'))

     reached_end = true;

     end

     end

 end

column_titles = {'temperature_reading' 'phototransistor_reading' 'IR_sensor_reading' 'seconds_now' 'milliseconds' 'accelerometer_x' 'accelerometer_y' 'accelerometer_z' 'magnetometer_x' 'magnetometer_y' 'magnetometer_z'};

table = [column_titles; num2cell(data)];

writecell(table, "log.csv")