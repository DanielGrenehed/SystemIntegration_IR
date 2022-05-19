package com.example.demo;

import com.google.gson.Gson;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.socket.TextMessage;

import java.io.IOException;
import java.sql.Timestamp;
import java.time.LocalDateTime;

@RestController
public class TestController {

    @PostMapping("/test")
    IRSensorData testResponse(@RequestBody IRSensorData sensorData) throws IOException {
        if(sensorData.getTimestamp() == null) sensorData.setTimestamp(Timestamp.valueOf(LocalDateTime.now()));
        SensorSocketHandler.broadcastTextMessage(new TextMessage(new Gson().toJson(sensorData)));
        return sensorData;
    }


}
