package server;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import server.database.DatabaseAccessObject;
import server.database.SharedDBAO;
import server.websockets.IRSignalDispatcher;

import java.sql.SQLException;

@SpringBootApplication
public class IrServerApplication {

	public static void main(String[] args) {
		Thread thread = new Thread(IRSignalDispatcher.getInstance());
		thread.start();
		SpringApplication.run(IrServerApplication.class, args);
	}

}
