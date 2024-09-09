  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; 
	console.log('Done with initWebsocket');
  }
  
  function onOpen(event) {
    console.log('Connection opened');
	get_glitch_param();
  }
  
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  
  
  function onLoad(event) {
	console.log('onLoad called');
	//ChartAmpOut.data.datasets[0].data = [];
	//ChartBiasOut.data.datasets[0].data = [];
    initWebSocket();
    initButton();
	console.log('onLoad Done');
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
			if(myObj.PacketType == "glitch_param"){
				console.log("PacketType is glitch_param");

				console.log(myObj.CommsVersion);
				console.log(myObj.PacketType);
				console.log(myObj.start_time_ns);
				console.log(myObj.stop_time_ns);
				console.log(myObj.delay_between_glitches_ms);
				console.log(myObj.step_size_ns);
				console.log(myObj.retry_times);

				document.getElementById("start_time_ns_input").value = myObj.start_time_ns;
				document.getElementById("stop_time_ns_input").value = myObj.stop_time_ns;
				document.getElementById("delay_between_glitches_ms_input").value = myObj.delay_between_glitches_ms;
				document.getElementById("step_size_ns_input").value = myObj.step_size_ns;
				document.getElementById("retry_times_input").value = myObj.retry_times;


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
		  //websocket.send('enable_adc_stream');
		  var obj = new Object();
		  obj.CommsVersion = 1.1;
		  obj.PacketType = "enable_adc_stream";
		  var jsonString= JSON.stringify(obj);
		  websocket.send(jsonString);
	  }
	  else
	  {
		  console.log("unchecked");
		  //websocket.send('disable_adc_stream');
		  var obj = new Object();
		  obj.CommsVersion = 1.1;
		  obj.PacketType = "disable_adc_stream";
		  var jsonString= JSON.stringify(obj);
		  websocket.send(jsonString);
	  }
	  
  }

  function get_glitch_param(){
	  console.log("get_glitch_param called");
	  var obj = new Object();
	  obj.CommsVersion = 1.1;
	  obj.PacketType = "get_glitch_param";
	  var jsonString= JSON.stringify(obj);
	  websocket.send(jsonString);
  }

  function send_glitch_param(){
	console.log("send_glitch_param called");
	var obj = new Object();
	obj.CommsVersion = 1.1;
	obj.PacketType = "set_glitch_param";
	obj.start_time_ns = parseInt(document.getElementById("start_time_ns_input").value);
	obj.stop_time_ns = parseInt(document.getElementById("stop_time_ns_input").value);
	obj.delay_between_glitches_ms = parseInt(document.getElementById("delay_between_glitches_ms_input").value);
	obj.step_size_ns = parseInt(document.getElementById("step_size_ns_input").value );
	obj.retry_times = parseInt(document.getElementById("retry_times_input").value);


	var jsonString= JSON.stringify(obj);
	websocket.send(jsonString);


  }

  function test_json_packet(){
	  console.log("test_json_packet called");
	  var obj = new Object();
	  obj.CommsVersion = 1.1;
	  obj.PacketType = "testpacket";
	  obj.somevalue  = 32;
	  obj.somebool = false;
	  var jsonString= JSON.stringify(obj);
	  websocket.send(jsonString);
  }

  
  function start_power_analysis(){
    //websocket.send('start_power_analysis');
	var obj = new Object();
	obj.CommsVersion = 1.1;
	obj.PacketType = "start_power_analysis";
	var jsonString= JSON.stringify(obj);
	websocket.send(jsonString);
  }

  function start_glitching(){
    //websocket.send('start_glitching');
	//Send the latest parameters
	console.log("Sending current parameters first");
	send_glitch_param();
	//Send the message to start glitching.
	//Maybe I should also run a get and verify the paremeters were set?
	var obj = new Object();
	obj.CommsVersion = 1.1;
	obj.PacketType = "start_glitching";
	var jsonString= JSON.stringify(obj);
	websocket.send(jsonString);
  }
  
  function test_state_text(){
	  document.getElementById("glitching_status").innerText = "Changed";
  }
