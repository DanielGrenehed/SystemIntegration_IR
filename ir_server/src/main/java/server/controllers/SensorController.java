package server.controllers;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import server.sensor.Sensor;
import server.sensor.SensorDAO;

import java.sql.SQLException;
import java.util.List;

@RestController
@RequestMapping("/sensor")
public class SensorController {

	private SensorDAO dao = new SensorDAO();

	@GetMapping("/{id}")
	public Sensor getSensorById(@PathVariable int id) throws SQLException {
		return dao.getSensorByID(id);
	}

	@GetMapping("/token/{token}")
	public Sensor getSensorBy(@PathVariable String token) throws SQLException {
		return dao.getSensorByToken(token);
	}

	@GetMapping("/all")
	public List<Sensor> getSensors() throws SQLException {
		return dao.getSensors();
	}

}
