package server.websockets;

import server.ir_signal.IRSignal;

import java.io.IOException;
import java.util.LinkedList;
import java.util.Queue;

public class IRSignalDispatcher implements Runnable {

	private static IRSignalDispatcher dispatcher = new IRSignalDispatcher();

	public static IRSignalDispatcher getInstance() {
		return dispatcher;
	}

	private Queue<IRSignal> signal_queue = new LinkedList<>();
	private boolean running = false;
	private IRSignalDispatcher() {}

	public synchronized void enqueueSignal(IRSignal signal) {
		signal_queue.add(signal);
	}

	public synchronized void stop() {
		running = false;
	}

	public boolean isRunning() {
		return running;
	}

	@Override
	public void run() {
		running = true;
		while (running) {
			while (!signal_queue.isEmpty()) {
				IRSignal signal = signal_queue.poll();
				try {
					IRSensorSocketHandler.sendBySensorID(signal);
				} catch (IOException e) {
					throw new RuntimeException(e);
				}
			}
		}

	}
}
