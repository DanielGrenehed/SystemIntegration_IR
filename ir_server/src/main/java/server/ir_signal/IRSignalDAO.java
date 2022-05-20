package server.ir_signal;

import server.database.DatabaseAccessObject;
import server.database.Promise;
import server.database.SharedDBAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

public class IRSignalDAO {

	DatabaseAccessObject dbao = SharedDBAO.getInstance();

	private IRSignal getIRSignalFromSet(ResultSet set) throws SQLException {
		IRSignal signal = new IRSignal();
		signal.setSignal(set.getString("sensor_data_value"));
		signal.setTime(set.getTimestamp("sensor_data_date"));
		signal.setSensor_id(set.getInt("sensor_id"));
		return signal;
	}

	public List<IRSignal> getSignalsFromSensor(int sensor_id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor_data WHERE sensor_id=?;");
		statement.setInt(1, sensor_id);
		List<IRSignal> result = new ArrayList<>();
		dbao.Query(statement, set -> { while(set.next()) result.add(getIRSignalFromSet(set)); });
		return result;
	}

	public IRSignal insertSignal(IRSignalInsert insert) throws SQLException {
		PreparedStatement statement = dbao.prepareUpdateStatement("INSERT INTO sensor_data (sensor_id, sensor_data_value, sensor_data_date) values ((SELECT sensor_id FROM sensor WHERE sensor_name=?), ?, ?);");
		statement.setString(1, insert.getToken());
		statement.setString(2, insert.getSignal());
		statement.setTimestamp(3, (Timestamp) insert.getTime());
		Promise result = new Promise();
		dbao.QueryUpdate(statement, ((affected, set) -> {
			if (affected > 0)
			while (set.next()) result.set(getIRSignalFromSet(set));
		}));
		return (IRSignal) result.get();
	}

}
