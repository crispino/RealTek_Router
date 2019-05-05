<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/jquery.vegas.min.js"></script>
<script type="text/javascript" src="js/tinybox.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<link href="css/jquery.vegas.css" rel="stylesheet" type="text/css" />
<link href="css/and.css" rel="stylesheet" type="text/css" />
<style type="text/css">
<!--
body{}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:510px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:10px 15px;}
.int h1{ font-size:18px; line-height:42px; border-bottom:1px #d3d3d3 solid; height:50px;}
.int h1 strong{ float:left; padding-right:20px; color:#676767;}
.int b{display:block; height:42px; cursor:pointer; padding:0 15px;font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:42px;}
.int b:hover{ background:#56bbcb;}
.int .rb{ float:right; margin-left:10px;background:#99e4f0; }
.int .cancel{background:#99e4f0; }
.spc5{ margin-right:5px;}
.overview{ padding-top:5px; height:42px;}
.int .unselect{ background:#d2d2d2;}
.data{ float:right; color:#a3a3a3; width:332px;}
.moddata{ float:left; width:170px; height:42px;}
.moddata .txt{float:left; font-size:12px; height:42px; line-height:21px;}
.fh{float:left; font-size:42px; line-height:36px; height:42px; text-align:center; width:25px;}
.datashow{float:left; background:url(images/upanddown.png) no-repeat left center; height:39px; padding-left:20px; width:68px; color:#919191; line-height:18px; padding-top:3px;}
.alldata{ float:right; width:150px; height:42px; padding-left:12px; background:url(images/sls.png) no-repeat left center;}
.alldata .alltxt{float:left; line-height:42px; font-size:12px;}
.tabarrow{ height:9px; background:url(images/arrowtab.png) no-repeat 45px 5px; padding-top:5px;}
.tabarrow2{ height:9px; background:url(images/arrowtab.png) no-repeat 150px 5px; padding-top:5px;}
.datalist{ background:#fff; padding:7px;height:335px; overflow:hidden;}
.title{ height:35px; background:#f4f4f4;}
.title ul{}
.title li{ float:left; line-height:35px ; font-size:14px; color:#666;}
.title li span{ padding-left:35px;}
.l1{ width:170px;}
.l2{ width:126px;}
.l3{ width:180px;}
.l4{ width:100px;}
.l5{ width:80px;}
.l6{ width:100px; text-align:left;}
.data_detail{ }
.data_detail ul{ height:40px; padding:5px 0; border-bottom:1px #f1f1f1 solid;}
.data_detail li { float:left; height:40px; line-height:40px; color:#919191;}
.data_detail li span{ padding-left:35px; height:28px; margin:6px 0; display:block; line-height:28px; background:url(images/device.png) no-repeat }
.data_detail li .device1{ background-position:0 0;}
.data_detail li .device2{ background-position:0 -28px;}
.data_detail li .device3{ background-position:0 -56px;}
.data_detail li .device4{ background-position:0 -84px;}
.data_detail li .device5{ background-position:0 -114px;}
.data_detail li .device6{ background-position:0 -140px;}
.data_detail li p{ line-height:20px;}
.data_detail li b{ font-size:12px; line-height:20px; height:20px;}
.data_detail .l4{ background:url(images/upanddown.png) no-repeat 0 center; padding-left:20px; width:80px; line-height:20px;}
.data_detail li i{ font-style:normal; line-height:16px;}
.data_detail li strong{ display:block;width:40px; height:40px; font-size:14px; color:#77d1df; text-align:center; background:#f6f6f6; font-weight:normal; cursor:pointer;}
.data_detail li strong:hover{ background:#eaeaea;}
.data_detail li em{ font-style:normal; height:40px; width:70px; font-size:14px; background:#f6f6f6; cursor:pointer; display:block; text-align:center; color:#ff8484;}
.data_detail li em:hover{background:#eaeaea;}
.countinfo{ height:50px; line-height:50px;}
.countinfo ul{}
.countinfo li{ color:#008edb; float:left; font:14px;}
.countinfo li em{ font-size:18px; font-weight:bold; font-style:normal;}
.countinfo .pagechange{ font-size:18px; float:right; padding-top:15px; }
.countinfo strong{ font-weight:normal; margin-left:20px; cursor:pointer;}
.countinfo .nextpage{ background:url(images/randl.png) no-repeat right 0; padding-right:21px; line-height:21px;}
.countinfo .prepage{ background:url(images/randl.png) no-repeat left -21px; padding-left:21px; line-height:21px;}
.info{ padding:10px 0; height:26px;width 40%}
.info span{ float:left; line-height:26px; color:#666;}
.info b{ float:left; line-height:26px; height:26px; margin:0 10px; font-size:16px;}
.save {  float:right; line-height:50px; font-size:26px;height:50px;margin:0 10px;}
-->
</style>
<script type="text/javascript">
	var startnum = 1;
	var endnum   = 5;
function NextDisplay(Clientnum)
{
	var ID= new Array();
	var i = startnum;
	
	if(Clientnum >= 5 && endnum <Clientnum)
	{
		for(i;(i<=endnum+5)&&(i<=Clientnum) ;i++)
		{
				ID[i] ="display"+i	;
				if(i <= endnum)
				{
					document.getElementById(ID[i]).style.display = "none";
					startnum++;
				}
				else
				{	
					document.getElementById(ID[i]).style.display = "";
				}
		}
		endnum = i -1;
	}
}

function PreDisplay(Clientnum)
{
	var ID= new Array();
	var i = endnum < Clientnum ?endnum:Clientnum;
	if(startnum > 5&& endnum > 5)
	{
		for(i;i >= startnum-5;i--)
		{
				ID[i] ="display"+i	;
				if(i >= startnum)
				{
					document.getElementById(ID[i]).style.display = "none";
					endnum--;
				}
				else
					document.getElementById(ID[i]).style.display = "";
		}
		startnum = i+1;
	}
}

function showwifi()
{
	window.parent.openwifipage();
	
}
function showMAC()
{
	window.parent.openMACpage();
	
}
 
function showWirelessDev()
{
	$("#connectlist").html("已连接无线设备列表");
	$("div#wireless").css({"display":"block"});
	$("div#wired").css({"display":"none"});
	
	$("div#wirelesslist").css({"display":"block"});
	$("div#wiredlist").css({"display":"none"});
	$("div.tabarrow").css({"display":"block"});
	$("div.tabarrow2").css({"display":"none"});
	$("b.rb").css("display", "block");
}

function showWiredDev()
{
	$("#connectlist").html("已连接有线设备列表");
	$("div#wireless").css({"display":"none"});
	$("div#wired").css({"display":"block"});
	
	$("div#wirelesslist").css({"display":"none"});
	$("div#wiredlist").css({"display":"block"});
	$("div.tabarrow").css({"display":"none"});
	$("div.tabarrow2").css({"display":"block"});
	$("b.rb").css("display", "none");
}

function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	http_request.send("mac="+content);
}

function parseAllData(str){
//alert("已强制断开");	
location.reload();	
}

function disconnectbymacfilterHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
		} else {
			//alert('There was a problem with the request.'+http_request.readyState+':'+http_request.status);
		}
	}	

}
function disconnect(macid,emid,Ocemid)
{
	var macstr	= document.getElementById(macid).innerHTML;
	makeRequest("/boafrm/disconnectbymacfilter", macstr, null);
	document.getElementById(emid).style.display = "none";
	document.getElementById(Ocemid).style.display = "";
	Alert("正在强制断开，请稍候", 10);	
	setTimeout("Alert('设置成功，无线网络将断开，请关闭页面并在30秒后重新进入', 20)", 10100);
	setTimeout("location.reload();", 30200);	
}

function doratelimit(macid)
{
	Alert("尚未开放，敬请留意后续软件更新", 3);
}

</script>
</head>

<body>
<form method=post name="quickSet" id="quickSet" action="/boafrm/QuickSetUpSave" >
	<input type="hidden" name="save" id="save" value="true"/>
<div class="int">
	<h1><strong id="connectlist">已连接无线设备列表</strong><b class="cancel" Onclick="location.reload(); ";>刷新设备列表</b><!--<b class="rb" onClick="showwifi();"; >设置无线密码</b>--><b class="rb" onClick="showMAC();"; >黑名单管理</b></h1>
	 <div class="overview">
	 	<div id="wireless" style="display:block">
			<b class="spc5" onclick="showWirelessDev()">无线设备</b>
			<b class="unselect" onclick="showWiredDev()">有线设备</b>
		</div>
		<div id="wired" style="display:none">
			<b class="unselect" onclick="showWirelessDev()">无线设备</b>
			<b style="margin-left:5px" onclick="showWiredDev()">有线设备</b>
		</div>
		<!--
		<div class="data">
			<div class="moddata">
				<div class="txt">当前模式<br />总流量</div>
				<div class="fh">:</div>
				<div class="datashow">
					99999 KB/S<br />
					10KB/S
				</div>
			</div>
			<div class="alldata">
				<div class="alltxt">总流量</div>
				<div class="fh">:</div>
				<div class="datashow">
					128 KB/S<br />10KB/S
				</div>
			</div>
		</div>
		-->
	</div>
	<div class="tabarrow" style="display:block"></div>
	<div class="tabarrow2" style="display:none"></div>
	<div id="wirelesslist" class="datalist" style="display:block">
		<div class="title">
			<ul>
				<li class="l1"><span>MAC地址</span></li>
				<li class="l2">IP</li>
				<li class="l3">设备名称</li>
				<li class="l6">设备操作</li>
			</ul>
		</div>
		
			   <div class='data_detail'>
			   <% dhcpWirelessClientList(); %>
	  </div>
	  <div class='countinfo'>
	  	<ul>
				<li>接入无线设备总数量：<em><% getHeInfo("wireless_dhcp_client_num"); %></em></li>
	      <li class='pagechange'><strong class='nextpage'  Onclick='NextDisplay(0);'>下一页</strong></li>
	      <li class='pagechange'><strong class='prepage'   Onclick='PreDisplay(0);'>上一页</strong></li>
	    </ul>
	   </div>

<!--
		<div class="data_detail">
			<ul id = "display1" style="display:">
				<li class="l1"><span class="device1">130:85:A9:5E:CD:AB </span></li>
				<li class="l2">192.168.199.231</li>
				<li class="l3"><p>android-db3e3f2bbac25f70</p><b>改名</b></li>
				<li class="l4">128 KB/S<br />10KB/S</li>
				<li class="l5"><strong>无</strong></li>
				<li class="l6"><em>强制断开</em></li>
			</ul>
			<ul id = "display2" style="display:">
				<li class="l1"><span class="device4">30:85:A9:5E:CD:AB </span></li>
				<li class="l2">192.168.199.231</li>
				<li class="l3"><p>android-db3e3f2bbac25f70</p><b>改名</b></li>
				<li class="l4">128 KB/S<br />10KB/S</li>
				<li class="l5"><strong>无</strong></li>
				<li class="l6"><em>强制断开</em></li>
			</ul>
			<ul>
				<li class="l1"><span class="device3">30:85:A9:5E:CD:AB </span></li>
				<li class="l2">192.168.199.231</li>
				<li class="l3"><p>android-db3e3f2bbac25f70</p><b>改名</b></li>
				<li class="l4">128 KB/S<br />10KB/S</li>
				<li class="l5"><strong>无</strong></li>
				<li class="l6"><em>强制断开</em></li>
			</ul>
			<ul>
				<li class="l1"><span class="device2">30:85:A9:5E:CD:AB </span></li>
				<li class="l2">192.168.199.231</li>
				<li class="l3"><p>android-db3e3f2bbac25f70</p><b>改名</b></li>
				<li class="l4">128 KB/S<br />10KB/S</li>
				<li class="l5"><strong>无</strong></li>
				<li class="l6"><em>强制断开</em></li>
			</ul>
			<ul>
				<li class="l1"><span class="device5">30:85:A9:5E:CD:AB </span></li>
				<li class="l2">192.168.199.231</li>
				<li class="l3"><p>android-db3e3f2bbac25f70</p><b>改名</b></li>
				<li class="l4">128 KB/S<br />10KB/S</li>
				<li class="l5"><strong>无</strong></li>
				<li class="l6"><em>强制断开</em></li>
			</ul>
		
	  </div>
	  <div class="countinfo">
	  	<ul>
				<li>无线设备总数量：<em>6</em></li>
				<li class="pagechange"><strong class="nextpage" Onclick="NextDisplay();">下一页</strong></li>
				<li class="pagechange"><strong class="prepage"  Onclick="PreDisplay();"  >上一页</strong></li>
			</ul>
	  </div>
-->		  
		<div class="clean"></div>
	</div>
	<div id="wiredlist" class="datalist" style="display:none">
		<div class="title">
			<ul>
				<li class="l1"><span>MAC地址</span></li>
				<li class="l2">IP</li>
				<li class="l3">设备名称</li>
				<li class="l6">设备操作</li>
			</ul>
		</div>		
			   <div class='data_detail'>
			   <% dhcpWiredClientList(); %>
	  </div>
	  <div class='countinfo'>
	  	<ul>
				<li>接入有线设备总数量：<em><% getHeInfo("wired_dhcp_client_num"); %></em></li>
	      <li class='pagechange'><strong class='nextpage'  Onclick='NextDisplay(0);'>下一页</strong></li>
	      <li class='pagechange'><strong class='prepage'   Onclick='PreDisplay(0);'>上一页</strong></li>
	    </ul>
	   </div>
 
		<div class="clean"></div>
	</div>	
</div>
</form>
</body>
</html>
