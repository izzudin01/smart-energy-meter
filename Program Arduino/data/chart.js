var chartE = new Highcharts.Chart({
    chart:{ renderTo : 'chart-energi'},
    title: { 
        text: 'Konsumsi Energi',
        style: {
          color: 'rgb(255, 242, 221)',
          font: 'bold 16px Lato'
       } 
    },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#52DE97' }
    },
    xAxis: { type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Energi (KWh)',
      style: {
        color: 'rgb(255, 242, 221)',
        font: 'bold 16px Lato'
     }  }
    },
    credits: { enabled: false }
  });
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        //console.log(this.responseText);
        if(chartE.series[0].data.length > 40) {
          chartE.series[0].addPoint([x, y], true, true, true);
        } else {
          chartE.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/energi", true);
    xhttp.send();
 }, 5000 ) ;
  
  var chartT = new Highcharts.Chart({
    chart:{ renderTo : 'chart-tegangan' },
    title: { 
      text: 'Penggunaan Tegangan',
      style: {
        color: 'rgb(255, 242, 221)',
        font: 'bold 16px Lato',
     } 
  
  },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#54E346' }
    },
    xAxis: { type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Tegangan (Volt)',
      style: {
        color: 'rgb(255, 242, 221)',
        font: 'bold 16px Lato'
     }  }
    },
    credits: { enabled: false }
  });
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        //console.log(this.responseText);
        if(chartT.series[0].data.length > 40) {
          chartT.series[0].addPoint([x, y], true, true, true);
        } else {
          chartT.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/tegangan", true);
    xhttp.send();
  }, 5000 ) ;
  
var chartA = new Highcharts.Chart({
    chart:{ renderTo:'chart-arus' },
    title: { text: 'Arus pada Beban',
    style: {
      color: 'rgb(255, 242, 221)',
      font: 'bold 16px Lato'
   }  },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#0CECDD' }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Arus (Ampere)',
      style: {
        color: 'rgb(255, 242, 221)',
        font: 'bold 16px Lato'
     }  }
    },
    credits: { enabled: false }
  });
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        //console.log(this.responseText);
        if(chartA.series[0].data.length > 40) {
          chartA.series[0].addPoint([x, y], true, true, true);
        } else {
          chartA.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/arus", true);
    xhttp.send();
}, 5000 ) ;
  
  var chartW = new Highcharts.Chart({
    chart:{ renderTo:'chart-daya' },
    title: { text: 'Daya Terpakai',
    style: {
      color: 'rgb(255, 242, 221)',
      font: 'bold 16px Lato'
   }  },
    series: [{
      showInLegend: false,
      data: []
    }],
    plotOptions: {
      line: { animation: false,
        dataLabels: { enabled: true }
      },
      series: { color: '#28FFBF' }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { text: 'Daya (Watt)',
      style: {
        color: 'rgb(255, 242, 221)',
        font: 'bold 16px Lato'
     }  }
    },
    credits: { enabled: false }
  });
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
            y = parseFloat(this.responseText);
        //console.log(this.responseText);
        if(chartW.series[0].data.length > 40) {
          chartW.series[0].addPoint([x, y], true, true, true);
        } else {
          chartW.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", "/daya", true);
    xhttp.send();
  }, 5000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("energi").innerHTML = this.responseText;
        var y = parseFloat(this.responseText);
        if(y >= 0.5 ){
          document.getElementById("alert-box").style.display = "flex";  
        }
      }
    };
    xhttp.open("GET", "/energi", true);
    xhttp.send();
  }, 5000 ) ;

  function closeForm() {
    document.getElementById("alert-box").style.display = "none";
  }
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("harga").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/harga", true);
    xhttp.send();
  }, 5000 ) ;

  function clock() {
    var hours = document.getElementById("hours");
    var minutes = document.getElementById("minutes");
    var seconds = document.getElementById("seconds");
    var phase = document.getElementById("phase");
  
    var h = new Date().getHours();
    var m = new Date().getMinutes();
    var s = new Date().getSeconds();
  
    h = h < 10 ? "0" + h : h;
    m = m < 10 ? "0" + m : m;
    s = s < 10 ? "0" + s : s;
  
    hours.innerHTML = h;
    minutes.innerHTML = m;
    seconds.innerHTML = s;
    phase.innerHTML = am;
  }
var interval = setInterval(clock, 1000);
  
    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("arus").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/arus", true);
    xhttp.send();
  }, 5000 ) ;

    setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("tegangan").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/tegangan", true);
    xhttp.send();
  }, 5000 ) ;
  
      setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("daya").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/daya", true);
    xhttp.send();
  }, 5000 ) ;

      setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("energi1").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/energi", true);
    xhttp.send();
  }, 5000 ) ;