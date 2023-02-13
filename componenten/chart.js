async function create_chart() {
    const res = await fetch('xwaarden.json')
    var xValues = await res.json();
    var yValues = [5, 17, 8, 8, 9, 9, 9, 10, 11, 14, 14, 15, 13, 15, 18];
    const arr = yValues;
    const Gewenste_waarden = 10
    const lijst_gew = []
    const lijst_max = []
    const lijst_min = []
    var maximum = Math.max(...arr)
    var minimum = Math.min(...arr)

    // Bereken gemiddelde waarde van arr
    var som = 0;
    for (var number of arr) {
      som += number;
    }
    var gem = som / arr.length;
    var arr_2 = []
    for (var _ of arr) {
      arr_2.push(gem)
    }
    for (var _ of arr) {
      lijst_gew.push(Gewenste_waarden)
    }
    for (var _ of arr) {
      lijst_max.push(maximum)
    }
    for (var _ of arr) {
      lijst_min.push(minimum)
    }
    new Chart("myChart", {
      type: "line",
      data: {
        labels: xValues,
        datasets: [{
          fill: false,
          label: "Waterwaarden",
          lineTension: 0,
          backgroundColor: "rgba(0,0,255,1)",
          borderColor: "rgba(0,0,255,1)",
          data: yValues
        },
        {
          fill: false,
          lineTension: 0,
          label: "Gemiddelde waterwaarden",
          backgroundColor: "rgba(0,255,0,1.0)",
          borderColor: "rgba(0,255,0,1)",
          data: arr_2
        },
        {
          fill: false,
          lineTension: 0,
          label: "Gewenste waterwaarden",
          backgroundColor: "rgba(255,0,0,1.0)",
          borderColor: "rgba(255,0,0,1)",
          data: lijst_gew
        },
        {
          fill: false,
          lineTension: 0,
          label: "Hoogste Waterwaarden",
          backgroundColor: "rgba(0,0,0,1)",
          borderColor: "rgba(0,0,0,1)",
          data: lijst_max
        },
        {
          fill: false,
          lineTension: 0,
          label: "Laagste Waterwaarden",
          backgroundColor: "rgba(0,0,0,1)",
          borderColor: "rgba(0,0,0,1)",
          data: lijst_min
        }
        ]
      },
      options: {
        legend: { display: true },
        scales: {
          yAxes: [{
            ticks: {
              beginAtZero: true,
              min: 0,
              max: Math.max(...arr) + 1,
              stepSize: 1
            },
            display: true,
            scaleLabel: {
              display: true,
              labelString: 'Waarden in Liter'
            }
          }],
          xAxes: [{
            display: true,
            scaleLabel: {
              display: true,
              labelString: 'Tijd in uur '
            }
          }]
        }
      }
    });
  }