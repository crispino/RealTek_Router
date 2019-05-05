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
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#ECEDEB url(images/arrow1.png) no-repeat 170px center;}
.cfgleft li.on a{color:#1e9ec5;}
.cfgright{ float:right; width:470px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:50px; font-size:14px;}
.cfgright .spheight{ height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.cfgright td b{ font-weight:normal; padding-right:22px; color:#999; line-height:26px; display:block; height:26px; float:left; background:url(images/help.png) no-repeat right center;}
.cfgright td i{ color:#999; font-style:normal;}

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
.inp2{ height:26px; border:1px #d2d2d2 solid; width:37px;font-family:"微软雅黑"; color:#666; padding:0 5px;}
.inp3{ height:26px; border:1px #d2d2d2 solid; width:150px;font-family:"微软雅黑"; color:#666; padding:0 5px;}
.checkinp{height:26px; width:26px; padding:0; border:none; background:#f5f5f5;}
.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
-->
</style>
<script type="text/javascript">

var wifi_off;
var HideSSID;
var ssid2;
var click_num;
var wlan_guest_idx = '<%getHeInfo("wlan_guest_idx", "wlan2")%>';
var wlan_num = '<%getHeInfo("wlan_num")%>';

if(wlan_guest_idx==1){
	var wifi_off= '<%getHeInfo("wifi_off", "wlan2-va0")%>';
	var HideSSID= '<%getHeInfo("hiddenSSID", "wlan2-va0")%>';
	var ssid2 = '<%getHeInfo("SSID", "wlan2-va0")%>';
}

function $$$$$(_sId){
 return document.getElementById(_sId);
 }
 
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }
 
function afterSubmit() {
  parent.TINY.box.hide();
  parent.location.reload();
}

function isHiddenField(ishidden,hid_id){

    if(ishidden == 1){
	      document.getElementById(hid_id).style.display = "none";

    }else{
	      document.getElementById(hid_id).style.display = "";
    }
} 
function isCloseWIFI(close){
	
    if(close == 1){
		   isHiddenField(1,"networkName");
	  }else if(close == 0){	
		   isHiddenField(0,"networkName");
	  }
	   
} 

$(document).ready(function(){
	click_num=0;
	if(wifi_off == 0){
		$("#state").removeClass("switchoff");
		document.wireless_guest.wifihiddenButton.value= 1; 
		isCloseWIFI(0);    
	}else{
		$("#state").addClass("switchoff");
	    document.wireless_guest.wifihiddenButton.value= 0; 
		isCloseWIFI(1);
	}
	$("#switch").click(function(){
		click_num++;
		if(click_num%2 == 1){
			//turn to open
		  if(wifi_off==1){
			  $("#state").removeClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 1;					
			    isCloseWIFI(0);
			  }else{
			  $("#state").addClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 0;
			    isCloseWIFI(1);
			    
			 }
		}else{
		  if(wifi_off==1){
			  $("#state").addClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 0;

			    isCloseWIFI(1);
			  }else{
			  $("#state").removeClass("switchoff");
			    document.wireless_guest.wifihiddenButton.value = 1;

			    isCloseWIFI(0);
			 }
			
		}
	})
	
	/*var str = new Array();
	 str = ssid2.split("CMCC_");
	 document.wireless_guest.mssid_prefix.value = "CMCC_";
	 document.wireless_guest.mssid.value = str[1];
	*/
	document.wireless_guest.mssid.value = ssid2;
		
	var HideSSIDArray = HideSSID.split(";");
	if(HideSSIDArray[0] == "0"){
      document.getElementById("hssid").checked = false;
	}else{
      document.getElementById("hssid").checked = true;
	}
});

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
    return true;
}

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
    return true;
}

function check_value(){
   
	var ssidName;
	ssidName = document.wireless_guest.mssid.value;
/*	
	if(document.wireless_guest.wifihiddenButton.value == 0){
	    return true;
	}
*/	
	if(document.wireless_guest.mssid.value == "")
	{
		alert("请输入完整的网络名称");
		return false;
	}
	if(ssidName.length < 4 || ssidName.length > 26 )
	{
      alert('输入的网络名称长度需为4-26个英文字母或数字');
			return false;	
	}
	if(document.getElementById("hssid").checked == true)
	    document.wireless_guest.hssid_1.value = "1"; 
	else
		document.wireless_guest.hssid_1.value = "0";  

	//document.wireless_guest.mssid_1.value = "CMCC_" + document.wireless_guest.mssid.value;
	document.wireless_guest.mssid_1.value = document.wireless_guest.mssid.value;
	return true;
}

function wifiGuestSubmit()
{
	if(check_value() == false )
	{
		return;
	}
	else
	{
		//document.wireless_channel.submit();	
		//alert("设置成功");
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面',25)", 10100);
		/*
		 $.ajax({
      	type: "post",
        url: "/boafrm/wireless_guest",     
        data: $("#wireless_guest").serialize(),    
        success: function(data){
        	//alert("保存成功！");
        },
        error: function(data) {
        	//alert("保存失败！\n" + data);
        }
      })
      */
      document.wireless_guest.submit();
	}
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
                <li class="on"><a href="wireless_guest.asp">访客网络</a></li>
				<li><a href="wireless_wps.asp">WPS设置</a></li>
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


<form method="post" name="wireless_guest" id="wireless_guest" action="/boafrm/wireless_guest">
<input type=hidden name=page value=HeMu_guest>
<input type=hidden name=wlan_conf value="2860">
<input type=hidden name=ssidIndex value="0">
<input type=hidden name=mssid_1 id="mssid_0"  value="">
<input type=hidden name=hssid_1 id="hssid_0"  value="">
 <input type="hidden" value="/herouter/wireless_guest.asp" name="submit-url">
  <div class="cfgright" style="padding-top:12px;">	 
    <table width="100%" border="0" cellspacing="0" cellpadding="0">
		  <tr>
			<td width="25%" colspan="2"><span>网络开关</span></td>
			<td width="47%">
				<div class="selectswitch" id="state">
					<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
				</div>
				<input type="hidden" id="wifihiddenButton" name="wifihiddenButton" value="1">
			</td>
			<td width="8%" colspan="2">&nbsp;</td>
			<td width="20%" colspan="2">&nbsp;</td>
		  </tr>
		  <tr id="networkName">
			<td colspan="2"><span>网络名称</span></td>
			<td>
			  <!--<input class="inp2" disabled="disabled" type="text" maxlength="5" id="mssid_prefix" name="mssid_prefix" value="" style="height:26px;border:1px #d2d2d2 solid;width:45px;font-family:'微软雅黑';color:#666;padding:0 10px;"/>-->
			  <input class="inp3" type="text" maxlength="27" id="mssid" name="mssid" value="" onkeyup="value=value.replace(/[^\w\.\-\/]/ig,'')"/>
			</td>  
			<td align="center" colspan="2"><input class="checkinp" type="checkbox" id="hssid" name="hssid"/></td>
			<td colspan="2"><i>隐藏网络</i></td>
		</tr>
	 </table>
   <div class="subpart">
            <ul>
                <li><b onclick="wifiGuestSubmit();">保存</b></li>
                <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
            </ul>
        </div>
   </div>
</form>
  </div>
</div>
</body>
</html>

