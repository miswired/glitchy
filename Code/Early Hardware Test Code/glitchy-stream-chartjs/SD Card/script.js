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
  
  
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
    document.getElementById('button_adc').addEventListener('click', startADC);
	document.getElementById('stream_amp_bias_charts_toggle').addEventListener('click', stream_control);
  }

  //parse incoming websocket messages
  function onMessage(event) {
	  var myObj = JSON.parse(event.data);
	  console.log(myObj);
	  
	  if("CommsVersion" in myObj){
		//console.log("has CommsVersion");
		if(myObj.CommsVersion == 1.1){
		  //console.log("CommsVersion is 1.1");
		  if("PacketType" in myObj){
			//console.log("has PacketType");
			if(myObj.PacketType = "ADCStream"){
				//console.log("PacketType is ADCStream");
				ChartAmpOut.data.datasets[0].data.push({
					x: Date.now(),
					y: myObj.LastAmpInVolts
				});
				ChartBiasOut.data.datasets[0].data.push({
					x: Date.now(),
					y: myObj.LastBiasVolts
				});
			}
			if(myObj.PacketType = "KeyTraces"){
				console.log("PacketType is KeyTraces");
				//populate key1
				ChartPowerAnalysis.data.datasets[0].data.push({
					x: Date.now(),
					y: myObj.LastAmpInVolts
				});
				//populate key2
				ChartPowerAnalysis.data.datasets[1].data.push({
					x: Date.now(),
					y: myObj.LastBiasVolts
				});
				//populate key3
				ChartPowerAnalysis.data.datasets[3].data.push({
					x: Date.now(),
					y: myObj.LastBiasVolts
				});
			}
		  }			  
		}
	  }
	}
  
  //test function
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
	  
  function stream_control() {
	  console.log("streamControl called");
	  var checkBox = document.getElementById("stream_amp_bias_charts_toggle");
	  if(checkBox.checked == true)
	  {
		  console.log("checked");
		  websocket.send('enable_adc_stream');
	  }
	  else
	  {
		  console.log("unchecked");
		  websocket.send('disable_adc_stream');
	  }
	  
  }
  
  function startADC(){
    websocket.send('start_adc');
  }

  function toggle(){
    websocket.send('toggle');
  }