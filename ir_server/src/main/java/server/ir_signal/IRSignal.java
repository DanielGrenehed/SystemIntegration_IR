package server.ir_signal;

import java.util.Date;

public class IRSignal {
	private String signal;
	private int sensor_id;
	private Date time;
	public int getSensor_id() {
		return sensor_id;
	}

	public void setSensor_id(int sensor_id) {
		this.sensor_id = sensor_id;
	}

	public String getSignal() {
		return signal;
	}

	public void setSignal(String signal) {
		this.signal = signal;
	}

	public Date getTime() {
		return time;
	}

	public void setTime(Date time) {
		this.time = time;
	}
}
