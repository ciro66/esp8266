var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
};
connection.onclose = function(){
    console.log('WebSocket connection closed');
};
function sendR () {
  var r = document.getElementById('r').value;
  connection.send("Led Rosso: " + r + "\n\r");
}
function sendG () {
  var g = document.getElementById('g').value;
  connection.send("Led Verde: " + g + "\n\r");
}
function sendB () {
  var b = document.getElementById('b').value;
  connection.send("Led Blu: " + b + "\n\r");
}
