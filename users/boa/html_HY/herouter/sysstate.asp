<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:<%getHeInfo("sysstateHeight"); %>px; width:700px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; border-bottom:1px #d3d3d3 solid;}
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.state{ padding:15px 0; color:#757575;}
.state1{ float:left; width:300px;; padding:10px ;}
.state2{ float:left; width:350px;  padding:10px ;}
.basicinfo{ padding:5px 0px 20px 0; border-bottom:1px #ddd solid;}
.basicinfo ul{}
.basicinfo li{ line-height:28px; font-size:12px; height:28px;}
.basicinfo li b{ width:80px; display:block; float:left;}
.basicinfo li span{ display:block; float:left;}
.intstate{ padding-top:15px;}
.intstate h3{ font-size:16px; color:#3aaec1; line-height:28px;}
.intstate ul{}
.intstate li{line-height:28px; font-size:12px; height:28px;}
.intstate li b{width:70px; display:block; float:left;}
.intstate li span{display:block; float:left;}
.intstate li a,.intstate li a:visited{ display:block; height:28px; padding:0 12px; background:#77d1df; float:left; color:#fff; margin-left:10px; }
.intstate li a:hover,.intstate li a:active{ text-decoration:none; background:#56bbcb;}

.entstate{  padding-bottom:10px; }
.entstate h3{ font-size:16px; color:#3aaec1; line-height:28px;padding-top:10px;}
.entstate ul{}
.entstate li{line-height:28px; font-size:12px; height:28px;}
.entstate li b{width:70px; display:block; float:left;}
.entstate li span{display:block; float:left;}
.entstate li a,.intstate li a:visited{ display:block; height:28px; padding:0 12px; background:#77d1df; float:left; color:#fff; margin-left:10px; }
.entstate li a:hover,.intstate li a:active{ text-decoration:none; background:#56bbcb;}

.wifistate{ }
.wifistate h3{ font-size:16px; padding-top:10px; color:#3aaec1; line-height:28px;}
.wifistate ul{}
.wifistate li{line-height:28px; font-size:12px; height:28px;}
.wifistate li b{width:70px; display:block; float:left;}
.wifistate li span{display:block; float:left;}
.wifistate li a,.intstate li a:visited{ display:block; height:28px; padding:0 12px; background:#77d1df; float:left; color:#fff; margin-left:10px; }
.wifistate li a:hover,.intstate li a:active{ text-decoration:none; background:#56bbcb;}


-->
</style>
<script type="text/javascript">
function $$$$$(_sId){
 return document.getElementById(_sId);
 }
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }
function pick(v,targetid,abc) {
	document.getElementById(targetid).value=v;
	hide(abc);
}
var switchcode=0;
$(document).ready(function(){
	$("#switch").click(function(){
		if(switchcode==0){
			 $("#state").addClass("switchoff");
			 switchcode=1;
			}else{
			$("#state").removeClass("switchoff");
			switchcode=0;
			}
	})
	


	
});

var ChannelSeleted1 ='<%getHeInfo("ChannelSeleted","wlan1");%>';
var ChannelSeleted2 ='<%getHeInfo("ChannelSeleted","wlan2");%>';
var WIFIAuthMode1 = '<%getHeInfo("WlanAuthMode","wlan1");%>';
var WIFIAuthMode2 = '<%getHeInfo("WlanAuthMode","wlan2");%>';
var rp_WIFIAuthMode1 = '<%getHeInfo("WlanAuthMode","wlan1-vxd");%>';
var rp_WIFIAuthMode2 = '<%getHeInfo("WlanAuthMode","wlan2-vxd");%>';
function aa(){
	parent.TINY.box.hide();
}
function PageInit()
{
	var wifi_off1 = '<%getHeInfo("wifi_off","wlan1");%>';
	var wifi_off2 = '<%getHeInfo("wifi_off","wlan2");%>';
	var Autochannel1 = '<%getHeInfo("Autochannel","wlan1");%>';
	var Autochannel2 = '<%getHeInfo("Autochannel","wlan2");%>';
	var CurrentChannel1 = '<%getHeInfo("CurrentChannel","wlan1");%>'; 
	var CurrentChannel2 = '<%getHeInfo("CurrentChannel","wlan2");%>'; 
	var wlan_num = '<% getIndex("wlan_num"); %>';
	var rp_enabled1 = '<%getHeInfo("isRepeaterEnabled","wlan1");%>';
	var rp_enabled2 = '<%getHeInfo("isRepeaterEnabled","wlan2");%>';
	var rp_connect_state1 = '<%getHeInfo("repeaterState","wlan1-vxd");%>';
	var rp_connect_state2 = '<%getHeInfo("repeaterState","wlan2-vxd");%>';
	
	if(wifi_off1 == '0')
	{
		document.getElementById("wifiup1").style.display = "block";
		document.getElementById("wifidown1").style.display = "none";
	}
	else
	{
		document.getElementById("wifiup1").style.display = "none";
		document.getElementById("wifidown1").style.display = "block";
	}
	if(wifi_off2 == '0')
	{
		document.getElementById("wifiup2").style.display = "block";
		document.getElementById("wifidown2").style.display = "none";
	}
	else
	{
		document.getElementById("wifiup2").style.display = "none";
		document.getElementById("wifidown2").style.display = "block";
	}

	if(ChannelSeleted1 == 'AutoSelect' || Autochannel1 == '1')
	{
		wirelesschannel1.innerHTML = CurrentChannel1 + " (自动选择)";
		document.getElementById("manualselect1").style.display = "none";
		document.getElementById("autoselect1").style.display = "block";
	}
	else
	{
		document.getElementById("manualselect1").style.display = "block";
		document.getElementById("autoselect1").style.display = "none";
	}
	if(ChannelSeleted2 == 'AutoSelect' || Autochannel2 == '1')
	{
		wirelesschannel2.innerHTML = CurrentChannel2 + " (自动选择)";
		document.getElementById("manualselect2").style.display = "none";
		document.getElementById("autoselect2").style.display = "block";
	}
	else
	{
		document.getElementById("manualselect2").style.display = "block";
		document.getElementById("autoselect2").style.display = "none";
	}
	if(rp_enabled1==1 && wlan_num>=1){	
		document.getElementById("wifiVxdStatus1").style.display = "block";	
		if(rp_connect_state1 == 'Connected'){
			document.getElementById("wifivxdConnectOk1").style.display = "block";
			document.getElementById("wifivxdConnectFail1").style.display = "none";
		}else{
			document.getElementById("wifivxdConnectOk1").style.display = "none";
			document.getElementById("wifivxdConnectFail1").style.display = "block";
		}
	}else{
		document.getElementById("wifiVxdStatus1").style.display = "none";		
	}

	if(rp_enabled2==1 && wlan_num>=2){	
		document.getElementById("wifiVxdStatus2").style.display = "block";	
		if(rp_connect_state2 == 'Connected'){
			document.getElementById("wifivxdConnectOk2").style.display = "block";
			document.getElementById("wifivxdConnectFail2").style.display = "none";
		}else{
			document.getElementById("wifivxdConnectOk2").style.display = "none";
			document.getElementById("wifivxdConnectFail2").style.display = "block";
		}
	}else{
		document.getElementById("wifiVxdStatus2").style.display = "none";		
	}	
	
	if(wlan_num>=1){
		document.getElementById("wifiStatus1").style.display = "block";					
	}
	else{
		document.getElementById("wifiStatus1").style.display = "none";		
	}
	if(wlan_num>=2){
		document.getElementById("wifiStatus2").style.display = "block";		
	}
	else{
		document.getElementById("wifiStatus2").style.display = "none";
	}
}
</script>
</head>

<body onLoad="PageInit()">
<div class="int">
  <h1>系统状态</h1>
  <div class="state">
    <div class="state1">
      <div class="basicinfo">
        <ul>
          <li><b>开机时长</b><span>
            <% getInfo("uptime"); %>
          </span></li>
        </ul>
        <ul>
          <li><b>SN</b><span style="width:130px;">
            <%getHeInfo("STBID");%>(保修专用)
           </span></li>
        </ul>
        <div class="clean"></div>
      </div>
      <div class="intstate">
        <h3>互联网</h3>
        <ul>
          <li><b>上网方式</b><span>
            <%getHeInfo("ConMode");%>
          </span></li>
          <li><b>IPv4地址</b><span>
            <% getInfo("wan-ip"); %>
            /
            <% getInfo("wan-mask"); %>
          </span><!--<a href="#">修改</a>--></li>
          <li><b>MAC地址</b><span>
            <% getInfo("wan-hwaddr"); %>
          </span></li>
          <li><b>DNS地址</b><span>
             <%getHeInfo("wan-dnsaddr");%>
          </span></li>
          <li><b>网关地址</b><span>
            <% getInfo("wan-gateway"); %>
          </span></li>
        </ul>
      </div>
      <div class="entstate">
        <h3>局域网</h3>
        <ul>
          <li><b>IPv4地址</b><span>
            <% getInfo("ip"); %>
            /
            <% getInfo("mask"); %>
          </span><!--<a href="#">修改</a>--></li>
          <li><b>MAC地址</b><span>
            <% getInfo("hwaddr"); %>
            </span></li>
        </ul>
      </div>
    </div>
    <div class="state2">
      <div class="wifistate">
      <div id="wifiStatus1" style="display:block">
        <h3><% getHeInfo("wirelessName", "wlan1"); %></h3>
        <ul>
          <li id="wifiup1"><b>无线状态</b><span >工作中</span><!--<a href="#">修改</a>--></li>
          <li id="wifidown1"><b>无线状态</b><span>已关闭</span><!--<a href="#">修改</a>--></li>
          <li><b>网络名称</b><span>
            <%getHeInfo("SSID","wlan1");%>
          </span></li>
		  <li id="wifiopen1"><b>安全类型</b><span><%getHeInfo("WlanAuthMode","wlan1");%></span></li>
          <li id="manualselect1"><b>无线信道</b><span><%getHeInfo("CurrentChannel","wlan1");%>（手动选择）</span></li>
          <li id="autoselect1"><b>无线信道</b><span id=wirelesschannel1></span></li>
        </ul>
        </div>

        <div id="wifiVxdStatus1" style="display:block">
        <h3><% getHeInfo("wirelessName", "wlan1-vxd"); %></h3>
        <ul>          
          <li><b>网络名称</b><span>
            <%getHeInfo("repeaterSSID_drv","wlan1-vxd");%>
          </span></li>
		  <li id="wifivxdopen1"><b>安全类型</b><span><%getHeInfo("WlanAuthMode","wlan1-vxd");%></span></li>
          <li id="wifivxdConnectOk1"><b>连接状态</b><span>已连接</span></li>
          <li id="wifivxdConnectFail1"><b>连接状态</b><span>未连接</span></li>
        </ul>
        </div>
        
		<div id="wifiStatus2" style="display:block">
        <h3><% getHeInfo("wirelessName", "wlan2"); %></h3>
        <ul>
          <li id="wifiup2"><b>无线状态</b><span >工作中</span><!--<a href="#">修改</a>--></li>
          <li id="wifidown2"><b>无线状态</b><span>已关闭</span><!--<a href="#">修改</a>--></li>
          <li><b>网络名称</b><span>
            <%getHeInfo("SSID","wlan2");%>
          </span></li>
		  <li id="wifiopen2"><b>安全类型</b><span><%getHeInfo("WlanAuthMode","wlan2");%></span></li>
          <li id="manualselect2"><b>无线信道</b><span><%getHeInfo("CurrentChannel","wlan2");%>（手动选择）</span></li>
          <li id="autoselect2"><b>无线信道</b><span id=wirelesschannel2></span></li>
        </ul>
        </div>

        <div id="wifiVxdStatus2" style="display:block">
        <h3><% getHeInfo("wirelessName", "wlan2-vxd"); %></h3>
        <ul>          
          <li><b>网络名称</b><span>
            <%getHeInfo("repeaterSSID_drv","wlan2-vxd");%>
          </span></li>
		  <li id="wifivxdopen2"><b>安全类型</b><span><%getHeInfo("WlanAuthMode","wlan2-vxd");%></span></li>
          <li id="wifivxdConnectOk2"><b>连接状态</b><span>已连接</span></li>
          <li id="wifivxdConnectFail2"><b>连接状态</b><span>未连接</span></li>
        </ul>
        </div>        
      </div>
    </div>
    <div class="clean"></div>
  </div>
</div>
</body>
</html>
