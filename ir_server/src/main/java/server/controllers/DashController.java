package server.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

import java.sql.SQLException;

@Controller
public class DashController {

	@RequestMapping("/dash/{id}")
	public String showDash(@PathVariable int id, Model model) throws SQLException {
		model.addAttribute("sensor_id", id);
		return "dash";
	}
}
