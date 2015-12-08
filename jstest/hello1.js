var express = require('express');
http = require('http'),
app = express(),
server = http.createServer(app);
var bodyParser = require('body-parser');
var GPIO = require('onoff').Gpio,
	led = new GPIO(13, 'out'),
	servomotor2 = new GPIO(27, 'out'),
	SevenSegment_1 = new GPIO(19, 'out'),
	SevenSegment_2 = new GPIO(26, 'out'),
	SevenSegment_3 = new GPIO(23, 'out'),
	SevenSegment_4 = new GPIO(24, 'out'),
	SevenSegment_5 = new GPIO(25, 'out'),
	SevenSegment_6 = new GPIO(12, 'out'),
	SevenSegment_7 = new GPIO(20, 'out'),
	tvled = new GPIO(21, 'out');


var TVchannelnumber = 0;
var piblaster = require('pi-blaster.js');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended:false}));

app.get('/', function(req,res){
res.sendfile('control.html', {root: __dirname});
});


function showSevenSegmentChannel(channelnumber){
	console.log("I'am here");
	switch(channelnumber){
	case 0:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(0);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(1);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	case 1:
		SevenSegment_1.writeSync(0);
		SevenSegment_2.writeSync(0);
		SevenSegment_3.writeSync(0);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(0);
		SevenSegment_7.writeSync(1);
		break;
	case 2:
		SevenSegment_1.writeSync(0);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(1);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(0);
		break;
	case 3:
		SevenSegment_1.writeSync(0);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	case 4:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(0);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(0);
		SevenSegment_7.writeSync(1);
		break;
	case 5:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(0);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	case 6:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(0);
		SevenSegment_5.writeSync(1);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	case 7:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(0);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(0);
		SevenSegment_7.writeSync(1);
		break;
	case 8:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(1);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	case 9:
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(1);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		break;
	default:
		break;
	}
}

app.post('/led',function(req,res){
	var state = req.body.led;
	var reserve = req.body.ledreserve;
	if(state == 'on'){
		led.writeSync(1);
		console.log(state);
	}else if(state == 'off'){
		led.writeSync(0);
		console.log(state);
	}

	if(reserve == 'ledreserveOn'){
		var date = new Date();
		var LED = req.body.LEDTime;
		var newDate = new Date(LED);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){led.writeSync(1);
			console.log(reserve + " is now working!!");
			}, comparedate);
	}else if (reserve == 'ledreserveOff'){
		var date = new Date();
		var LED = req.body.LEDTime;
		var newDate = new Date(LED);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){led.writeSync(0)}, comparedate);
	}

	res.sendfile('control.html',{root:__dirname});
});

app.post('/servomotor',function(req,res){
	var state = req.body.servomotor2;
	var reserve = req.body.WMreserve;
	if(state == 'on'){
		servomotor2.writeSync(1);
		console.log(state);
		
	}else if(state == 'off'){
		servomotor2.writeSync(0);
		console.log(state);
	}

	if(reserve == 'wmreserveOn'){
		var date = new Date();
		var WM = req.body.WMTime;
		var newDate = new Date(WM);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){servomotor2.writeSync(1);
			console.log(reserve + " is now working!!");
			}, comparedate);
	}else if (reserve == 'wmreserveOff'){
		var date = new Date();
		var WM = req.body.WMTime;
		var newDate = new Date(WM);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){servomotor2.writeSync(0);
			console.log(reserve + " is now working!!");
			}, comparedate);
	}

	res.sendfile('control.html',{root:__dirname});
});


app.post('/TVled',function(req,res){
	var state = req.body.tvled;
	if(state == 'on'){
		tvled.writeSync(1);
		SevenSegment_1.writeSync(1);
		SevenSegment_2.writeSync(0);
		SevenSegment_3.writeSync(1);
		SevenSegment_4.writeSync(1);
		SevenSegment_5.writeSync(1);
		SevenSegment_6.writeSync(1);
		SevenSegment_7.writeSync(1);
		console.log(state);
	}else{
		tvled.writeSync(0);
		SevenSegment_1.writeSync(0);
		SevenSegment_2.writeSync(0);
		SevenSegment_3.writeSync(0);
		SevenSegment_4.writeSync(0);
		SevenSegment_5.writeSync(0);
		SevenSegment_6.writeSync(0);
		SevenSegment_7.writeSync(0);
		console.log(state);
	}
	res.sendfile('control.html',{root:__dirname});
});


app.post('/TVchannel',function(req,res){
	var state = req.body.tvled;
	var channelcommand = req.body.tvchannelcommand;
	var reserve = req.body.TVreserve;
	if(tvled.readSync()==1){
		if(channelcommand == 'up'){
			tvled.writeSync(1);
			if(TVchannelnumber==9)
				TVchannelnumber=1;
			else
				TVchannelnumber++;
			showSevenSegmentChannel(TVchannelnumber);
			console.log(TVchannelnumber);
		}else{
			tvled.writeSync(1);
			if(TVchannelnumber==1)
				TVchannelnumber=9;
			else
				TVchannelnumber--;
			showSevenSegmentChannel(TVchannelnumber);
			console.log(TVchannelnumber);
		}
	}else{

		tvled.writeSync(0);
	}


	if(reserve == 'tvreserveOn'){
		var date = new Date();
		var TVchtime = req.body.TVCHTime;
		TVchannelnumber = req.body.TVCHValue;
		TVchannelnumber *= 1;
		var newDate = new Date(TVchtime);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){
			tvled.writeSync(1);
			showSevenSegmentChannel(TVchannelnumber);
			console.log(reserve + " is now working!! This is" +TVchannelnumber);
			}, comparedate);
	}else if (reserve == 'tvreserveOff'){
		var date = new Date();
		var TVchtime = req.body.TVCHTime;
		var newDate = new Date(TVchtime);		
		var comparedate = newDate - date -(9* 1000* 60 * 60);
		console.log(comparedate + "  " + reserve);
		setTimeout(function(){
			tvled.writeSync(0);
			SevenSegment_1.writeSync(0);
			SevenSegment_2.writeSync(0);
			SevenSegment_3.writeSync(0);
			SevenSegment_4.writeSync(0);
			SevenSegment_5.writeSync(0);
			SevenSegment_6.writeSync(0);
			SevenSegment_7.writeSync(0);
			console.log(reserve + " is now working!!");
			}, comparedate);
	}

	res.sendfile('control.html',{root:__dirname});
});


server.listen(8000, function(){
console.log('express server listening on port' + server.address().port);
});

