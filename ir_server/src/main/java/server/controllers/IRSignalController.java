package server.controllers;


import org.springframework.web.bind.annotation.*;
import server.ir_signal.IRSignal;
import server.ir_signal.IRSignalDAO;
import server.ir_signal.IRSignalInsert;

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

	@GetMapping("/{id}/{limit}")
	public List<IRSignal> getLimitedIRSignalHistoryForSensor(@PathVariable int id, @PathVariable int limit) throws SQLException {
		return dao.getSignalsFromSensor(id, limit);
	}

	@GetMapping("/{id}/latest")
	public IRSignal getLatestIRSignalFromSensor(@PathVariable int id) throws SQLException {
		return dao.getLatestSignalFromSensor(id);
	}

	@PostMapping("")
	public IRSignal postIRSignal(@RequestBody IRSignalInsert insert) throws SQLException {
		return dao.insertSignal(insert);
	}

}
