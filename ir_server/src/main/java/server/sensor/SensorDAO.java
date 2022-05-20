package server.sensor;

import server.database.DatabaseAccessObject;
import server.database.Promise;
import server.database.SharedDBAO;
import server.ir_signal.IRSignal;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class SensorDAO {

	private DatabaseAccessObject dbao = SharedDBAO.getInstance();

	private Sensor getSensorFromSet(ResultSet set) throws SQLException {
		Sensor sensor = new Sensor();
		sensor.setId(set.getInt("sensor_id"));
		sensor.setType(set.getString("sensor_type"));
		sensor.setToken(set.getString("sensor_name"));
		return sensor;
	}

	public Sensor getSensorByID(int id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor WHERE sensor_id=?;");
		statement.setInt(1, id);
		Promise result = new Promise();
		dbao.Query(statement, set -> {
			while (set.next()) result.set(getSensorFromSet(set));
		});
		return (Sensor) result.get();
	}

	public Sensor getSensorByToken(String token) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor WHERE sensor_name=?;");
		statement.setString(1, token);
		Promise result = new Promise();
		dbao.Query(statement, set -> {
			while (set.next()) result.set(getSensorFromSet(set));
		});
		return (Sensor) result.get();
	}

	public List<Sensor> getSensors(int sensor_id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor;");
		List<Sensor> result = new ArrayList<>();
		dbao.Query(statement, set -> { while(set.next()) result.add(getSensorFromSet(set)); });
		return result;
	}
}
