package com.example.demo;

import com.google.gson.Gson;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.io.IOException;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

@Component
public class SensorSocketHandler extends TextWebSocketHandler {

    static List<WebSocketSession> sessions = new CopyOnWriteArrayList<>();

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        sessions.add(session);
        List<IRTestData> testDataList = new ArrayList<>();
        testDataList.add(new IRTestData("abc","abc",Timestamp.valueOf(LocalDateTime.now())));
        testDataList.add(new IRTestData("aaa","aaa",Timestamp.valueOf(LocalDateTime.now())));
        testDataList.add(new IRTestData("bbb","bbb",Timestamp.valueOf(LocalDateTime.now())));

        session.sendMessage(new TextMessage(new Gson().toJson(testDataList)));
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception{
        sessions.remove(session);

    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message)
            throws InterruptedException, IOException {
        Map value = new Gson().fromJson(message.getPayload(), Map.class);

        broadcastTextMessage(new TextMessage("Message from sensor" + value.get("sensorMessage")));
    }

    public static void broadcastTextMessage(TextMessage message) throws IOException {
        for(WebSocketSession webSocketSession : sessions) {
            webSocketSession.sendMessage(message);
        }
    }

}
