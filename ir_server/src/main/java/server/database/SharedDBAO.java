package server.database;

public class SharedDBAO {

	private static DatabaseAccessObject database_access_object = null;

	public static DatabaseAccessObject getInstance() {
		if (database_access_object == null) database_access_object = new DatabaseAccessObject();
		return database_access_object;
	}

	public static void setInstance(DatabaseAccessObject dbao) {
		database_access_object = dbao;
	}
}
