package server.websockets;

import com.google.gson.Gson;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

@Component
public class IRSensorSocketHandler extends TextWebSocketHandler {

    static List<WebSocketSession> sessions = new CopyOnWriteArrayList<>();
    static Map<WebSocketSession, SessionSensorId> sessionMap = new HashMap<>();

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        sessions.add(session);
        //Send current data stored in database;
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        sessions.remove(session);

    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message)
            throws InterruptedException, IOException {
            //messages from websocket
            SessionSensorId sensorId = new Gson().fromJson(message.getPayload(), SessionSensorId.class);
            sessionMap.put(session, sensorId);

    }

    public static void broadcastTextMessage(TextMessage message) throws IOException {
        for(WebSocketSession webSocketSession : sessions) {
            webSocketSession.sendMessage(message);
        }
    }

    public static void sendBySensorID(TextMessage message, int id) throws IOException {

        for(Map.Entry<WebSocketSession, SessionSensorId> entry : sessionMap.entrySet()){
            if(entry.getValue().getSensorId() == id){
                entry.getKey().sendMessage(message);
            }
        }
    }



}
