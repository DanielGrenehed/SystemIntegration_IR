package server.websockets;

import com.google.gson.Gson;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import server.ir_signal.IRSignal;

import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

@Component
public class IRSensorSocketHandler extends TextWebSocketHandler {

    static List<WebSocketSession> sessions = new CopyOnWriteArrayList<>();
    static Map<WebSocketSession, SessionSensorId> session_sid_map = new ConcurrentHashMap<>();

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        sessions.add(session);
        //Send current data stored in database;
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        sessions.remove(session);
        session_sid_map.remove(session);
        System.out.println("Session Closed");
    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message)
            throws InterruptedException, IOException {
            //messages from websocket
            SessionSensorId sensorId = new Gson().fromJson(message.getPayload(), SessionSensorId.class);
            session_sid_map.put(session, sensorId);

    }

    public static void broadcastTextMessage(TextMessage message) throws IOException {
        for(WebSocketSession webSocketSession : sessions) {
            webSocketSession.sendMessage(message);
        }
    }


    public static void sendBySensorID(IRSignal signal) throws IOException {
        TextMessage message = new TextMessage(new Gson().toJson(signal));
        for(Map.Entry<WebSocketSession, SessionSensorId> entry : session_sid_map.entrySet()){
            if(entry.getValue().getSensor_id() == signal.getSensor_id()){
                entry.getKey().sendMessage(message);
                //System.out.println("Sending signal to websocket");
            }
        }
    }



}
