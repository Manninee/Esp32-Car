#ifndef MAINPAGE_hh
#define MAINPAGE_hh

namespace pages
{
const char mainPage[] PROGMEM = R"=====(
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Car</title>
<link rel="stylesheet" type="text/css" href="/mainCSS">
</head>
<body>
<div id="streamSlot">
	<div id="streamWrapper">
		<iframe id="stream"></iframe>
	</div>
</div>
<div class="row">
	<div class="column L">
		<p>Mouse pad</p>
		<div id="joystick-container">
			<div id="joystick">
				<div id="Xaxis"></div>
				<div id="Yaxis"></div>
			</div>
		</div>
		<p id="joystickValues">X value: 0, Y value: 0</p>
	</diV>
	<div class="column C">
		<div class="BaseCone F">
			<div class="distCone F" id="distConeF"></div>
			<p id="ConeTextF">0cm</p>
		</div>
		<br><br><br>
		<div class="BaseCone B">
			<div class="distCone B" id="distConeB"></div>
			<p id="ConeTextB">0cm</p>
		</div>
	</div>
	<div class="column R">
		<label class="switch">
			<input type="checkbox" id="keyboardSwitch">
			<span class="slider round"></span>
			<div>Keyboard steering</div>
		</label>
		<div class="slidercontainer">
			<input type="range" min="0" max="100" value="50" class="rangeSlider" id="sliderRange">
			<div>Speed:<span id="sliderValue">50</span>%</div>
		</div><hr>
		<label class="switch">
			<input type="checkbox" id="mouseSwitch">
			<span class="slider round"></span>
			<div>Mouse steering</div>
		</label>
		<br>
		<label class="switch">
			<input type="checkbox" id="cameraSwitch">
			<span class="slider round"></span>
			<div>Camera</div>
		</label>
		<br>
		<label class="switch">
			<input type="checkbox" id="reverseSwitch">
			<span class="slider round"></span>
			<div>Reverse camera</div>
		</label>
	</div>
</div>
<script type ='text/javascript' src='/mainJs'></script>
</body>
</html>
)=====";
}
#endif