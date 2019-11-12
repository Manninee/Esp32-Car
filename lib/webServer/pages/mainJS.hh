#ifndef MAINJS_hh
#define MAINJS_hh

namespace pages
{
const char mainPageJs[] PROGMEM = R"=====(
var dataBuffer = [];
var mouseData = {xValue : 255, yValue: 100, pressed: false, pressChange: false};
var keyboardData = {keys: ['w', 'a', 's', 'd'], keysPressed: [false, false, false, false], pressChange: false};

var emptyCounter = 0;
setInterval(transferData, 100);

// Elements
const slider = document.getElementById("sliderRange");
const sliderValue = document.getElementById("sliderValue");
const keyboardButton = document.getElementById("keyboardSwitch");
const mouseButton = document.getElementById("mouseSwitch");
const cameraButton = document.getElementById("cameraSwitch");
const reverseButton = document.getElementById("reverseSwitch");
const joystick = document.getElementById("joystick");
const joystickContainer = document.getElementById("joystick-container");
const joystickValues = document.getElementById("joystickValues");
const stream = document.getElementById("stream");
const streamSlot = document.getElementById("streamSlot");

// Start states
document.addEventListener("keydown", getKeyPress, false);
document.addEventListener("keyup", getKeyPress, false);
slider.disabled = true;
document.getElementById("streamSlot").hidden = true;

// Oninput events
keyboardButton.oninput = switchHandler;
mouseButton.oninput = switchHandler;
cameraButton.oninput = switchHandler;
reverseButton.oninput= switchHandler;
slider.oninput = function(){ sliderValue.innerHTML = slider.value;}

// Mouse events
joystick.onmousemove = getCoordinates;
joystick.onmouseup = resetCoordinates;
joystick.onmousedown = startMousePress;
//joystick.onmouseout = resetCoordinates;

// Touch events
joystick.ontouchmove = getCoordinates;
joystick.ontouchstart = startMousePress;
joystick.ontouchcancel = resetCoordinates;
joystick.ontouchend = resetCoordinates;

function getCoordinates(event){
if(!mouseButton.checked)
	return;

var tempX = 0; var tempY = 0;
if(event.type == "mousemove" && event.buttons === 1){
	tempX = event.clientX;
	tempY = event.clientY;
}
else if(event.type == "touchmove"){
	tempX = event.touches[0].clientX;
	tempY = event.touches[0].clientY;
}
else
	return;

var halfSize = joystick.clientWidth / 2;
var x = (tempX - joystick.getBoundingClientRect().left) - halfSize;
var y = halfSize - (tempY - joystick.getBoundingClientRect().top);

if(x > halfSize)
	x = halfSize;
else if(x < -halfSize)
	x = -halfSize;

if(y > halfSize)
	y = halfSize;
else if(y < -halfSize)
	y = -halfSize;

mouseData.xValue = Math.round(x);
mouseData.yValue = Math.round(y);

joystickValues.innerHTML = "X value: " + mouseData.xValue + ", Y value: " +  mouseData.yValue;
}

function resetCoordinates(event){
mouseData.xValue = 0;
mouseData.yValue = 0;
joystickValues.innerHTML = "X value: " + mouseData.xValue + ", Y value: " +  mouseData.yValue;
mouseData.pressed = false;
}

function startMousePress(){
mouseData.pressed = true;
mouseData.pressChange = true;
}

function switchHandler(event){
var status = false;
var source = "";
switch(event.srcElement.id){
case "keyboardSwitch":
	status = keyboardButton.checked;
	keyboardSteering = status;
	slider.disabled = !status;
	if(status)
		mouseButton.checked = false;
	else{
		slider.value = 50;
		sliderValue.innerHTML = 50;
	}
	return;
case "mouseSwitch":
	status = mouseButton.checked;
	joystick.style.background = status ? 'green' : 'red';
	if(status)
		keyboardButton.checked = false;
	return;
case "cameraSwitch":
	status = cameraButton.checked;
	streamSlot.hidden = !status;
	document.reverseButton.disabled = !status;
	if(!status){
		stream.src = "";
		if(reverseButton.checked)
			addData("R", 0);
	}
	source = "C";
	break;
case "reverseSwitch":
	status = reverseButton.checked;
	source = "R";
	break;
default:
	return;
}
status = status ? 1:0;
addData(source, status);
}

function addData(type, value){ 
for(i = 0; i < dataBuffer.length; i++){
    if(dataBuffer[i][0] === type){
        dataBuffer[i][1] = value;
		return;
}}
dataBuffer.push([type, value]);
}

function getKeyPress(event){
if(keyboardButton.checked){
	const index = keyboardData.keys.indexOf(event.key.toLowerCase());
    if(index != -1){
		keyboardData.pressChange = true;
        keyboardData.keysPressed[index] = event.type == 'keydown';
}}}

function parseDataUrl(){
var temp = dataBuffer;
dataBuffer = [];
const len = temp.length;

value = getSpeedAndDirection();

if(len == 0 && !value.pressed)
	return "/data";

var url = "/data?" + "S" + "=" + value.speed + "&D" + "=" + value.direction + "&";
for(i = 0; i < len; i++)
    url += temp[i][0] + "=" + temp[i][1] + "&";

return url.substr(0, url.length - 1);
}

function getSpeedAndDirection(){
var data = {
	speed: 255,
	direction: 100,
	pressed: false
}
if(keyboardButton.checked){
	var speedValue = parseInt(slider.value) * 2.55;
	var press = false;
	for(i = 0; i < keyboardData.keysPressed.length; i++){
		if(keyboardData.keysPressed[i] == true){
		press = true;
		switch(keyboardData.keys[i]){
		case 'a':
			data.direction -= 100; break;	
		case 'd':
			data.direction += 100; break;
		case 'w':
			data.speed += speedValue; break;
		case 's':
			data.speed -= speedValue; break;
		default:
			break;
	}}}
	data.pressed = press || keyboardData.pressChange;
	if(!press)
			keyboardData.pressChange = false;
}
else if(mouseButton.checked){
	data.speed += mouseData.yValue * 2.55;
	data.direction += mouseData.xValue;
	data.pressed = mouseData.pressed || mouseData.pressChange;
	if(!mouseData.pressed)
		mouseData.pressChange = false;
}
data.speed = Math.round(data.speed);
data.direction = Math.round(data.direction);
return data;
}

function transferData(){
var url = parseDataUrl();

// Send one check request every five otherwise empty requests
if(url.length == 5){
    emptyCounter++;
    if(emptyCounter < 5)
        return;
}
emptyCounter = 0;

var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200)
        handleResponse(this);
};
xhttp.open("GET", url, true);
xhttp.send();
}

function handleResponse(xhttp){
var content = xhttp.responseText.split(";");
const len = content.length;
for(i = 0; i < len; i++){
	var pair = content[i].split("=");
	switch(pair[0]){
	case "C":
		var status = !!+pair[1];
		cameraButton.checked = status;
		streamSlot.hidden = !status;
		if(status)
			stream.src = "https://upload.wikimedia.org/wikipedia/commons/b/b5/800x600_Wallpaper_Blue_Sky.png"
		break;
	case "R":
		reverseButton.checked = !!+pair[1];
		break;
	case "F":
		setDistanceConeSize("F", pair[1]);
		break;
	
	case "B":
		setDistanceConeSize("B", pair[1]);
		break;
	
	default:
		return;
	}
}}

function setDistanceConeSize(direction, distance){
var cone = "";
var coneText = "";
switch(direction){
case "F":
	cone = "distConeF";
	coneText = "ConeTextF";
	break;
case "B":
	cone = "distConeB";
	coneText = "ConeTextB";
	break;
default:
	return;
}
var size = "0px";
var text = "0cm";
var dist = parseInt(distance);
if(dist === 201){
	dist--;
	text = ">" + dist + "cm";
}
else
	text = dist + "cm";

size = Math.round(dist / 2) +"px";
document.getElementById(cone).style.width = size;
document.getElementById(cone).style.height = size;
document.getElementById(coneText).innerHTML = text;
}

)=====";
}
#endif