



//const chart_obj = document.getElementById('ChartAmpOut');

//var xValues = [];
//var yValues = [];

/*
for(let i = 0; i < 100; i++) {
	xValues[i] = i;
	yValues[i] = i;
}
*/

const config_ampout = {
  type: 'line',
  data: {
    datasets: [
      {
		label: 'AMP Output',
        backgroundColor: 'rgba(54, 162, 235, 0.5)',
        borderColor: 'rgb(54, 162, 235)',
        cubicInterpolationMode: 'monotone',
        fill: true,
        data: []
      },
	  /*
      {
		label: 'Dataset 2',
        backgroundColor: 'rgba(54, 162, 235, 0.5)',
        borderColor: 'rgb(54, 162, 235)',
        cubicInterpolationMode: 'monotone',
        fill: true,
        data: []
      }
	  */
    ]
  },
  options: {
    scales: {
      x: {
        type: 'realtime',
        realtime: {
		  delay: 1000,
        },
		title: {
			display: true,
			text: 'Time'
		  }
	  },
	  y: {
		  title: {
			display: true,
			text: 'Volts'
		  }
		}
    }
  }
};

const ChartAmpOut = new Chart(
  document.getElementById('ChartAmpOut_Canvasobj'),
  config_ampout
);


/*
const myChart = new Chart(chart_obj, {
	type: 'line',
	data: {
	  labels: xValues,
	  datasets: [{
		fill: false,
		label: 'Test Chart',
		data: yValues,
		borderWidth: 1,
		pointBackgroundColor: 'rgba(0, 0, 0, 0)',
        pointBorderColor: 'rgba(0, 0, 0, 0)',
		pointRadius: 0,
		tension: 0.1
	  }]
	},
	options: {
		animation: {
			duration: 0
		},
	  scales: {
		y: {
		  beginAtZero: false,
		  title: {
			display: true,
			text: 'Volts'
		  }
		},
		x: {
		  title: {
			display: true,
			text: 'Samples'
		  }
		}
	  }
	}
});
*/

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
	  
	  if("CommsVersion" in myObj){
		console.log("has CommsVersion");
		if(myObj.CommsVersion == 1.1){
		  console.log("CommsVersion is 1.1");
		  if("PacketType" in myObj){
			console.log("has PacketType");
			if(myObj.PacketType = "ADCStream"){
				console.log("PacketType is ADCStream");
				ChartAmpOut.data.datasets[0].data.push({
					x: Date.now(),
					y: myObj.LastAmpInVolts
				});
			}
		  }			  
		}
	  }
	  //myChart.data.datasets[0].data = myObj.chartarray;
	  
	  /*
	  let xAxis = [];
	  for(let i = 0; i < myObj.chartarray.length; i++) {
			xAxis[i] = i;
	  }
	  */
	  
	  /*
	  myChart.data.datasets.forEach(function(dataset) {
		dataset.data.push({
			x: Date.now(),
			y: myObj.chartarray[myObj.chartarray.length-1]
		});
	  });
	  */
	  
	  //myChart.data.labels = xAxis;
	  
	  //myChart.update();
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
  
  function push(){
		ChartAmpOut.data.datasets[0].data.push({
			x: Date.now(),
			y: 1.1
		});   
  }
  
  
  function selectTab(tabIndex) {
         //Hide All Tabs
         document.getElementById("tab1Content").style.display = "none";
         document.getElementById("tab2Content").style.display = "none";
         document.getElementById("tab3Content").style.display = "none";
		 document.getElementById("tab4Content").style.display = "none";
         //Show the Selected Tab
         document.getElementById("tab" + tabIndex + "Content").style.display =
         "block";
      }
	  
