
const m_sensor_id = document.getElementById("sensor_id_div").getAttribute("sensor_id");
let table = new Tabulator("#data-table", {
    layout:"fitColumns",
    pagination:"local",
    paginationSize:10,
    paginationSizeSelector:[5, 10, 20, 50],
    paginationCounter:"rows",
    columns:[
        {title:"Code", field:"signal", sorter:"string"},
        {title:"Timestamp", field:"time", sorter:"string"},
    ],
});

let ws = new WebSocket('ws://localhost:8081/ir-sensor');

function dateFromTimestamp(timestamp) {
    return new Date(timestamp);
}

function addHoursToDate(date, hours) {
    date.setTime(date.getTime() + hours * 60 * 60 * 1000);
    return date;
}

function formatDate(time) {
    const date = new Date(time);
    return date.toLocaleString('en-GB', { timeZone: 'CET' });
}

function addListToTable(list) {
    for (let i = 0; i < list.length; i++) {
        let obj = list[i];
        obj.time = formatDate(dateFromTimestamp(obj.time));
        table.addData([obj], false)
    }
    table.redraw();
}

function addDataToTable(data) {
    let obj = data;
    obj.time = formatDate(addHoursToDate(dateFromTimestamp(data.time), 2));
    table.addData([obj], true);
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
    ws.send(JSON.stringify({sensor_id:m_sensor_id}));
}

// load history
url = '/IR/' + m_sensor_id + '/' + 30; // 30 chosen by a fair dice roll
fetch(url).then(res => res.json()).then((out) => {
    addListToTable(out);
});