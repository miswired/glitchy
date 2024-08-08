
var powerAnalysisXValues = [];

const config_poweranalyis = {
  type: 'line',
  data: {
	labels: powerAnalysisXValues,
    datasets: [
      {
		label: 'Key1',
        backgroundColor: 'rgba(98, 242, 33, 0.96)',
        borderColor: 'rgba(98, 242, 33, 0.90)',
        cubicInterpolationMode: 'monotone',
        fill: false,
		tension: 0.1,
        data: []
      },
	  {
		label: 'Key2',
        backgroundColor: 'rgba(40, 175, 206, 0.96)',
        borderColor: 'rgba(40, 175, 206, 0.90)',
        cubicInterpolationMode: 'monotone',
        fill: false,
		tension: 0.1,
        data: []
      },
	  {
		label: 'Key3',
        backgroundColor: 'rgba(242, 33, 141, 0.96)',
        borderColor: 'rgba(242, 33, 141, 0.90)',
        cubicInterpolationMode: 'monotone',
        fill: false,
		tension: 0.1,
        data: []
      }
    ]
  },
  options: {
    scales: {
      x: {
		title: {
			display: true,
			text: 'Samples'
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
      }
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

const config_biasout = {
  type: 'line',
  data: {
    datasets: [
      {
		label: 'Bias Output',
        backgroundColor: 'rgba(54, 162, 235, 0.5)',
        borderColor: 'rgb(54, 162, 235)',
        cubicInterpolationMode: 'monotone',
        fill: true,
        data: []
      }
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

const ChartBiasOut = new Chart(
  document.getElementById('ChartBias_Canvasobj'),
  config_biasout
);

const ChartPowerAnalysis = new Chart(
  document.getElementById('ChartPowerAnalysis_Canvasobj'),
  config_poweranalyis
);