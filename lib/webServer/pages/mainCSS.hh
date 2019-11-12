#ifndef MAINCSS_hh
#define MAINCSS_hh

namespace pages
{
const char mainPageCSS[] PROGMEM = R"=====(
body{background-color: lightblue;}

.switch{
position: relative;
display: inline-block;
width: 60px;
height: 34px;
margin-top: 5px;
margin-bottom: 5px;
}

.switch input{
opacity: 0;
width: 0;
height: 0;
}

.switch div{
display: inline-block;
position: absolute;
left: 70px;
top: 25%;
width: auto;
white-space: nowrap;
}

.slider{
position: absolute;
cursor: pointer;
top: 0;
left: 0;
right: 0;
bottom: 0;
background-color: #ccc;
-webkit-transition: .4s;
transition: .4s;
}

.slider:before{
position: absolute;
content: "";
height: 26px;
width: 26px;
left: 4px;
bottom: 4px;
background-color: White;
-webkit-transition: .4s;
transition: .4s;
}

input:checked + .slider{background-color: #2196F3;}

input:focus + .slider{box-shadow: 0 0 1px #2196F3;}

input:checked + .slider:before{
-webkit-transform: translateX(26px);
-ms-transform: translateX(26px);
transform: translateX(26px);
}

.slider.round{border-radius: 34px;}

.slider.round:before{border-radius: 50%;}

.slidercontainer{
width: auto;
margin-top: 5px;
margin-bottom: 5px;
text-align: center;
}

.slidercontainer div{
margin-top: 2px;
font-weight: bold;
color: Black;
font-size: 120%;
}

.rangeSlider{
-webkit-appearance: none;
width: 100%;
height: 15px;
border-radius: 5px;
background: #d3d3d3;
outline: none;

opacity: 0.5;
-webkit-transition: .2s;
transition: opacity .2s;
}

.rangeSlider:hover {opacity: 7;}

.rangeSlider::-webkit-slider-thumb{
-webkit-appearance: none;
appearance: none;
width: 25px;
height: 25px;
border-radius: 50%;
background: Lime;
cursor: pointer;
}

.rangeSlider::-moz-range-thumb{
width: 25px;
height: 25px;
border-radius: 50%;
background: #4CAF50;
cursor: pointer;
}

*{box-sizing: border-box;}

.row{
display: table;
table-layout: auto;
width: 100%;
}

.column{
padding: 5px;
border-style: solid;
display: table-cell;
vertical-align: top;
background-color:LightSlateGrey;
}

.column.L{
border-width:2px 0px 2px 2px;
text-align:center;
}
.column.R{border-width:2px 2px 2px 0px;}

.column.C{
border-width:2px 2px 2px 2px;
text-align:center;
}

@media only screen and (max-width: 820px) {.column{
display: block;
width: 100%;
}
.column.L{border-width:2px 2px 0px 2px;}
.column.R{border-width:0px 2px 2px 2px;}
}

.BaseCone{
 width:100px; 
 height:100px; 
 background: DarkGray;
 border-radius: 90px 0 0 0;
 text-align: center;
 margin: auto;
}

.BaseCone.F{transform: rotate(45deg);}
.BaseCone.B{transform: rotate(-135deg);}

.BaseCone p{
width: 100px;
font-weight: bold;
color: Black;
font-size: 150%;
position: absolute;
display: inline-block;
}

.BaseCone.F p{transform: rotate(-45deg) translateX(-45px) translateY(-10px);}
.BaseCone.B p{transform: rotate(+135deg) translateX(45px) translateY(10px);}

.distCone{
 background: MediumSpringGreen	;
 border-radius: 90px 0 0 0;
 position: absolute;
 right: 0px;
 bottom: 0px;
}

.distCone.F{width:0%; height:0%;}
.distCone.B{width:0%; height:0%;}

#streamSlot{
width: 100%;
height: auto;
background: LightSlateGrey;
border-style: solid;
border-width:2px 2px 0px 2px;
max-height: 602px;
overflow: hidden;
}

#streamWrapper{
height: 0;
padding-top: 75%;
position: relative;
background: LightSlateGrey;
}

#stream{
border-style: none;
position: absolute;
top: 0;
left: 0;
width: 100%;
height: 100%;
}

#joystick-container {
border: solid 2px #000;
display: inline-block;
background: red;
}

#joystick {
width: 200px;
height: 200px;
cursor: crosshair;
position: relative;
touch-action: none;
}

#Xaxis{
width: 100%;
border: 1px solid black;
position: absolute;
top: 50%;
}

#Yaxis{
height: 100%;
border: 1px solid black;
position: absolute;
left: 50%;
}
)=====";
}
#endif