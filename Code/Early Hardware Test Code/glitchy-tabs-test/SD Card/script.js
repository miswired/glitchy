const chart_obj = document.getElementById('myChart');

var xValues = [];

for(let i = 0; i < 100; i++) {
	xValues[i]=i;
}

var yValues = [];

const myChart = new Chart(chart_obj, {
	type: 'line',
	data: {
	  labels: xValues,
	  datasets: [{
		fill: false,
		label: 'Test Chart',
		data: yValues,
		borderWidth: 1,
		tension: 0.1
	  }]
	},
	options: {
	  scales: {
		y: {
		  beginAtZero: false
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
    websocket.onmessage = onMessage; 
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  /*
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
  */
  
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
  
  function onMessage(event) {
	  var myObj = JSON.parse(event.data);
	  console.log(myObj);
	  myChart.data.datasets[0].data = myObj.chartarray;
	  myChart.update();
	  /*
	  for (i in myObj.gpios){
		var output = myObj.gpios[i].output;
		var state = myObj.gpios[i].state;
		console.log(output);
		console.log(state);
		if (state == "1"){
		  document.getElementById(output).checked = true;
		  document.getElementById(output+"s").innerHTML = "ON";
		}
		else{
		  document.getElementById(output).checked = false;
		  document.getElementById(output+"s").innerHTML = "OFF";
		}
	  }
	  */
	  //console.log(event.data);
	}
  
  function selectTab(tabIndex) {
         //Hide All Tabs
         document.getElementById("tab1Content").style.display = "none";
         document.getElementById("tab2Content").style.display = "none";
         document.getElementById("tab3Content").style.display = "none";
         //Show the Selected Tab
         document.getElementById("tab" + tabIndex + "Content").style.display =
         "block";
      }
	  
