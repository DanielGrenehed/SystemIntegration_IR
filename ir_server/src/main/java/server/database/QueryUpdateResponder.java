package server.database;

import java.sql.ResultSet;
import java.sql.SQLException;

public interface QueryUpdateResponder {
	void onQueryUpdate(int affected, ResultSet set) throws SQLException;
}
