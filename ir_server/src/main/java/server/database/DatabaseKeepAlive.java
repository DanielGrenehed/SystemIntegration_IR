package server.database;

import java.sql.SQLException;

public class DatabaseKeepAlive implements Runnable {

	private DatabaseAccessObject dbao = SharedDBAO.getInstance();

	private static final long sleep_time_ms = 1000 * 60 * 60; // sleep for one hour
	private boolean running = false;

	public void stopRunning() {
		running = false;
	}

	public boolean isRunning() {
		return running;
	}

	@Override
	public void run() {
		running = true;
		while (running) {
			try {
				dbao.Query("SELECT * FROM sensor;", set -> {});
				Thread.sleep(sleep_time_ms);
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (SQLException e) {
				throw new RuntimeException(e);
			}
		}

	}
}
