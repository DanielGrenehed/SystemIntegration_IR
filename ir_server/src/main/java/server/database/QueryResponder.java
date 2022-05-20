package server.database;

import java.sql.ResultSet;
import java.sql.SQLException;

public interface QueryResponder {
	void onQuerySet(ResultSet set) throws SQLException;
}
