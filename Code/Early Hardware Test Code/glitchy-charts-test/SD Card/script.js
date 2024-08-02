const chart_obj = document.getElementById('myChart');

new Chart(chart_obj, {
	type: 'line',
	data: {
	  labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
	  datasets: [{
		label: '# of Votes',
		data: [12, 19, 3, 5, 2, 3],
		borderWidth: 1
	  }]
	},
	options: {
	  scales: {
		y: {
		  beginAtZero: true
		}
	  }
	}
});

var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
    document.getElementById('button_adc').addEventListener('click', startADC);
  }
  function startADC(){
    websocket.send('start_adc');
  }

  function toggle(){
    websocket.send('toggle');
  }