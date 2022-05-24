package server;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import server.database.DatabaseAccessObject;
import server.database.DatabaseKeepAlive;
import server.database.SharedDBAO;
import server.websockets.IRSignalDispatcher;

import java.sql.SQLException;

@SpringBootApplication
public class IrServerApplication {

	public static void main(String[] args) {
		Thread dispatcher_thread = new Thread(IRSignalDispatcher.getInstance());
		dispatcher_thread.start();
		Thread keep_alive_thread = new Thread(new DatabaseKeepAlive());
		keep_alive_thread.start();
		SpringApplication.run(IrServerApplication.class, args);
	}

}
