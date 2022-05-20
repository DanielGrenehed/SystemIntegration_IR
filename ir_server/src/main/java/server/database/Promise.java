package server.database;

public class Promise {
	private Object object = null;

	public Promise(){}

	public void set(Object o) {
		object = o;
	}

	public Object get() {
		return object;
	}

	public boolean isSet() {
		return object != null;
	}
}
