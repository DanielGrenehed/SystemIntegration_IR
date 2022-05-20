package server.ir_signal;

import server.database.DatabaseAccessObject;
import server.database.SharedDBAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class IRSignalDAO {

	DatabaseAccessObject dbao = SharedDBAO.getInstance();

	private IRSignal getFromSet(ResultSet set) throws SQLException {
		IRSignal signal = new IRSignal();
		signal.setSignal(set.getString("sensor_data_value"));
		signal.setTime(set.getTimestamp("sensor_data_date"));
		return signal;
	}

	public List<IRSignal> getSignalsFromSensor(int sensor_id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT sensor_data_value, sensor_data_date FROM sensor_data WHERE sensor_id=?;");
		statement.setInt(1, sensor_id);
		List<IRSignal> result = new ArrayList<>();
		dbao.Query(statement, set -> {
			while(set.next()) result.add(getFromSet(set));
		});
		return result;
	}
}
