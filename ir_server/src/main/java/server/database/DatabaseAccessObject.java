package server.database;
import java.io.FileInputStream;
import java.io.IOException;
import java.sql.*;
import java.util.Properties;

public class DatabaseAccessObject {

	Connection database = null;

	public DatabaseAccessObject() {
		Properties p = new Properties();
		try {
			p.load(new FileInputStream("src/main/resources/application.properties"));
			System.out.println("Loading application properties");
			Class.forName(p.getProperty("spring.datasource.driver-class-name"));
			System.out.println("Loading database driver");
			database = DriverManager.getConnection(p.getProperty("spring.datasource.url"), p.getProperty("spring.datasource.username"), p.getProperty("spring.datasource.password"));
			System.out.println("Database connection established");
		} catch (IOException e) {
			throw new RuntimeException(e);
		} catch (ClassNotFoundException e) {
			throw new RuntimeException(e);
		} catch (SQLException e) {
			throw new RuntimeException(e);
		}

	}

	public void QueryUpdate(PreparedStatement statement, QueryUpdateResponder responder) throws SQLException {
		int width = statement.executeUpdate();
		ResultSet set = statement.getGeneratedKeys();
		responder.onQueryUpdate(width, set);
		set.close();
		statement.close();
	}

	public void Query(String query, QueryResponder responder) throws SQLException {
		Statement st = database.createStatement();
		ResultSet rs = st.executeQuery(query);
		responder.onQuerySet(rs);
		rs.close();
		st.close();
	}

	public void Query(PreparedStatement statement, QueryResponder responder) throws SQLException {
		ResultSet rs = statement.executeQuery();
		responder.onQuerySet(rs);
		rs.close();
		statement.close();
	}

	public PreparedStatement prepareStatement(String query) throws SQLException {
		return database.prepareStatement(query);
	}

	public PreparedStatement prepareUpdateStatement(String query) throws SQLException {
		return database.prepareStatement(query, Statement.RETURN_GENERATED_KEYS);
	}

	public void close() throws SQLException {
		database.close();
	}


}
