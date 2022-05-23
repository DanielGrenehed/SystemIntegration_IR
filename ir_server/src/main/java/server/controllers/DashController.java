package server.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import server.sensor.SensorDAO;

import java.sql.SQLException;

@Controller
public class DashController {

	private SensorDAO dao = new SensorDAO();

	@RequestMapping("/dash/{id}")
	public String showDash(@PathVariable int id, Model model) throws SQLException {
		model.addAttribute("sensor_id", id);
		return "dash";
	}

	@RequestMapping("/")
	public String showSensorDash(Model model) throws SQLException {
		model.addAttribute("sensor_list", dao.getSensors());
		return "sensordash";
	}
}
