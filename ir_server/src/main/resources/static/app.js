
var table = new Tabulator("#data-table", {
    layout:"fitColumns",
    pagination:"local",
    paginationSize:5,
    paginationSizeSelector:[5, 10, 20, 50],
    paginationCounter:"rows",
    columns:[
        {title:"Code", field:"code"},
        {title:"Token", field:"token"},
        {title:"Timestamp", field:"timestamp"},
    ],
});

let ws = new WebSocket('ws://localhost:8080/sensor');

ws.onmessage = function(event) {

    let reading = JSON.parse(event.data);
    console.log(reading);
    if(reading.constructor === Array) {
        reading.forEach(irData => table.addData([irData], true));
    } else{
        table.addData([reading], true);
    }



}

ws.onopen = function (){
    console.log("connected")
}
