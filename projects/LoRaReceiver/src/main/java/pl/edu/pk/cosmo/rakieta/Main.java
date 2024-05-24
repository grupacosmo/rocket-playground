package pl.edu.pk.cosmo.rakieta;


import com.fasterxml.jackson.databind.ObjectMapper;
import com.fazecast.jSerialComm.SerialPort;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import pl.edu.pk.cosmo.rakieta.entity.SensorPacket;
import pl.edu.pk.cosmo.rakieta.service.EspRead;
import pl.edu.pk.cosmo.rakieta.service.FireBaseService;

import java.io.IOException;

public class Main {


    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        FireBaseService fireBaseService = new FireBaseService();
        FirebaseDatabase database = fireBaseService.getDb();
        EspRead data = new EspRead();
        SerialPort serialPort = data.choosePort();
        DatabaseReference ref = database.getReference("LoRa");
        while (true) {
            SensorPacket sensorPacket = data.readdata(serialPort);
            try {
                String dataLoRaJson = objectMapper.writeValueAsString(sensorPacket);
                ref.setValue(dataLoRaJson, (databaseError, databaseReference) -> {
                    if (databaseError != null) {
                        System.out.println("Data could not be saved. " + databaseError.getMessage());
                    } else {
                        System.out.println("Data saved successfully.");
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            } //ToDo napraw, tylko jak ejst w pętli while to się zapisuje
        }
    }
}

