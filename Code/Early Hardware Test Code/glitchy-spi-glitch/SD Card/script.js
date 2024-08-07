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
    document.getElementById('start_glitching_button').addEventListener('click', start_glitching);
    document.getElementById('button_start_power_analysis').addEventListener('click', start_power_analysis);
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
			if(myObj.PacketType == "ADCStream"){
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
			if(myObj.PacketType == "KeyTraces"){
				console.log("PacketType is KeyTraces");
				
				//generate x axis from array size
				let key1xAxis = [];
				let key2xAxis = [];
				let key3xAxis = [];
				
				let maxlength = 0;
				if(myObj.key_1_array.length > maxlength){maxlength = myObj.key_1_array.length;}
				if(myObj.key_2_array.length > maxlength){maxlength = myObj.key_2_array.length;}
				if(myObj.key_3_array.length > maxlength){maxlength = myObj.key_3_array.length;}
				
				for(let i = 0; i < maxlength; i++) {
					powerAnalysisXValues[i] = i;
				}
				
				//Populate data
				ChartPowerAnalysis.data.datasets[0].data = myObj.key_1_array;
				ChartPowerAnalysis.data.datasets[1].data = myObj.key_2_array;
				ChartPowerAnalysis.data.datasets[2].data = myObj.key_3_array;

				ChartPowerAnalysis.update();
			}
			if(myObj.PacketType == "UpdateGlitchStatus"){
				console.log("PacketType is UpdateGlitchStatus");
				
				let current_status = ""
				if(myObj.success == false){
					if(myObj.running == true){
						current_status = "Running" + ":" + " Current Delay:" + myObj.delay_value + "ns " + " Try Number:" + myObj.try_number;
					}else{
						current_status = "Stopped";
					}
				}else{
					current_status = "SUCCESS!!! " + "Last Run: " + "Delay:" + myObj.delay_value + "ns " + " Try Number:" + myObj.try_number;
				}
				console.log(current_status);
				document.getElementById("glitching_status").innerText = current_status;
				
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
  
  function push2(){
	let testArray = [];
	for(let i = 0; i < 50; i++) {
		powerAnalysisXValues[i] = i;
		testArray[i] = i*2;
	}
	
	
	//populate key1
	ChartPowerAnalysis.data.datasets[0].data = testArray;
	ChartPowerAnalysis.data.datasets[1].data = testArray;
	ChartPowerAnalysis.data.datasets[2].data = testArray;
	
	ChartPowerAnalysis.update();
	  
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
  
  function start_power_analysis(){
    websocket.send('start_power_analysis');
  }

  function start_glitching(){
    websocket.send('start_glitching');
  }
  
  function test_state_text(){
	  document.getElementById("glitching_status").innerText = "Changed";
  }
