package server.controllers;


import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import server.ir_signal.IRSignal;
import server.ir_signal.IRSignalDAO;

import java.sql.SQLException;
import java.util.List;

@RestController
@RequestMapping("/IR")
public class IRSignalController {

	private IRSignalDAO dao = new IRSignalDAO();

	@GetMapping("/{id}")
	public List<IRSignal> getIRSignalHistoryForSensor(@PathVariable int id) throws SQLException {
		return dao.getSignalsFromSensor(id);
	}

	@GetMapping("/{id}/latest")
	public IRSignal getLatestIRSignalFromSensor(@PathVariable int id) throws SQLException {
		return dao.getLatestSignalFromSensor(id);
	}

}
