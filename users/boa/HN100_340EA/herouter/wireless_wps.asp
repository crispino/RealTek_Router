<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; border-bottom:1px #d3d3d3 solid;}
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
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#EEEEED url(images/arrow1.png) no-repeat 170px center;}
.cfgleft li.on a{color:#1e9ec5;}
.cfgright{ float:right; width:470px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:0px; font-size:14px;}
.cfgright .spheight{ height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.cfgright td b{ font-weight:normal; padding-right:22px; color:#999; line-height:26px; display:block; height:26px; float:left; background:url(images/help.png) no-repeat right center;}
.cfgright td i{ color:#999; font-style:normal;}

.radiobox{ margin-top:-2px;}
.inp1 {height:26px; border:1px #d2d2d2 solid; width:160px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.subpart{ height:48px; padding-left:100px; padding-top:20px;}
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
.bm li{ color:#0781d9; line-height:30px; padding-left:10px; height:30px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}

.radiobox{ margin-top:-2px;}
.selectswitch{ height:29px;}
.selectswitch em{ float:left; height:20px; width:20px; display:block; margin:4px; padding:0;}
.selectswitch .closestate{ background:url(images/wifistateicon.png) no-repeat;}
.selectswitch .openstate{background:url(images/wifistateicon.png) no-repeat 0 -40px;}
.cfgright td .selectswitch span{ float:left; padding-left:0px; background:url(images/selbg.png) no-repeat; width:55px; height:29px; display:block;}
.selectswitch strong{ display:block; margin:3px; height:23px; width:49px; background:url(images/select.png) no-repeat right center; cursor:pointer;}
.switchoff{}
.switchoff .closestate{ background-position:0 -20px;}
.switchoff .openstate{ background-position:0 -60px;}
.switchoff  strong{ background-position:left center;}
.inp1{ height:26px; border:1px #d2d2d2 solid; width:230px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.inp2{ height:26px; border:1px #d2d2d2 solid; width:37px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.inp3{ height:26px; border:1px #d2d2d2 solid; width:162px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.checkinp{height:26px; width:26px; padding:0; border:none; background:#f5f5f5;}
.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}

.configbtn{ height:52px; padding-left:0px; padding-top:20px;}
.configbtn .cancel{display:block; cursor:pointer; width:95px; font-weight:normal; padding-left:26px;
				background:#77d1df; font-size:16px; color:#fff; text-align:center; text-vlaign:center;}
.configbtn b:hover{ background:#56bbcb;}

-->
</style>
<script type="text/javascript">

var wpsdisable = '<%getHeInfo("wscDisable", "wlan2")%>';
var wifi_off = '<%getHeInfo("wifi_off", "wlan2")%>';
var wlan_mode = '<%getHeInfo("wlanMode", "wlan2")%>';
var encrypt = '<%getHeInfo("encrypt", "wlan2")%>';
var wpsenable;
var click_num;
var wlan_num = '<%getHeInfo("wlan_num")%>';

function isHiddenField(ishidden,hid_id){
    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";
    } else {
	      document.getElementById(hid_id).style.display = "";
    }
} 


function isCloseWps(close){
	
    if(close == 1) {
	       isHiddenField(1,"wpsconfig");
	  }else {
	       isHiddenField(0,"wpsconfig");
	}
}

function afterSubmit() {
  parent.TINY.box.hide();
  parent.location.reload();
}


$(document).ready(function() {
	click_num = 0;
	if(wpsdisable==1)
		wpsenable = 0;
	else
		wpsenable1 = 1;
	
	if(wpsenable == 0){
	    $("#state").addClass("switchoff");
	    document.wps_setting.WPSEnable.value= 0; 
		isCloseWps(1);
	    
	} else {
		$("#state").removeClass("switchoff");
		document.wps_setting.WPSEnable.value= 1; 
     	isCloseWps(0);
	}
	
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//turn to open
		  if(wpsenable==0){
			  $("#state").removeClass("switchoff");
			    document.wps_setting.WPSEnable.value= 1;			
			  }else{
			  $("#state").addClass("switchoff");
			    document.wps_setting.WPSEnable.value= 0;		  
			 }
		} else {
		  if(wpsenable == 0){
			  $("#state").addClass("switchoff");
			    document.wps_setting.WPSEnable.value= 0;
			  } else {
			  $("#state").removeClass("switchoff");
			    document.wps_setting.WPSEnable.value= 1;
			 }			
		}
	})

});

function wpsEnableSumbit()
{
	Alert("正在保存设置，请稍候", 10);	
	setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
	/*
	$.ajax({
		type: "post",
		url: "/boafrm/wps_setting",     
		data: $("#wps_setting").serialize(),    
		success: function(data){
			//alert("保存成功！");
		},
		error: function(data) {
			//alert("保存失败！\n" + data);
		}
	});
	*/
	 document.wps_setting.submit();
	var wps_enable = 1- parseInt(document.wps_setting.WPSEnable.value);
	isCloseWps(wps_enable);
}

function wpsConfigSubmit()
{
	if(wifi_off==1){
		alert("Wifi处于关闭状态，不支持WPS连接");
		return;
	}
	if(encrypt==1 || encrypt==2){
		alert("该安全模式不支持WPS连接，建议使用WPA2安全模式");
		return;
	}
	
	Alert("正在保存设置，请稍候", 10);	
	setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
	/*
	$.ajax({
		type: "post",
		url: "/boafrm/wps_config",
		data: $("#wps_config").serialize(),    
		success: function(data){
			//alert("保存成功！");
		},
		error: function(data) {
			//alert("保存失败！\n" + data);
		}
	});
	*/
	document.wps_config.submit();
}

//防止缓存
//window.setInterval("refreshCookie()", 1000);  

</script>
</head>

<body>
<h1 id="lTitle"></h1>
<p id="lIntroduction"></p>
<div class="int">
	<h1>无线设置</h1>
    <div class="cfg">
	
        <div class="cfgleft">
            <ul>
                <li><a href="wireless_guest.asp">访客网络</a></li>
				<li class="on"><a href="wireless_wps.asp">WPS设置</a></li>
				<li><a href="wireless_mac.asp">无线访问控制</a></li>
				
				<script language="JavaScript" type="text/javascript">
				if(wlan_num>1){					
					document.write("<li><a href=\"wireless_guest_5G.asp\">5G访客网络</a></li>");
					document.write("<li><a href=\"wireless_wps_5G.asp\">5GWPS设置</a></li>");
					document.write("<li><a href=\"wireless_mac_5G.asp\">5G无线访问控制</a></li>");					
				}							
				</script>	
            </ul>
        </div>


<form method="post" name="wps_setting" id="wps_setting" action="/boafrm/wps_setting">
<input type=hidden name=page value="WPSConfig">
<input type=hidden name=wlan_conf value="2860">
<input type=hidden id="phyband" name="phyband" value="2">
<input type="hidden" value="/herouter/wireless_wps.asp" name="submit-url">
  <div class="cfgright" style="padding-top:12px;">
		<div style="border-bottom:1px #efefef solid; padding-bottom:15px;">
			<table width="100%" border="0" cellspacing="0" cellpadding="0">
				  <tr>
					<td width="25%"><span>WPS开关</span></td>
					<td width="75%">
						<div class="selectswitch" id="state">
							<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
						</div>
						<input type="hidden" id="WPSEnable" name="WPSEnable" value="1">
					</td>
				  </tr>

			</table>
			<div class="subpart">
				<ul>
					<li><b onclick="wpsEnableSumbit();">保存</b></li>
					<li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
				</ul>
			</div>
		</div>
</form>
<form method="post" name="wps_config" id="wps_config" action="/boafrm/wps_config">
	<input type=hidden name=page value="WPS">
	<input type=hidden name=wlan_conf value="2860">
	<input type=hidden id="phyband" name="phyband" value="2">
	<input type=hidden name="PINPBCRadio" id="PINPBCRadio" value="2">
	<input type="hidden" value="/herouter/wireless_wps.asp" name="submit-url">
		<table width="100%" border="0" cellspacing="0" cellpadding="0" id="wpsconfig">
			<tr>
				<td style="width:25%;"><span>PIN码</span></td>
				<td width="75%">
				    <span id="pin_num" name="pin_num"><% getInfo("wscLoocalPin");%></span>
				</td>
			</tr>
			  <tr>
				<td style="width:25%;"><span>WPS按键</span></td>
				<td width="75%">
					 <div class="configbtn">
						<b class="cancel" onclick="wpsConfigSubmit();">开始配置</b>
					</div>
				</td>
			  </tr>
		</table>
   </div>
</form>
  </div>
</div>
</body>
</html>
