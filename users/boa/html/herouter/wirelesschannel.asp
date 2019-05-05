<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fff url(images/popbg.gif) repeat-x 0 -2px; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; }
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.mainsetting{ padding:30px; }
.mainsetting h2{ font-size:14px; line-height:38px;}
.c1{ height:48px;}
.c1 b{ display:block; height:48px; cursor:pointer; width:190px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.c1 a{ background:url(images/help.png) no-repeat center center; display:block; float:left; height:48px; width:30px;}
.c1 b:hover{ background:#56bbcb;}
.c1 span{  font-size:12px; color:#919191; padding:0 0 0 15px; float:left; font-weight:normal; line-height:48px;}
.c1 .err{ background:#e71b1b;}
.c1 .err:hover{ background:#da1515;}
.mainsetting h3{ font-size:24px; font-weight:normal; line-height:50px; color:#29b8e4;}
.c2{ border-top:1px #e6e6e6 solid; margin-top:80px; line-height:30px;}

.cfg{ padding:15px 0; height:330px;}
.cfgleft{ float:left; width:185px; height:330px; background:url(images/cfgleftbg.png) repeat-y;}
.cfgleft ul{}
.cfgleft li{ cursor:pointer; height:40px; line-height:40px; font-size:14px; color:#666; border-left:4px #dfdfdf solid ; padding-left:12px;}
.cfgleft li a,.cfgleft li a:visited{color:#666; display:block; height:40px;width:170px; }
.cfgleft li a:hover,.cfgleft li a:active{ text-decoration:none;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
.cfgleft li.on a{color:#1e9ec5;}
.cfgright{ float:right; width:470px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:50px; font-size:14px;}
.cfgright .spheight{ height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.radiobox{ margin-top:-2px;}
.inp1 {height:26px; border:1px #d2d2d2 solid; width:160px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.subpart{ height:48px; padding-left:120px; padding-top:25px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:110px; background:#99e4f0; }



.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
.fix{ position:absolute;}
.bm {margin-top:30px;position:absolute;}
.ganyin{ position:absolute; height:30px;cursor:pointer; border:1px #333 solid; z-index:100001; background:#0099CC;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0;}
.bm ul{ padding:1px 0 0 0; border-bottom:1px #eee solid; border-left:1px #eee solid; border-right:1px #eee solid; }
.bm li{ color:#0781d9; line-height:15px; padding-left:10px; height:15px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}
-->
</style>
<script type="text/javascript">
	var PhyModeWlan1  = '<%getHeInfo("wlanband","wlan1");%>';
	var PhyModeWlan2  = '<%getHeInfo("wlanband","wlan2");%>';
  	var ChannelWlan1 = '<%getHeInfo("channel_desc","wlan1");%>';
	var ChannelWlan2 = '<%getHeInfo("channel_desc","wlan2");%>';
	var PhyMode = '';
	var ChannelSeleted = '';
	var ChannelSeleted_5g = '';
  	var wlan_num = <% getIndex("wlan_num"); %>;
	var click_time=1;
function StylDispOn()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function show_channel_2g()
{

	PhyMode = 1*PhyMode;
	
	document.getElementById("div_11a_channel").style.visibility = "hidden";
	document.getElementById("div_11a_channel").style.display = "none";
	document.getElementById("div_11b_channel").style.visibility = "hidden";
	document.getElementById("div_11b_channel").style.display = "none";
	document.getElementById("div_11g_channel").style.visibility = "hidden";
	document.getElementById("div_11g_channel").style.display = "none";
  	document.getElementById("div_11a_channel_5g").style.visibility = "hidden";
	document.getElementById("div_11a_channel_5g").style.display = "none";
    document.getElementById("wlan5g").style.visibility = "hidden";
	document.getElementById("wlan5g").style.display = "none"; 
    document.getElementById("wlan2g").style.visibility = "visible";
	document.getElementById("wlan2g").style.display = StylDispOn();

	
	if ((PhyMode == 0) || (PhyMode == 2) || (PhyMode == 3) || (PhyMode == 8) || (PhyMode == 11))
	{
		document.getElementById("div_11g_channel").style.visibility = "visible";
		document.getElementById("div_11g_channel").style.display = StylDispOn();
		document.getElementById("Channel").name = "sz11gChannel";
	}
	else if (PhyMode == 1)
	{
		document.getElementById("div_11b_channel").style.visibility = "visible";
		document.getElementById("div_11b_channel").style.display = StylDispOn();
		document.getElementById("Channel").name = "sz11bChannel";
	}
	else if ((PhyMode == 4) || (PhyMode == 12) || (PhyMode >= 14))
	{
		document.getElementById("div_11a_channel").style.visibility = "visible";
		document.getElementById("div_11a_channel").style.display = StylDispOn();
		document.getElementById("Channel").name = "sz11aChannel";
	}
	document.getElementById("select1").value = ChannelSeleted;
}
function show_channel_5g()
{
	
  document.getElementById("wlan2g").style.visibility = "hidden";
	document.getElementById("wlan2g").style.display = "none"; 
  document.getElementById("wlan5g").style.visibility = "visible";
	document.getElementById("wlan5g").style.display = StylDispOn();
  	document.getElementById("div_11a_channel_5g").style.visibility = "visible";
	document.getElementById("div_11a_channel_5g").style.display = StylDispOn();
  
	document.getElementById("select2").value = ChannelSeleted_5g;
}
function onLoad()
{
	document.getElementById("bandselect").value="2.4G";
	document.getElementById("band").value=0;
  if(wlan_num == 1)
  {
  	document.getElementById("bandtd").style.visibility = "hidden";
  	document.getElementById("bandtd").style.display = "none";
    document.getElementById("band_select").style.visibility = "hidden";
  	document.getElementById("band_select").style.display = "none"; 
    ChannelSeleted = ChannelWlan1; 
    PhyMode = PhyModeWlan1;
  }
  if(wlan_num == 2)
  {
      ChannelSeleted = ChannelWlan2;  
      ChannelSeleted_5g = ChannelWlan1; 
      PhyMode = PhyModeWlan2;
	    document.getElementById("select1").value = ChannelSeleted;
  	  document.getElementById("select2").value = ChannelSeleted_5g;      
  }
  //default show 2g
  show_channel_2g();
}
function wifichannelsubmit()
{
	if( document.getElementById("Channel").value == "" && ((wlan_num > 1) && (document.getElementById("Channel_5g").value == "")))
	{
		alert("请选择信道");
		return;
	}

	{
		//document.wireless_channel.submit();	
		//alert("设置成功");
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
    setTimeout("self.location.reload();",30100);
    ChannelSeleted =  document.getElementById("select1").value;
    ChannelSeleted_5g =   document.getElementById("select2").value;
                                                                                       
    	$.ajax({
	      	type: "post",
	        url: "/boafrm/wireless_channel",     
	        data: $("#wireless_channel").serialize(),    
	        success: function(data){
	        	//alert("保存成功！");
	        },
	        error: function(data) {
	        	//alert("保存失败！\n" + data);
	        }
    	})	
	}
}
function wlan_band_select(value)
{
  if(value=="2.4G")
  {
  	document.getElementById("bandselect").value=value;
	document.getElementById("band").value=0;
	hide("chanstyle");
    //alert("selected as 2g");
    show_channel_2g();
  } else
  {
    document.getElementById("bandselect").value=value;
	document.getElementById("band").value=1;
	hide("chanstyle");
    //alert("selected as 5g");  
    show_channel_5g(); 
	}
}
</script>
</head>

<body onLoad="onLoad()">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a href="entconfig.asp">局域网设置</a></li>
                <li><a href="DHCPbanding.asp">DHCP设置</a></li>
				<li><a href="MACClone.asp">MAC地址克隆</a></li>
				<li class="on"><a href="wirelesschannel.asp">无线信道设置</a></li>
				<li><a href="wirelessMAC.asp">无线MAC地址访问控制</a></li>
				<li><a href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <form method="post" id="wireless_channel" name="wireless_channel" action="/boafrm/wireless_channel">
        <div class="cfgright">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="25%">&nbsp;</td>
                <td width="75%">&nbsp;</td>
              </tr>
              <!--
              <tr>
              <td id="band"><span>频段</span></td>
	              <td>
		              <select id="band_select" name="band" onchange="wlan_band_select()">
		                <option value=0>2.4G</option>
		                <option value=1>5G</option>
		              </select>
	              </td>
	           </tr>
				-->
				 <tr id="band_select">
					  <td width="20%" id="bandtd"><span>频段</span></td>
							<td>
								<div class="fix" style=" z-index:999;">
									<div class="ganyin" onclick="hide('chanstyle')" style=" width:250px;"></div>
										<div id="chanstyle" style="display: none; " class="bm">
										<ul style=" width:250px;">
											<li  onclick="wlan_band_select('2.4G')" >2.4G</li>
											<li  onclick="wlan_band_select('5G')">5G</li>
										</ul>
									</div>
								</div>
							<input type="text" class="selectinp" id="bandselect" value="" />
							<input type="hidden" id="band" name="band" value="">
							</td>
		  			</tr>
        
              <tr name="wlan2g" id="wlan2g">
                <td><span>无线信道</span></td>
                <td><div class="fix" style=" z-index:999;">
											<div id="test1_auto" class="ganyin" onClick="hide('test1')" style=" width:250px;"></div>
										 	<div id="test1" style="display: none; " class="bm">
												<ul style=" width:250px;">
													<li  onClick="pick('0','select1','test1','AutoSelect')">AutoSelect</li>
													<div id="div_11a_channel">
														<li onClick="pick('36','select1','test1','5180MHz (Channel 36)')">5180MHz (Channel 36) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('40','select1','test1','5200MHz (Channel 40)')">5200MHz (Channel 40) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('44','select1','test1','5220MHz (Channel 44)')">5220MHz (Channel 44) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('48','select1','test1','5240MHz (Channel 48)')">5240MHz (Channel 48) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('52','select1','test1','5260MHz (Channel 52)')">5260MHz (Channel 52) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('56','select1','test1','5280MHz (Channel 56)')">5280MHz (Channel 56) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('60','select1','test1','5300MHz (Channel 60)')">5300MHz (Channel 60) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('64','select1','test1','5320MHz (Channel 64)')">5320MHz (Channel 64) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('100','select1','test1','5500MHz (Channel 100)')">5500MHz (Channel 100) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('104','select1','test1','5520MHz (Channel 104)')">5520MHz (Channel 104) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('108','select1','test1','5540MHz (Channel 108)')">5540MHz (Channel 108) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('112','select1','test1','5560MHz (Channel 112)')">5560MHz (Channel 112) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('116','select1','test1','5580MHz (Channel 116)')">5580MHz (Channel 116) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('120','select1','test1','5600MHz (Channel 120)')">5600MHz (Channel 120) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('124','select1','test1','5620MHz (Channel 124)')">5620MHz (Channel 124) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('128','select1','test1','5640MHz (Channel 128)')">5640MHz (Channel 128) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('132','select1','test1','5660MHz (Channel 132)')">5660MHz (Channel 132) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('136','select1','test1','5680MHz (Channel 136)')">5680MHz (Channel 136) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('140','select1','test1','5700MHz (Channel 140)')">5700MHz (Channel 140) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('149','select1','test1','5745MHz (Channel 149)')">5745MHz (Channel 149) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('153','select1','test1','5765MHz (Channel 153)')">5765MHz (Channel 153) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('157','select1','test1','5785MHz (Channel 157)')">5785MHz (Channel 157) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('161','select1','test1','5805MHz (Channel 161)')">5805MHz (Channel 161) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('165','select1','test1','5825MHz (Channel 165)')">5825MHz (Channel 165) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('169','select1','test1','5845MHz (Channel 169)')">5845MHz (Channel 169) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('173','select1','test1','5865MHz (Channel 173)')">5865MHz (Channel 173) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>

													</div>
													<div id="div_11b_channel" >
														<li onClick="pick('1','select1','test1','2412MHz (Channel 1)')">2412MHz (Channel 1) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('2','select1','test1','2417MHz (Channel 2)')">2417MHz (Channel 2) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('3','select1','test1','2422MHz (Channel 3)')">2422MHz (Channel 3) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('4','select1','test1','2427MHz (Channel 4)')">2427MHz (Channel 4) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('5','select1','test1','2432MHz (Channel 5)')">2432MHz (Channel 5) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('6','select1','test1','2437MHz (Channel 6)')">2437MHz (Channel 6) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('7','select1','test1','2442MHz (Channel 7)')">2442MHz (Channel 7) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('8','select1','test1','2447MHz (Channel 8)')">2447MHz (Channel 8) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('9','select1','test1','2452MHz (Channel 9)')">2452MHz (Channel 9) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('10','select1','test1','2457MHz (Channel 10)')">2457MHz (Channel 10) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('11','select1','test1','2462MHz (Channel 11)')">2462MHz (Channel 11) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>

													</div>
													<div id="div_11g_channel" >
														<li onClick="pick('1','select1','test1','2412MHz (Channel 1)')">2412MHz (Channel 1) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('2','select1','test1','2417MHz (Channel 2)')">2417MHz (Channel 2) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('3','select1','test1','2422MHz (Channel 3)')">2422MHz (Channel 3) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('4','select1','test1','2427MHz (Channel 4)')">2427MHz (Channel 4) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('5','select1','test1','2432MHz (Channel 5)')">2432MHz (Channel 5) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('6','select1','test1','2437MHz (Channel 6)')">2437MHz (Channel 6) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('7','select1','test1','2442MHz (Channel 7)')">2442MHz (Channel 7) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('8','select1','test1','2447MHz (Channel 8)')">2447MHz (Channel 8) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('9','select1','test1','2452MHz (Channel 9)')">2452MHz (Channel 9) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('10','select1','test1','2457MHz (Channel 10)')">2457MHz (Channel 10) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick('11','select1','test1','2462MHz (Channel 11)')">2462MHz (Channel 11) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>

													</div>
												</ul>
											</div>
									</div>
									<input type="hidden" id="Channel"  name=""/>
									<input class="selectinp" type="text" id="select1" name="select1"/>
								</td>      
        			</tr>
              <tr name="wlan5g" id="wlan5g">
                <td><span>无线信道</span></td>
                <td><div class="fix" style=" z-index:999;">
											<div id="test2_auto" class="ganyin" onClick="hide('test2')" style=" width:250px;"></div>
										 	<div id="test2" name="test2" style="display: none; " class="bm">
												<ul style=" width:250px;">
													<li  onClick="pick1('0','select2','test2','AutoSelect')">AutoSelect</li>
													<div id="div_11a_channel_5g">
														<li onClick="pick1('36','select2','test2','5180MHz (Channel 36)')">5180MHz (Channel 36) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('40','select2','test2','5200MHz (Channel 40)')">5200MHz (Channel 40) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('44','select2','test2','5220MHz (Channel 44)')">5220MHz (Channel 44) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('48','select2','test2','5240MHz (Channel 48)')">5240MHz (Channel 48) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('52','select2','test2','5260MHz (Channel 52)')">5260MHz (Channel 52) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('56','select2','test2','5280MHz (Channel 56)')">5280MHz (Channel 56) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('60','select2','test2','5300MHz (Channel 60)')">5300MHz (Channel 60) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('64','select2','test2','5320MHz (Channel 64)')">5320MHz (Channel 64) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('100','select2','test2','5500MHz (Channel 100)')">5500MHz (Channel 100) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('104','select2','test2','5520MHz (Channel 104)')">5520MHz (Channel 104) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('108','select2','test2','5540MHz (Channel 108)')">5540MHz (Channel 108) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('112','select2','test2','5560MHz (Channel 112)')">5560MHz (Channel 112) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('116','select2','test2','5580MHz (Channel 116)')">5580MHz (Channel 116) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('120','select2','test2','5600MHz (Channel 120)')">5600MHz (Channel 120) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('124','select2','test2','5620MHz (Channel 124)')">5620MHz (Channel 124) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('128','select2','test2','5640MHz (Channel 128)')">5640MHz (Channel 128) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('132','select2','test2','5660MHz (Channel 132)')">5660MHz (Channel 132) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('136','select2','test2','5680MHz (Channel 136)')">5680MHz (Channel 136) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('140','select2','test2','5700MHz (Channel 140)')">5700MHz (Channel 140) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('149','select2','test2','5745MHz (Channel 149)')">5745MHz (Channel 149) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('153','select2','test2','5765MHz (Channel 153)')">5765MHz (Channel 153) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('157','select2','test2','5785MHz (Channel 157)')">5785MHz (Channel 157) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('161','select2','test2','5805MHz (Channel 161)')">5805MHz (Channel 161) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('165','select2','test2','5825MHz (Channel 165)')">5825MHz (Channel 165) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('169','select2','test2','5845MHz (Channel 169)')">5845MHz (Channel 169) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
<li onClick="pick1('173','select2','test2','5865MHz (Channel 173)')">5865MHz (Channel 173) &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</li>
							
													</div>
												</ul>
											</div>
									</div>
                  <input type="hidden" id="Channel_5g"  name=""/>
                  					<input class="selectinp" type="text" id="select2" name="select2"/>
								</td>
					
        			</tr>	  
<!--						              
              <tr>
                <td><span>信号强度</span></td>
                <td><div class="fix" style=" z-index:998;">
						<div class="ganyin" onClick="hide('test2')" style=" width:250px;"></div>
							<div id="test2" style="display: none; " class="bm">
							<ul style=" width:250px;">
								<li id="a11" onClick="pick('测试1','select2','test2')" >测试1</li>
								<li id="a12" onClick="pick('测试2','select2','test2')" >测试2</li>
								<li id="a13" onClick="pick('测试3','select2','test2')" >测试3</li>
								<li id="a14" onClick="pick('测试4','select2','test2')" >测试4</li>
								<li id="a15" onClick="pick('测试5','select2','test2')" >测试5</li>
							</ul>
						</div>
					</div>
					<input class="selectinp" type="text" id="select2" name="textfield2" /></td>
              </tr>
-->              
            </table>
            <div class="subpart">
                <ul>
                    <li><b onclick="wifichannelsubmit();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			

        </div>
        </form>
    </div>
</div>
<script type="text/javascript">
function $$$$$(_sId){
 return document.getElementById(_sId);
 }
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
		if(_sId=="chanstyle"&&click_time==1)
		{
		click_time=2;
		document.getElementById('test1_auto').style.display="none";
		document.getElementById('test2_auto').style.display="none";
		return;
		}
	if(_sId=="chanstyle"&&click_time==2)
		{
		click_time=1;
		document.getElementById('test1_auto').style.display="block";
		document.getElementById('test2_auto').style.display="block";
		}
 }
function pick(v,targetid,abc,val) {
	document.getElementById(targetid).value=val;
	document.getElementById("Channel").value=v;
	hide(abc);
}

function pick1(v,targetid,abc,val) {
	document.getElementById(targetid).value=val;
	document.getElementById("Channel_5g").value=v;
	hide(abc);
}
</script>
</body>
</html>
