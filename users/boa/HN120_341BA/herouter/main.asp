<!doctype html> 
<head>
<meta name="KEYWords" Content="mainPageFlag">
<meta http-equiv="Pragma" CONTENT="no-cache">
<meta http-equiv="Cache-Control" CONTENT="no-cache">
<meta http-equiv="Expires" CONTENT="-1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"> 
<title>和路由-中国移动</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/jquery.vegas.min.js"></script>
<script type="text/javascript" src="js/tinybox.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>

<link href="css/jquery.vegas.css" rel="stylesheet" type="text/css" />
<link href="css/and.css" rel="stylesheet" type="text/css" />
<link rel="stylesheet" type="text/css" href="css/icons.css" />
<link rel="stylesheet" type="text/css" href="css/style1.css" />

<!--[if IE 8]>
<link rel="stylesheet" type="text/css" href="css/style1_ie8.css" />
<![endif]-->
<script type="text/javascript">

var WIFIAuthMode = "<%getHeInfo("WIFIAuthMode");%>";
var wifi_off1='<%getHeInfo("wifi_off","wlan1")%>';
var wifi_off2='<%getHeInfo("wifi_off","wlan2")%>';
var EncrypType='<%getHeInfo("EncrypType");%>';	
var IEEE8021X  = '0';
var ConMode = "<%getHeInfo("ConMode")%>";
var WANIP = "<%getInfo("wan-ip");%>";
var isWanPortPlug = "<%getHeInfo("WanPortPlug");%>";
var isNeedWizard = "<%getHeInfo("isNeedWizard");%>";
var PingStatus = "<%getHeInfo("PingStatus");%>";
var newUrl;


$(function() {
  $.vegas({
    src:'images/bodybg.jpg'
  });
});
function resetMenu(){
 TINY.box.show({iframe:'sysstate.asp',boxid:'frameless',width:700,height:<%getHeInfo("sysstateHeight"); %>,fixed:false,maskopacity:40})
 
	 $("#bt-menu").removeClass("bt-menu-open");
	 $("#bt-menu").addClass("bt-menu-close");
	 
}
function advsetting(){
 TINY.box.show({iframe:'entconfig.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})
 
	$("#bt-menu").removeClass("bt-menu-open");
	 $("#bt-menu").addClass("bt-menu-close");
	
}

function reload()
{
  window.location.href="/herouter/login.asp";
}

function showupdating(){
	//setTimeout("TINY.box.show({html:'<div class=waiting2 style=display:;><h2>路由升级中，请稍候</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",3000);
TINY.box.show({html:'<div class=waiting2 style=display:;><h2>路由升级中，请稍候</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5});
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>请勿断电！升级中，请关闭本页面并在5分钟后使用和路由。</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",10000);
	setTimeout("TINY.box.hide();reload();", 85000);
}

function showLocalupdating(){
	//setTimeout("TINY.box.show({html:'<div class=waiting2 style=display:;><h2>路由升级中，请稍候</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",5000);
//	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>请勿断电！升级中，请关闭本页面并在1分钟后使用和路由。</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",10000);
TINY.box.show({html:'<div class=waiting3 style=display:;><h2>请勿断电！升级中，请等待1分钟后使用和路由。</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5});
	setTimeout("TINY.box.hide();reload();", 60000);
}

function restored_reload()
{
	window.location.replace("http://192.168.8.1/herouter/login.asp");
}

function showrestoring(){
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>重启中，无线网络将断开，请关闭页面并在1分钟后重新进入</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",1000);
	setTimeout("TINY.box.hide();restored_reload()", 70000);
}

function showchangepwd(){
	TINY.box.show({html:'<div class=waiting3 style=display:;><h2>密码修改成功，请重新登录</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5});
	setTimeout("TINY.box.hide();reload();", 3000);
}

function showChangeOpmode(){
	TINY.box.show({html:'<div class=waiting3 style=display:;><h2>网络模式修改成功！请等待30秒...</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5});
	setTimeout("TINY.box.hide();reload();", 20000);
}
function showChangeRtOpmode(){
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>无线网络将断开，请在30秒后重新进入页面</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})", 10000);
	setTimeout("TINY.box.hide();reload_main();", 40000);
}
function reload_main()
{
  window.location.href="/herouter/main.asp";
}

function showConfProcess(){
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>设置成功，无线网络将断开，请关闭页面并在30秒后重新进入</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",3000);
	setTimeout("TINY.box.hide();reload_main();", 20000);
}

function showWifiConfProcess(){
	setTimeout("TINY.box.show({html:'<div class=waiting2 style=display:;><h2>正在设置路由器，请稍候</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",3000);
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>设置成功，无线网络将断开，请关闭页面并在1分钟后重新进入</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",10000);
	setTimeout("TINY.box.hide();reload_main();", 30000);
}

function showSettingProcess(){
	setTimeout("TINY.box.show({html:'<div class=waiting2 style=display:;><h2>正在保存设置,请稍候</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",3000);
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>设置成功，无线网络将断开，请在30秒后重新进入本页面</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",10000);
	setTimeout("TINY.box.hide();reload_main();", 30000);
}

function  getNewLinkValue(value)
{
  newUrl = value;
}

function reloadNewMain()
{
	window.location.replace(newUrl);
}

function showEntProcess(){
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>正在保存设置，请稍候...</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5})",1000);
	//setTimeout("TINY.box.hide();Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 30);",30000);
	setTimeout("TINY.box.show({html:'<div class=waiting3 style=display:;><h2>设置成功，无线网络将断开，请在30秒后重新进入本页面</h2><div class=animation></div></div>',animate:false,close:false,boxid:'',top:5});",2000);
	setTimeout("reloadNewMain();", 30000);
}

function openwifipage()
{
	TINY.box.show({iframe:'wifisetting1.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})		
}
function openMACpage()
{
	TINY.box.show({iframe:'wirelessMAC.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})		
}
function openQuick()
{
	TINY.box.show({iframe:'QuickSetUp.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})
}
$(document).ready(function(){
	
	//wifi enable
	if(wifi_off1 == 0 || wifi_off2==0){
		  
          $("#wifistate").addClass("wifiok");  
	}else{
		  $("#wifistate").addClass("wificlose");
	}

	//internet ico
  if (PingStatus != "pass")	{
  	  $("#internet").addClass("interr");  
  }else{
      $("#internet").removeClass("interr");	
  }
	
	if(isNeedWizard == 1)
	{
      TINY.box.show({iframe:'QuickSetUp.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})
	}
	document.getElementById("qrcode").style.visibility = "hidden";
	document.getElementById("qrcode").style.display = "none";
});

function reboot_alert()
{
    if (confirm("您确定需要重启吗?")){ 
        document.reboot.submit(); 
        showrestoring();
    }else{ 
    	  /*do nothing*/
    }
}

</script>
</head>


<body>
<div class="head">
	<div class="logo"></div>
	<div class="menu">
		<ul>
			<li class="line" onClick="TINY.box.show({iframe:'opmode.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})"><strong><span class="qc"><b>模式设置</span></strong></li>
			<li class="line" onClick="TINY.box.show({iframe:'netdiags.asp',boxid:'frameless',width:750,height:380,fixed:false,maskopacity:40})"><strong><span class="qc"><b>网络诊断</span></strong></li>
			<li class="line" onClick="TINY.box.show({iframe:'changepassword.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})"><strong><span class="qc"><b>修改密码</span></strong></li>
			<li class="line" onClick="TINY.box.show({iframe:'QuickSetUp.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})" ><strong><span class="qc"><b>设置向导</b></span></strong></li>
			<li class="line"
			onClick="TINY.box.show({iframe:'ShowSysLog.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})"
			><strong><span class="qc"><b>系统日志</b></span></strong></li>
			<form method="post" name="reboot" target="submitFrame" id="reboot" action="/boafrm/heUiReboot">
			<input type="hidden" name="LoadDefault" value="Load+Default"/>
			<li><strong><span class="reset" onclick="javascript:reboot_alert();"><b>重启路由</b></span></strong></li>
			</form>
			<!--for reboot submint-->
			<iframe style="width:0; height:0; margin-top:-10px; visibility:hidden; display:none;" name="submitFrame" src="about:blank"></iframe>
		</ul>
	</div>
</div>
<div class="mainsetting">
	<div class="ccbg">
		<div class="section1">
			<div id="internet" class="intstate">
				<div class="intok "></div>
				<div class="btnarea">
					<div class="btnint" id="intbtn" style="position:absolute;" onClick="TINY.box.show({iframe:'intconfig.asp',boxid:'frameless',width:700,height:420,fixed:false,maskopacity:40})" >
						<b></b><span>互联网</span><strong></strong>
					</div>
				</div>
				<div class="stateline"></div>
			</div>
			<div class="wifistate" id="wifistate">
				<div class="wifiok"></div>
				<div class="btnarea">
					<div class="btnint" id="wifibtn" onClick="TINY.box.show({iframe:'wifisetting1.asp',boxid:'frameless',width:700,height:510,fixed:false,maskopacity:40})">
						<b></b><span>无线</span><strong></strong>
					</div>
				</div>
				<div class="stateline"></div>
			</div>
			
		</div>
		<div class="section2">
			<div class="linkstate">
				<div class="btnarea">
					<div class="btnint"  onclick="TINY.box.show({iframe:'devicemag.asp',boxid:'frameless',width:700,height:530,fixed:false,maskopacity:40})">
						<span><em><% getHeInfo("wireless_dhcp_client_num"); %></em>台无线设备连接到和目路由器</span><strong></strong>
					</div>
				</div>
			</div>
		</div>
</div>

<div class="testmusic" id="bottomNav" >
	<div id="qrcode" class="qrcode"><img src="images/qrcode.png" /><p>手机客户端下载</p></div>
  <div class="footer">
  <a href="http://www.andluyou.com">官方网站</a><span>|</span>物理地址:<%getHeInfo("he_wan_mac");%><span>|</span>版本:<%getInfo("fwVersion");%></div>
</div>

<nav id="bt-menu" class="bt-menu">
	<ul>
		<a href="#" onClick="resetMenu();" ><span style="background:url(images/sysstate.png) no-repeat center center;  display:block; width:90px; height:80px;"></span></a>
		<a href="#" onClick="advsetting();" ><span style="background:url(images/advsetting.png) no-repeat center center; display:block; width:90px; height:80px;"></span></a>
	</ul>
</nav>

</body>
<script src="js/classie.js"></script>
<script src="js/borderMenu.js"></script>
</html>
