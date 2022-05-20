package server.ir_signal;

import server.database.DatabaseAccessObject;
import server.database.Promise;
import server.database.SharedDBAO;
import server.websockets.IRSignalDispatcher;

import java.sql.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class IRSignalDAO {

	DatabaseAccessObject dbao = SharedDBAO.getInstance();

	private IRSignal getIRSignalFromSet(ResultSet set) throws SQLException {
		IRSignal signal = new IRSignal();
		signal.setSensor_id(set.getInt("sensor_id"));
		signal.setSignal(set.getString("sensor_data_value"));
		signal.setTime(set.getTimestamp("sensor_data_date"));
		return signal;
	}

	public List<IRSignal> getSignalsFromSensor(int sensor_id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor_data WHERE sensor_id=? ORDER BY sensor_data_date DESC;");
		statement.setInt(1, sensor_id);
		List<IRSignal> result = new ArrayList<>();
		dbao.Query(statement, set -> { while(set.next()) result.add(getIRSignalFromSet(set)); });
		return result;
	}

	public IRSignal getLatestSignalFromSensor(int sensor_id) throws SQLException {
		PreparedStatement statement = dbao.prepareStatement("SELECT * FROM sensor_data WHERE sensor_id=? ORDER BY sensor_data_date DESC LIMIT 1;");
		statement.setInt(1, sensor_id);
		Promise result = new Promise();
		dbao.Query(statement, set -> {
			while (set.next()) result.set(getIRSignalFromSet(set));
		});
		return (IRSignal) result.get();
	}

	public IRSignal insertSignal(IRSignalInsert insert) throws SQLException {
		PreparedStatement statement = dbao.prepareUpdateStatement("INSERT INTO sensor_data (sensor_id, sensor_data_value, sensor_data_date) values ((SELECT sensor_id FROM sensor WHERE sensor_name=?), ?, ?);");
		statement.setString(1, insert.getToken());
		statement.setString(2, insert.getCode());
		if (insert.getTime() == null) insert.setTime(new Date());
		statement.setTimestamp(3, new Timestamp(insert.getTime().getTime()));

		Promise result = new Promise();
		dbao.QueryUpdate(statement, ((affected, set) -> {
			if (affected > 0){
				ResultSetMetaData metadata = set.getMetaData();
				int columnCount = metadata.getColumnCount();
				//System.out.println("Column count:" + String.valueOf(columnCount));
				while (set.next()) {
					//System.out.println(metadata.getColumnName(columnCount) + ": " + set.getString(columnCount));
					PreparedStatement stmt = dbao.prepareStatement("SELECT * FROM sensor_data WHERE sensor_data_id=?;");
					stmt.setInt(1, set.getInt(columnCount));
					dbao.Query(stmt, set1 -> {
						while(set1.next()) result.set(getIRSignalFromSet(set1));
					});
				}}
		}));
		if (result.isSet()) IRSignalDispatcher.getInstance().enqueueSignal((IRSignal) result.get());
		return (IRSignal) result.get();
	}

}
