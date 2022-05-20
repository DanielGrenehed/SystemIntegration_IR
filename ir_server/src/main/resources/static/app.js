
var table = new Tabulator("#data-table", {
    layout:"fitColumns",
    pagination:"local",
    paginationSize:5,
    paginationSizeSelector:[5, 10, 20, 50],
    paginationCounter:"rows",
    columns:[
        {title:"Code", field:"signal"},
        {title:"Sensor ID", field:"sensor_id"},
        {title:"Timestamp", field:"time"},
    ],
});

let ws = new WebSocket('ws://localhost:8081/ir-sensor');

function addDataToTable(data) {
    table.addData([data], true);
    table.redraw();
}

ws.onmessage = function(event) {

    let reading = JSON.parse(event.data);
    console.log(reading);
    if(reading.constructor === Array) {
        reading.forEach(irData => addDataToTable(irData));
    } else {
        addDataToTable(reading);
    }

}

ws.onopen = function (){
    console.log("connected")
    ws.send(JSON.stringify({sensor_id:5}));
}
